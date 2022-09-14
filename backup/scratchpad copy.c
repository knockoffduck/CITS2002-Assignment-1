#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

typedef struct
{
    int min;
    int hr;
    int day;
    int month;
    int dayOfWeek;
    char command[41];
} Command;

int amtOfLines(char *filename)
{
    FILE *pFile = fopen(filename, "r");
    int lines = 0;
    char *temp;
    if (!pFile)
    {
        printf("error loading file '%s'\n", filename);
        return 404;
    }
    fseek(pFile, 0, SEEK_END);
    int size = ftell(pFile);
    rewind(pFile);
    char *buffer = (char *)malloc(sizeof(char) * (size + 1));

    while (fgets(buffer, (size + 1), pFile) != NULL)
    {
        if (buffer[0] == '#')
            continue;
        lines++;
    }
    return lines;
}

int main(int argc, char **argv)
{
    char *filename = "crontab-file.txt";
    Command commands[amtOfLines(filename)];

    int linesAmt = amtOfLines(filename);

    FILE *pFile = fopen(filename, "r");

    if (!pFile)
    {
        printf("error loading file '%s'\n", filename);
        return 1;
    }

    int read = 0;
    int commandsAmt = 0;

    do
    {
        read = fscanf(pFile,
                      "%d %d")
    }

    char *buffer = (char *)malloc(sizeof(char) * (size + 1));

    fclose(pFile);

    return 0;
}