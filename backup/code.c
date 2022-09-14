#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

#define max_minutes 60
#define max_hours 24
#define max_days 7
#define month_amt 12
#define current_year 2022
#define line_size 100
#define command_size 40

typedef struct
{
    int min;
    int hr;
    int day;
    int month;
    int dayOfWeek;
    char command[42];
    int duration;
    int counter;
    bool state;
    int remainingTime;
} Command;

typedef struct
{
    char command[42];
    int remainingTime;
    bool state;
} Execute;

int amtOfLines(char *filename)
{
    FILE *pFile = fopen(filename, "r");
    int lines = 0;
    char *temp;
    if (!pFile)
    {
        printf("error loading file");
        exit(EXIT_FAILURE);
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

int main()
{
    char *cronFile = "crontab-file.txt";

    FILE *pFile = fopen(cronFile, "r");

    if (!pFile)
    {
        printf("error loading file '%s'\n", cronFile);
        exit(EXIT_FAILURE);
    }

    char line[100];
    char *token;

    int min;
    // min, hr, day, month, dayOfWeek;
    int i = 0;
    long int num;
    int columns;
    int j;

    while (fgets(line, sizeof line, pFile) != NULL)
    {
        j = 0;
        while (isspace(line[j]))
        { // Test for whitespace at start of sentence
            j++;
        }
        char strMin[command_size];
        char strHr[command_size];
        char strDate[command_size];
        char strMonth[command_size];
        char strDay[command_size];
        char strCommand[command_size];

        if (line[j] != '#')
        {
            columns = sscanf(line, "%s %s %s %s %s %s", strMin, strHr, strDate, strMonth, strDay, strCommand);

            if (columns >= 6)
                printf("%s %s %s %s %s %s\n", strMin, strHr, strDate, strMonth, strDay, strCommand);
        }
    }
    return 0;
}