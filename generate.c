#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void buildPermutation(const char *source, char* destination, FILE *log)
{
    int charCount = strlen(source);
    memset(destination, '-', charCount * sizeof(char));
    destination[charCount] = '\0';

    for (int index = 0; index < charCount; index++)
    {
        int remainingPositions = charCount - index;
        int randomOffset = remainingPositions * rand() / (RAND_MAX + 1);
        char offsetSkippedPositions = ' ';

        if (destination[randomOffset] == '-')
        {
            destination[randomOffset] = source[index];
        }
        else
        {
            char *p = destination;
            int vacantPositionCount = randomOffset;
            while (1)
            {
                if (*p == '-')
                {
                    vacantPositionCount--;
                    if (vacantPositionCount < 0)
                    {
                        break;
                    }
                }
                p++;
            }
            *p = source[index];
            offsetSkippedPositions = '*';
        }

        if (log)
        {
            fprintf(log, "%c\toffset: %2d %c\t%s\n", source[index], randomOffset, offsetSkippedPositions, destination);
        }
    }
}

int main(int argc, char *argv[])
{
    srand(time(0));

    FILE *log = NULL;
    if (argc > 1)
    {
        if (strcmp(argv[1], "-v") == 0)
        {
            log = stdout;
        }
    }

    const char original[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char permutation[sizeof(original)];

    buildPermutation(original, permutation, log);

    printf("%s\n%s\n", original, permutation);
    return 0;
}