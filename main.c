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

void red()
{
    printf("\033[1;31m");
}

void reset()
{
    printf("\033[0m");
}

void green()
{
    printf("\033[0;32m");
}

int rangeChecker(int num, int min, int max)
{
    if ((num - min) * (num - max) <= 0)
    {
        return 0;
    }
    else
    {
        printf("%d is an invalid month\n", num);
        exit(EXIT_FAILURE);
    }
}

int monthNametoInt(char *month)
{
    int m;

    char *months[12] = {"jan", "feb", "mar",
                        "apr", "may", "jun",
                        "jul", "aug", "sep",
                        "oct", "nov", "dec"};
    for (int i = 0; i < sizeof(months); i++)
    {
        if (strcmp(month, months[i]) == 0)
            return i;
    }

    printf("%s INVALID MONTH\n", month);
    exit(EXIT_FAILURE);
}

char dayNametoInt(char *string)
{
    char *temp;
    for (int i = 0; string[i] != '\0'; i++)
    {
        string[i] = tolower(string[i]);
    }

    if (strcmp(string, "sun") == 0)
        string = "0";

    if (strcmp(string, "mon") == 0)
        string = "1";

    if (strcmp(string, "tue") == 0)
        string = "2";

    if (strcmp(string, "wed") == 0)
        string = "3";

    if (strcmp(string, "thu") == 0)
        string = "4";

    if (strcmp(string, "fri") == 0)
        string = "5";

    if (strcmp(string, "sat") == 0)
        string = "6";

    return strtol(string, &temp, 10);
}

int find_max_value(float array[], int length)
{
    // set the value of index 0 as the "current max value"
    float max_value = array[0];
    // the same goes for the index number
    int max_index = 0;

    // go through the array
    for (int i = 1; i < length; i++)
    {
        // if the next index's value is greater than the "current max value"
        // update the max_value and max_index
        if (array[i] > max_value)
        {
            max_value = array[i];
            max_index = i;
        }
    }
    return max_index;
}

int getDayOfWeek(int month, int date)
{
    int day = 0;
    int year = current_year;
    month += 1;
    int year_temp = year - (14 - month) / 12;
    int x = year_temp + year_temp / 4 - year_temp / 100 + year_temp / 400;
    int month_temp = month + 12 * ((14 - month) / 12) - 2;
    day = (date + x + 31 * month_temp / 12) % 7;
    return day;
}
// Function to return the amount of days in given month
int daysInMonthAmt(int month)
{
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int daysAmt;
    daysAmt = daysInMonth[month];
    return daysAmt;
}

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

int parseEstimate(char *filename, char *stringCommand)
{
    FILE *pFile = fopen(filename, "r");

    if (!pFile)
    {
        printf("error loading file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(pFile, 0, SEEK_END);

    int size = ftell(pFile);

    rewind(pFile);

    char *buffer = (char *)malloc(sizeof(char) * (size + 1));
    while (fgets(buffer, (size + 1), pFile) != NULL)
    {
        char *token;
        char *temp;
        char *temp2;
        char *strNum;

        temp = buffer;
        if (buffer[0] == '#')
            continue;

        token = strtok(temp, " ");
        temp = NULL;
        if (strncmp(stringCommand, token, sizeof(&token)) == 0)
        {

            token = strtok(NULL, " ");
            long int num = strtol(token, &temp2, 10);
            return num;
        }
        token = strtok(NULL, " ");
    }
    return 0;
}

void estimateCron(int month, char *cronFile, char *estFile)
{
    int linesAmt = amtOfLines(cronFile);
    Command commands[linesAmt];
    Execute *status = (Execute *)malloc(sizeof *status);

    FILE *pFile = fopen(cronFile, "r");

    if (!pFile)
    {
        printf("error loading file '%s'\n", cronFile);
        exit(EXIT_FAILURE);
    }

    char buffer[line_size];

    int i = 0;
    int columns;
    while (fgets(buffer, sizeof buffer, pFile) != NULL)
    {
        char strMin[command_size];
        char strHr[command_size];
        char strDate[command_size];
        char strMonth[command_size];
        char strDay[command_size];
        char strCommand[command_size];
        char *temp2;

        columns = sscanf(buffer, "%s %s %s %s %s %s", strMin, strHr, strDate, strMonth, strDay, strCommand);
        if (columns == 6 && buffer[0] != '#')
        {
            strcpy(commands[i].command, strCommand);
            if (strcmp(strMin, "*") != 0)
                commands[i].min = strtol(strMin, &temp2, 10);
            else
                commands[i].min = -1;
            if (strcmp(strHr, "*") != 0)
                commands[i].hr = strtol(strHr, &temp2, 10);
            else
                commands[i].hr = -1;
            if (strcmp(strDate, "*") != 0)
                commands[i].day = strtol(strDate, &temp2, 10);
            else
                commands[i].day = 0;
            if (strcmp(strMonth, "*") != 0)
                commands[i].month = strtol(strMonth, &temp2, 10);
            else
                commands[i].month = -1;
            if (strcmp(strDay, "*") != 0)
                commands[i].dayOfWeek = dayNametoInt(strDay);
            else
                commands[i].dayOfWeek = -1;
            if (i < linesAmt)
                i++;
        }
    }
    fclose(pFile);

    for (int i = 0; i < linesAmt; i++)
    {
        commands[i].duration = parseEstimate(estFile, commands[i].command);
    }
    int days = daysInMonthAmt(month);

    int minute;
    int minCount;
    int hour;
    int date = 1;

    hour = minute = 0;
    int amtProcesses = 0;
    int queue = 0;
    int maxQueue = 0;
    int weekday = getDayOfWeek(month, date);
    for (int i = 0; i < linesAmt; i++)
    {
        commands[i].counter = 0;
    }

    while (1)
    {
        minute++;

        if (minute == max_minutes)
        {
            hour += 1;
            minute = 0;
        }
        if (hour == max_hours)
        {
            date++;
            weekday++;
            hour = 0;
            minute = 0;
        }
        if (weekday == max_days)
        {
            weekday = 0;
        }

        for (int i = 0; i < linesAmt; i++)
        {

            if (queue > maxQueue)
                maxQueue = queue;

            if ((minute == commands[i].min || commands[i].min == -1) &&
                (hour == commands[i].hr || commands[i].hr == -1) &&
                (date == commands[i].day || commands[i].day == 0) &&
                (month == commands[i].month || commands[i].month == -1) &&
                (weekday == commands[i].dayOfWeek || commands[i].dayOfWeek == -1))
            {
                queue++;
                strcpy(status[amtProcesses].command, commands[i].command);
                status[amtProcesses].state = true;
                status[amtProcesses].remainingTime = commands[i].duration;
                commands[i].counter++;
                green();
                printf("@%02d:%02d\t%02d/%02d\tinvoke\t(pid=%d, running=%d)\t\t%s\n",
                       hour,
                       minute,
                       date,
                       month,
                       amtProcesses,
                       queue,
                       commands[i].command);
                reset();
                amtProcesses++;
            }
        }
        for (int j = 0; j < amtProcesses; j++)
        {
            if (status[j].state == true &&
                status[j].remainingTime > 0)
                status[j].remainingTime--;
            else if (status[j].state == true &&
                     status[j].remainingTime == 0)
            {
                if (queue > 0)
                    queue--;
                status[j].state = false;
                red();
                printf("@%02d:%02d\t%02d/%02d\thas-terminated\t(pid=%d, running=%d)\t%s\n", hour, minute, date, month, amtProcesses, queue, status[j].command);
                reset();
            }
        }

        if (date > days)
        {
            for (int i = 0; i < linesAmt; i++)
            {
                commands[i].remainingTime = 0;
            }
            break;
        }
    }
    printf("\n------------------------------------\n");
    float count[linesAmt];
    for (int i = 0; i < linesAmt; i++)
    {
        count[i] = commands[i].counter;
        printf("%d\t", commands[i].counter);
        printf("%s\n", commands[i].command);
    }
    int indexOfMaxProcess = find_max_value(count, linesAmt);

    printf("\n------------------------------------\n");
    printf("%s %i %i", commands[indexOfMaxProcess].command, amtProcesses, maxQueue);
    printf("\n------------------------------------\n");
}
int main(int argc, char **argv)
{
    int month;
    if (argc != 4)
    {
        printf("%s expected 3 arguments, instead got %d\n", argv[0], argc - 1);
    }
    else if (!isdigit(*argv[1]))
        month = monthNametoInt(argv[1]);
    else
    {
        month = atoi(argv[1]);
        if (rangeChecker(month, 0, 11) != 0)
        {
            printf("%d INVALID MONTH", month);
            exit(EXIT_FAILURE);
        }
    }
    char *cronFile = argv[2];
    char *estFile = argv[3];

    estimateCron(month, cronFile, estFile);

    return 0;
}