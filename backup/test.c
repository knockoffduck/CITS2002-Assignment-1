
// Project 1 for CITS2002 2022
// Student1: 23454661

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define max_minutes 60
#define max_hours 24
#define max_days_in_week 7
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
} Command;

typedef struct
{
    char command[42];
    int remainingTime;
    bool state;
} Execute;

// Functions to pwettify code with colours
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

void crontabValidator(char *file)
{ // Function validates and checks the crontab file for any errors
    FILE *pFile = fopen(file, "r");

    if (!pFile)
    {
        red();
        printf("error loading file '%s'\n", file);
        reset();
        exit(EXIT_FAILURE);
    }

    char buffer[line_size];
    int columns;

    while (fgets(buffer, sizeof buffer, pFile) != NULL) // while loop iterates through each line of the file
    {
        char strMin[command_size]; // string variables are created to store each string from the line
        char strHr[command_size];
        char strDate[command_size];
        char strMonth[command_size];
        char strDay[command_size];
        char strCommand[command_size];

        if (buffer[0] != '#') // skips comment lines where the first character in the line is a '#'
        {
            columns = sscanf(buffer, "%s %s %s %s %s %s", strMin, strHr, strDate, strMonth, strDay, strCommand);
            // reads from the pointer to the line and stores each string seperated by whitespace into the string variables
            if (columns != 6)
            { // checks whether the line has the valid amount of strings (columns) if not then exit
                red();
                printf("error validating file '%s'\n", file);
                printf("incorrect crontab format, expected 6 columns instead got: %d\n", columns);
                reset();
                exit(EXIT_FAILURE);
            }

            else if (strcmp(strMin, "*") == 0 &&
                     strcmp(strHr, "*") == 0 &&
                     strcmp(strDate, "*") == 0 &&
                     strcmp(strMonth, "*") == 0 &&
                     strcmp(strDay, "*") == 0)
            { // checks whether the line has invalid inputs, in which each value for the column is empty
                red();
                printf("error validating file '%s'\n", file);
                printf("incorrect crontab format \n");
                printf("all values are empty for command: %s\n", strCommand);
                reset();
                exit(EXIT_FAILURE);
            }

            else if (strcmp(strMin, "*") == 0 && strcmp(strHr, "*") == 0)
            { // checks whether the line has invalid inputs, in which the hour and minute are both missing
                red();
                printf("error validating file '%s'\n", file);
                printf("incorrect crontab format \n");
                printf("minute or hour value must be present for command: %s\n", strCommand);
                reset();
                exit(EXIT_FAILURE);
            }
        }
    }
}

void estfileValidator(char *file)
{ // Function validates and checks the estimate file for any errors
    FILE *pFile = fopen(file, "r");

    if (!pFile) // checks whether the program is able to open the file, otherwise exit
    {
        red();
        printf("error loading file '%s'\n", file);
        reset();
        exit(EXIT_FAILURE);
    }

    char buffer[line_size];
    int columns;

    while (fgets(buffer, sizeof buffer, pFile) != NULL)
    {
        char strCommand[command_size];
        char strDuration[command_size];

        if (buffer[0] != '#')
        {
            columns = sscanf(buffer, "%s %s ", strCommand, strDuration);
            if (columns != 2)
            {
                red();
                printf("error validating file '%s'\n", file);
                printf("incorrect estfile format, expected 2 columns instead got: %d\n", columns);
                reset();
                exit(EXIT_FAILURE);
            }
        }
    }
}

int rangeChecker(int num, int min, int max)
{
    if ((num - min) * (num - max) <= 0)
    {
        return 0;
    }
    else
    {
        red();
        printf("%d is an invalid month\n", num);
        reset();
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
    red();
    printf("%s INVALID MONTH\n", month);
    reset();
    exit(EXIT_FAILURE);
}

char dayNametoInt(char *string)
{
    char *daysInWeek[max_days_in_week] = {"sun", "mon", "tue", "wed",
                                          "thu", "fri", "sat"};
    for (int i = 0; string[i] != '\0'; i++)
    {
        string[i] = tolower(string[i]);
    }
    for (int i = 0; i < max_days_in_week; i++)
    {
        if (strcmp(string, daysInWeek[i]) == 0)
            return i;
    }
    red();
    printf("%s INVALID DAY\n", string);
    exit(EXIT_FAILURE);
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
    int yTemp = year - (14 - month) / 12;
    int x = yTemp + yTemp / 4 - yTemp / 100 + yTemp / 400;
    int mTemp = month + 12 * ((14 - month) / 12) - 2;
    return (date + x + 31 * mTemp / 12) % 7;
    ;
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
    if (!pFile)
    {
        red();
        printf("error loading file: %s", filename);
        exit(EXIT_FAILURE);
    }

    int lines = 0;
    char buffer[line_size];

    while (fgets(buffer, sizeof(buffer), pFile) != NULL)
    {
        if (buffer[0] != '#')
            lines++;
    }
    return lines;
}

int parseEstimate(char *filename, char *stringCommand)
{
    FILE *pFile = fopen(filename, "r");

    if (!pFile)
    {
        red();
        printf("error loading file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    char buffer[line_size];
    while (fgets(buffer, sizeof(buffer), pFile) != NULL)
    {
        char command[command_size];
        char duration[command_size];
        char line[line_size];
        int columns;
        long num;

        columns = sscanf(line, "%s %s", command, duration);
        if (line[0] != '#' && strcmp(stringCommand, command) == 0)

            num = strtol(duration, NULL, 10);
        int minutes = num;
        return minutes;
    }
    printf("%s INVALID ESTIMATE FILE\n", filename);
    printf("Could not match command from crontab to estfile\n");
    exit(EXIT_FAILURE);
}

void estimateCron(int month, char *cronFile, char *estFile)
{

    FILE *pFile = fopen(cronFile, "r");

    if (!pFile)
    {
        printf("error loading file '%s'\n", cronFile);
        exit(EXIT_FAILURE);
    }
    int linesAmt = amtOfLines(cronFile);
    Command commands[linesAmt];
    Execute *status = (Execute *)malloc(sizeof *status);

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

        columns = sscanf(buffer, "%s %s %s %s %s %s", strMin, strHr, strDate, strMonth, strDay, strCommand);
        if (columns == 6 && buffer[0] != '#')
        {
            strcpy(commands[i].command, strCommand);
            if (strcmp(strMin, "*") != 0)
                commands[i].min = strtol(strMin, NULL, 10);
            else
                commands[i].min = -1;
            if (strcmp(strHr, "*") != 0)
                commands[i].hr = strtol(strHr, NULL, 10);
            else
                commands[i].hr = -1;
            if (strcmp(strDate, "*") != 0)
                commands[i].day = strtol(strDate, NULL, 10);
            else
                commands[i].day = 0;
            if (strcmp(strMonth, "*") != 0)
                commands[i].month = strtol(strMonth, NULL, 10);
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
        if (weekday == max_days_in_week)
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
    free(status);
}
int main(int argc, char **argv)
{
    int month;
    if (argc != 4)
    {
        red();
        printf("expected 3 arguments, instead got %d\n", argc - 1);
        reset();
        exit(EXIT_FAILURE);
    }
    else if (!isdigit(*argv[1]))
        month = monthNametoInt(argv[1]);
    else
    {
        month = atoi(argv[1]);
        if (rangeChecker(month, 0, 11) != 0)
        {
            red();
            printf("%d INVALID MONTH", month);
            reset();
            exit(EXIT_FAILURE);
        }
    }
    char *cronFile = argv[2];
    char *estFile = argv[3];

    crontabValidator(cronFile);
    estfileValidator(estFile);

    estimateCron(month, cronFile, estFile);

    return 0;
}