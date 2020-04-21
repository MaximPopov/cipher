#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CYPHER_LENGTH 100
#define ERROR_ARGUMENTS 1
#define ERROR_CYPHER 2

#define STR_LEN(x) STR(x)
#define STR(x) #x

typedef struct {
    char original[MAX_CYPHER_LENGTH + 1];
    char permutation[MAX_CYPHER_LENGTH + 1];
} Cypher;

static const char *programName;
static Cypher cypher;

void printUsageAndExit(int exitCode)
{
    printf("Usage: %s -k:<keyfile> [<file1>[, <file2>]...]", programName);
    exit(exitCode);
}

const char *parseArguments(int argc, char *argv[])
{
    const char *keyfileName = NULL;

    if (argc <= 1)
    {
        printUsageAndExit(ERROR_ARGUMENTS);
    }

    for (int index = 1; index < argc; index++)
    {
        const char *arg = argv[index];
        if (arg[0] == '-' || arg[0] == '/' || arg[0] == '\\')
        {
            const char *option = arg + 1;
            if (strcmp(option, "?") == 0 || strcmp(option, "h") == 0)
            {
                printUsageAndExit(0);
            }

            if (strncmp(option, "k:", 2) == 0)
            {
                keyfileName = option + 2;
            }
        }
    }

    if (keyfileName == NULL)
    {
        printUsageAndExit(ERROR_ARGUMENTS);
    }

    return keyfileName;
}

void readCypher(const char *keyfileName, Cypher *cypher, int maxCypherLength)
{
    FILE *fp = fopen(keyfileName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Couldn't open key file %s: %s", keyfileName, strerror(errno));
        exit(ERROR_CYPHER);
    }

    fscanf(fp, "%" STR_LEN(MAX_CYPHER_LENGTH) "s", cypher->original);
    fscanf(fp, "%" STR_LEN(MAX_CYPHER_LENGTH) "s", cypher->permutation);

    fclose(fp);
}

void validateCypher(Cypher *cypher)
{
    int originalLength = strlen(cypher->original);
    int permutationLength = strlen(cypher->permutation);
    if (originalLength != permutationLength)
    {
        fprintf(stderr, "Invalid cypher: original length and permutation length do not match.");
        exit(ERROR_CYPHER);
    }

    for (int index = 0; index < originalLength; index++)
    {
        if (!isalpha(cypher->original[index]))
        {
            fprintf(stderr, "Invalid cypher: non-alpha character in original string at position %d.", index);
            exit(ERROR_CYPHER);
        }
        if (!isalpha(cypher->permutation[index]))
        {
            fprintf(stderr, "Invalid cypher: non-alpha character in permutation string at position %d.", index);
            exit(ERROR_CYPHER);
        }
    }
}

char applyCypher(char ch, Cypher *cypher)
{
    char encodedCh = ch;
    int isLower = islower(ch);
    const char *pch = strchr(cypher->original, toupper(ch));

    if (pch != NULL)
    {
        encodedCh = *(cypher->permutation + (pch - cypher->original));
        if (isLower)
        {
            encodedCh = tolower(encodedCh);
        }
    }

    return encodedCh;
}

int main(int argc, char *argv[])
{
    programName = argv[0];

    const char *keyfileName = parseArguments(argc, argv);
    readCypher(keyfileName, &cypher, MAX_CYPHER_LENGTH);
    validateCypher(&cypher);

    char ch;
    while ((ch = getc(stdin)) != EOF)
    {
        putchar(applyCypher(ch, &cypher));
    }

    return 0;
}