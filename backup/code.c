// CITS2002 PROJECT 1 2022
// STUDENT 1: 23234683 TRINH LINH
// STUDENT 2: 23147438 NGUYEN THAI HOANG LONG

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_COMMANDS 20
#define MAX_CHAR 100

struct Crontab
{
    char minute[MAX_CHAR];
    char hour[MAX_CHAR];
    char day[MAX_CHAR];
    char month[MAX_CHAR];
    char date[MAX_CHAR];
    char command[41];
};

struct Estimate
{
    char command[41];
    char min[MAX_CHAR];
};

FILE *openfile(char filename[])
{
    // If file is able to be opened, return the opened file

    FILE *files = fopen(filename, "r");
    if (files == NULL)
    {
        fprintf(stderr, "File does not exist or cannot be opened '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    return files;
}

int fileProcessCron(FILE *crontab, struct Crontab crontab_data[MAX_COMMANDS])
{
    int line = 0; // keeps track of every line in file and also act as pointer for struct idex
    int j = 0;    // keeps track of the individual character in a line
    int file_size = 0;

    char lines[MAX_CHAR];

    while (fgets(lines, sizeof lines, crontab) != NULL)
    {
        char cmin[MAX_CHAR];
        char chour[MAX_CHAR];
        char cday[MAX_CHAR];
        char cmonth[MAX_CHAR];
        char cdate[MAX_CHAR];
        char cmd[41];

        while (isspace(lines[j]))
        { // if the character is a white space, continute to examine next character
            j++;
        }

        if (lines[j] != '#')
        { // if the first nons-space character is not a # then store data into struct
            sscanf(lines, "%s %s %s %s %s %s",
                   cmin, chour, cday, cmonth, cdate, cmd);
            strcpy(crontab_data[line].minute, cmin);
            strcpy(crontab_data[line].hour, chour);
            strcpy(crontab_data[line].day, cday);
            strcpy(crontab_data[line].month, cmonth);
            strcpy(crontab_data[line].date, cdate);
            strcpy(crontab_data[line].command, cmd);
            line++;
            file_size++;
        }
    }
    return file_size;
}

int fileProcessEstimate(FILE *estimate, struct Estimate est_data[MAX_COMMANDS])
{
    int line = 0; // keeps track of every line in file and act as pointer for struct index
    int j = 0;    // keeps track of the individual character in a line
    int file_size = 0;

    char lines[MAX_CHAR];

    while (fgets(lines, sizeof lines, estimate) != NULL)
    {
        char cmd[41];
        char minute[MAX_CHAR];

        while (isspace(lines[j]))
        { // if the character is a white space, continute to examine next character
            j++;
        }

        if (lines[j] != '#')
        { // if the first nons-space character is not a # then store data into struct
            sscanf(lines, "%s %s",
                   cmd, minute);
            strcpy(est_data[line].command, cmd);
            strcpy(est_data[line].min, minute);
            line++;
            file_size++;
        }
    }
    return file_size;
}

int daysOfMonth(int month)
{
    // returns max number of days in month;
    int days;
    switch (month)
    {
    case 0:
    case 2:
    case 4:
    case 6:
    case 7:
    case 9:
    case 11:
        days = 31;
        break;
    case 3:
    case 5:
    case 8:
    case 10:
        days = 30;
        break;
    case 1:
        days = 28;
        break;
    default:
        fprintf(stderr, "Month %i is an invalid input\n", month);
        exit(EXIT_FAILURE);
    }
    return days;
}

int dayToInt(char *day)
{
    // if given a string (mon-sun) convert that to a number
    int num = 0;
    if ((isnumber(*day)))
    {
        if (atoi(day) < 7)
        {
            num = atoi(day); // if it is already an int that is less than 7, return that
            return num;
        }
    }

    else if (strcmp(day, "*") == 0)
    {
        return -1;
    }

    else
    {
        char *days[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
        for (int i = 0; i < 7; i++)
        {
            if ((strcmp(days[i], day)) == 0)
            { // Compares whether given day is a valid day (or substring)
                num = i;
                return num;
            }
        }
    }

    fprintf(stderr, "Day provided is an invalid input\n");
    exit(EXIT_FAILURE);
}

int monthToInt(char *month)
{
    // if given a string (jan-dec) convert that to a number
    int num;
    if (isnumber(*month))
    {
        if (atoi(month) < 11)
        {
            num = atoi(month);
            return num;
        }
    }
    else if (strcmp(month, "*") == 0)
    {
        return -1;
    }
    else
    {
        const char *months[12] = {"jan", "feb", "mar", "apr", "may",
                                  "jun", "jul", "aug", "sep", "oct", "nov", "dec"};

        for (int i = 0; i < 12; i++)
        {
            if (strcmp(months[i], month) == 0)
            { // Compares whether month match with any months array item
                num = i;
                return num;
            }
        }
    }
    fprintf(stderr, "Month is an invalid input\n");
    exit(EXIT_FAILURE);
}

int toInteger(char *value)
{
    int num;
    if (isnumber(*value))
    {
        num = atoi(value);
        return num;
    }
    else
    {
        fprintf(stderr, "Value %s is an invalid input\n", value);
        exit(EXIT_FAILURE);
    }
}

void reportError(struct Crontab crontab_data[MAX_COMMANDS], int cronSize)
{

    for (int i = 0; i < cronSize; i++)
    {
        // reporting errors on minute in crontab,
        if (strcmp(crontab_data[i].minute, "*") != 0)
        { // if minute is not * -->
            if (atoi(crontab_data[i].minute) < 0 || atoi(crontab_data[i].minute) > 59)
            { // check if its < 0 or > 59, if yes report error and exit
                fprintf(stderr, "Minute provided is out of scope\n");
                exit(EXIT_FAILURE);
            }
        }

        // reporting errors on hour in crontab
        if (strcmp(crontab_data[i].hour, "*") != 0)
        { // if hour is not * -->
            if (atoi(crontab_data[i].hour) < 0 || atoi(crontab_data[i].hour) > 23)
            { // check if its <0 or > 23, if yes report error and exit
                fprintf(stderr, "Hour provided is out of scope\n");
                exit(EXIT_FAILURE);
            }
        }

        // reporting errors on date in crontab
        if (strcmp(crontab_data[i].day, "*") != 0)
        { // if date is not * -->

            if (strcmp(crontab_data[i].month, "*") == 0)
            { // if month is equals to * report generic error
                if (atoi(crontab_data[i].day) < 0 || atoi(crontab_data[i].day) > 31)
                { // date < 0 or date > 31 then exit and report error
                    fprintf(stderr, "Date provided is out of scope\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(crontab_data[i].month, "*") != 0)
            { // and if the month provided is also not * -->
                int currentMonth = monthToInt(crontab_data[i].month);
                int maxDay = daysOfMonth(currentMonth);
                // report error accordingly to the specified month
                if (atoi(crontab_data[i].day) < 0 || atoi(crontab_data[i].day) > maxDay)
                { // if the date provided is < 0 or is larger than the max date for the provide month, report error and exit
                    fprintf(stderr, "Date provided is out of scope\n");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // reporting errors on month in crontab
        if (strcmp(crontab_data[i].month, "*") != 0)
        { // if day is not * -->
            int currentMonth = monthToInt(crontab_data[i].month);
            if (currentMonth < 0 || currentMonth > 11)
            { // if month is < 0 or > 11, report error and exit
                fprintf(stderr, "Month provided is out of scope\n");
                exit(EXIT_FAILURE);
            }
        }

        // reporting errors on day in crontab
        if (strcmp(crontab_data[i].date, "*") != 0)
        { // if dayis not * -->
            int currentDay = dayToInt(crontab_data[i].date);
            if (currentDay < 0 || currentDay > 6)
            { // if day is < 0 or > 6, report error and exit
                fprintf(stderr, "Day provided is out of scope\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int started(int month, struct Crontab crontab_data[MAX_COMMANDS], int i, int min)
{
    // returns 0 if the command provided started in the minute that we are looping through
    int hour = (min / 60) % 24;              // remainder of total min and 24 will give the current hour
    int currentMin = (min % (24 * 60)) % 60; // remainder of min / total min in day and 60 wil give what minute we are iterating through
    int date = (min / (24 * 60)) + 1;

    // if crontab month is equals to month provided or *
    if (monthToInt(crontab_data[i].month) == month || strcmp(crontab_data[i].month, "*") == 0)
    {
        // if crontab day is equals to day we are looping through
        if (atoi(crontab_data[i].day) == date || strcmp(crontab_data[i].day, "*") == 0)
        {
            // if crontab hour is equals to the hour we are looping through
            if (atoi(crontab_data[i].hour) == hour || strcmp(crontab_data[i].hour, "*") == 0)
            {
                // if crontab minute is equals to the minute we are looping through
                if (atoi(crontab_data[i].minute) == currentMin || strcmp(crontab_data[i].minute, "*") == 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void estimatecron(char *month, FILE *crontab_file, FILE *estimates_file)
{
    int maxCommand = 0;     // max command invoked at any particular time
    int invokedCommand = 0; // total invoked command in the month
    int nrunning = 0;       // keeps track of the amount of command running while looping through every minute
    int cmdMonth = monthToInt(month);
    int processPointer = 0; // pointer to keep track of where we are in the Process array, allows us to append to end of array
    int commandPointer = 0; // pointer to keep track of the end index in the Commands array, allows us to append to end of array

    // reading files and storing into arrays of structs
    struct Crontab crontab_data[MAX_COMMANDS];
    struct Estimate est_data[MAX_COMMANDS];
    int estSize = fileProcessEstimate(estimates_file, est_data);
    int cronSize = fileProcessCron(crontab_file, crontab_data);
    reportError(crontab_data, cronSize);

    for (int n = 0; n < estSize; n++)
    {
        printf("command: %s, min: %s \n", est_data[n].command, est_data[n].min);
    }

    for (int m = 0; m < estSize; m++)
    {
        printf(" min: %s , hour: %s, day: %s, month: %s, date: %s, command: %s\n",
               crontab_data[m].minute, crontab_data[m].hour, crontab_data[m].day, crontab_data[m].month, crontab_data[m].date, crontab_data[m].command);
    }

    struct Process
    {
        char name[41];  // keeps track of no of commands running during the same process
        int finishTime; // timer for the process
    } process[MAX_COMMANDS];

    struct Commands
    {
        char name[41]; // keeps track of name of commands
        int ninvoked;  // keeps track of number of times it was invoked
    } commands[MAX_COMMANDS];

    for (int i = 0; i < MAX_COMMANDS; i++)
    {
        // initialised all comamnds invoked time and finish time to 0, -1
        commands[i].ninvoked = 0;
        process[i].finishTime = -1;
    }

    // iterate through every minute in the provided and month and log commands
    int minInMonth = (60 * 24) * daysOfMonth(cmdMonth); // no of minutes in the given month
    for (int min = 0; min <= minInMonth; min++)
    {
        // determine if process finishes at the minute we are iterating through
        for (int n = 0; n < processPointer; n++)
        {
            if (process[n].finishTime > 0)
            {
                process[n].finishTime--; // decrement every process timer
            }

            else if (process[n].finishTime == 0)
            {
                nrunning--; // if a process is finished, decrement nrunning
            }
        }

        // testing if a command starts in that minute
        for (int i = 0; i < cronSize; i++)
        {
            /*- as we are looping through the minutes, determine if a command starts in that minute,
             - if yes, increment nrunning and command invoked, then see if nrunning is > maxcommand running at one time and update if needed
             - then find the command that just started and match it with appropriate command in estimate file, store the execution time of that command
             and its name to process array
             - check if the command that just started already exist in the command array, if yes then increment ninvoked of that command, if not then
             add it to command array and increment the pointer to end of array

             as we are looping through the minutes, loop through process array in order to decrement

            */

            if (started(cmdMonth, crontab_data, i, min) == 0)
            { // if a command start during this minute increments nrunning and invoked commands
                invokedCommand++;
                nrunning++;
                printf("Command executed: %s at: %s:%s:%s:%s:%s\n",
                       crontab_data[i].command, crontab_data[i].minute, crontab_data[i].hour, crontab_data[i].day, crontab_data[i].month, crontab_data[i].date);

                if (nrunning > maxCommand)
                {
                    maxCommand = nrunning;
                }
            }

            for (int j = 0; j < estSize; j++)
            {
                if (strcmp(est_data[j].command, crontab_data[i].command) == 0) // finding the command that just started and add its execution time to process array
                {
                    int minRunning = toInteger(est_data[j].min);
                    strcpy(process[processPointer].name, crontab_data[i].command);
                    process[processPointer].finishTime = minRunning;
                    processPointer++;
                }
            }

            for (int k = 0; k < commandPointer; k++)
            {
                // if the command already exist in command array, increment no time it was invoked
                if (strcmp(crontab_data[k].command, commands[k].name) == 0)
                {
                    commands[commandPointer].ninvoked++;
                }
                // if command did not already exist, add it to commands array
                else
                {
                    strcpy(commands[commandPointer].name, crontab_data[i].command);
                    commands[commandPointer].ninvoked++;
                    commandPointer++;
                }
            }
        }
    }

    // determine commands that was called the most
    int maxInvoked = 0;
    char maxCommandName[41];
    for (int i = 0; i < commandPointer; i++)
    {
        if (commands[i].ninvoked > maxInvoked)
        {
            maxInvoked = commands[i].ninvoked;
            strcpy(maxCommandName, commands[i].name);
        }
    }

    printf("Number of commands invoked: %i, most frequently executed command: %s, max commands running: %i\n", invokedCommand, maxCommandName, maxCommand);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "4 arguments required\n");
        exit(EXIT_FAILURE);
    }
    char *month = argv[1];
    FILE *crontab = openfile(argv[2]);
    FILE *estimate = openfile(argv[3]);

    estimatecron(month, crontab, estimate);
}
