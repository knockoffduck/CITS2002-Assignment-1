
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
{ // struct array for each unique command and its fields
    int min;
    int hr;
    int day;
    int month;
    int dayOfWeek;
    char command[command_size + 1];
    int duration; // Specifies the duration of the command
    int counter;
    char test[command_size]; // Specifies the amount of times the command was executed
} Command;

typedef struct
{ // struct array for the status of each non-unique command executed
    char command[command_size + 1];
    int remainingTime; // specifies the remaining time from the command duration, this is used to calculate when a command execution has expired
    bool state;        // specifies the state in which the executed command is in. This is used to observe whether the command.
} Execute;

// Colour functions to prettify code with colours
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

FILE *openFile(char *file) 
{   // Function to open the file, assign a pointer to the file and raise an error if unable to open the file
    FILE *pFile = fopen(file, "r");

    if (!pFile)
    {
        red();
        printf("error loading file '%s'\n", file);
        reset();
        exit(EXIT_FAILURE);
    }
    return pFile;
}

void crontabValidator(char *file)
{ // Function validates and checks the crontab file for any errors
    FILE *pFile = openFile(file);
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
            // reads from the pointer to the line and stores each string separated by whitespace into the string variables
            if (columns != 6)
            { // checks whether the line has the valid amount of strings (columns) if not then exit
                red();
                printf("error validating file '%s'\n", file);
                printf("incorrect crontab format, expected 6 columns instead got: %d\n", columns);
                reset();
                exit(EXIT_FAILURE);
            }

            else if (strcmp(strMin, "*") == 0 && strcmp(strHr, "*") == 0 && strcmp(strDate, "*") == 0 &&
                     strcmp(strMonth, "*") == 0 && strcmp(strDay, "*") == 0)
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

void estFileValidator(char *file)
{ // Function validates and checks the estimate file for any errors
    FILE *pFile = openFile(file);

    char buffer[line_size];
    int columns;

    while (fgets(buffer, sizeof buffer, pFile) != NULL) // while loop iterates through each line of the file
    {
        char strCommand[command_size]; // string variables are created to store each string from the line
        char strDuration[command_size];

        if (buffer[0] != '#') // skips comment lines where the first character in the line is a '#'
        {
            columns = sscanf(buffer, "%s %s ", strCommand, strDuration);
            if (columns != 2)
            { // checks whether the line has the valid amount of strings (columns) if not then exit
                red();
                printf("error validating file '%s'\n", file);
                printf("incorrect estFile format, expected 2 columns instead got: %d\n", columns);
                reset();
                exit(EXIT_FAILURE);
            }
        }
    }
}

int rangeChecker(int num, int min, int max)
{ // Function to check whether a integer parameter is in the specified range
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
{ // Function to convert the first three letters of a month to int (0,...,11)
    char *months[12] = {"jan", "feb", "mar",
                        "apr", "may", "jun",
                        "jul", "aug", "sep",
                        "oct", "nov", "dec"};
    for (int i = 0; i < sizeof(months); i++)
    { // Loop through the string array and return the index as the integer format of the month
        if (strcmp(month, months[i]) == 0)
            return i;
    }
    red();
    printf("%s INVALID MONTH\n", month);
    reset();
    exit(EXIT_FAILURE);
}

char dayNametoInt(char *string)
{ // Function to convert the day of the week to integer format (0, ..., 6)
    char *daysInWeek[max_days_in_week] = {"sun", "mon", "tue", "wed",
                                          "thu", "fri", "sat"};
    for (int i = 0; string[i] != '\0'; i++)
    { // loops through the characters in the string and converts each character to lower
        string[i] = tolower(string[i]);
    }
    for (int i = 0; i < max_days_in_week; i++)
    { // Loop through string array and return the index as the integer format of the weekday
        if (strcmp(string, daysInWeek[i]) == 0)
            return i;
    }
    red(); // If the function is unable to convert the string to integer then exit with error
    printf("%s INVALID DAY\n", string);
    exit(EXIT_FAILURE);
}

int getMaxValue(float *array, int length)
{
    // set the value of index 0 as the "current max value"
    float highestValue = array[0];
    int maxIndex = 0;

    // loop through the array
    for (int i = 1; i < length; i++)
    {
        // if the value of the next index is greater than the current highest value then update the variables max_value and max_index
        if (array[i] > highestValue)
        {
            highestValue = array[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

int getDayOfWeek(int month, int date)
{ // Function to return the day of the week given the date, month and year
    int year = current_year;
    month++;
    return (date += month < 3 ? year-- : year - 2, 23 * month / 9 + date + 4 + year / 4 - year / 100 + year / 400) % 7;
}

int daysInMonthAmt(int month)
{ // Function to return the amount of days in given month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int daysAmt;
    daysAmt = daysInMonth[month]; // utilises the month integer value as the index to the array of list
    return daysAmt;
}

int amtOfLines(char *filename)
{ // Function to return the amount of lines in crontab file
    FILE *pFile = openFile(filename);

    int lines = 0;
    char buffer[line_size];

    while (fgets(buffer, sizeof(buffer), pFile) != NULL)
    { // Loop through each line in the file and if it is not a comment line then add 1 to lines amount
        if (buffer[0] != '#')
            lines++;
    }
    return lines;
}

int parseEstimate(char *filename, char *stringCommand)
{ // Function to return the duration of the specified command from the estimates file
    FILE *pFile = openFile(filename);
    int columns;

    char buffer[line_size];
    while (fgets(buffer, sizeof(buffer), pFile) != NULL)
    { // loop through each line in the file
        char command[command_size];
        char duration[command_size];

        if (buffer[0] != '#')
        {
            columns = sscanf(buffer, "%s %s", command, duration); // parse the strings from the two columns separated by whitespace into strings variables
            if (strcmp(command, stringCommand) == 0)
            // checks whether the line is a comment line and the command parameter matches the command in the line, then return the duration converted from string to int
            {
                return atoi(duration);
            }
        }
    }
    printf("%s INVALID ESTIMATE FILE\n", filename); // if the function hasn't returned a value then raise error
    printf("Could not match command from crontab to estFile\n");
    exit(EXIT_FAILURE);
}

void estimateCron(int month, char *cronFile, char *estFile)
{
    // primary function to execute and simulate the commands by incrementing each minute in a month given the month, cronFile name and estimate filename parameters
    FILE *pFile = openFile(cronFile);
    int linesAmt = amtOfLines(cronFile);
    Command commands[linesAmt];
    Execute *status = (Execute *)malloc(sizeof *status); // allocate memory to the status struct so the length of the struct can be dynamic to accommodate for the amount of commands executed

    char buffer[line_size];

    int i = 0;
    int columns;
    while (fgets(buffer, sizeof buffer, pFile) != NULL) // while loop iterates through each line of the file
    {
        char strMin[command_size]; // string variables are created to store each string from the line
        char strHr[command_size];
        char strDate[command_size];
        char strMonth[command_size];
        char strDay[command_size];
        char strCommand[command_size];

        columns = sscanf(buffer, "%s %s %s %s %s %s", strMin, strHr, strDate, strMonth, strDay, strCommand); // reads from the pointer to the line and stores each string separated by whitespace into the string variables
        if (columns == 6 && buffer[0] != '#')
        {                                            // skips comment lines where the first character in the line is a '#'
            strcpy(commands[i].command, strCommand); // copy the command from the line into the commands struct
            if (strcmp(strMin, "*") != 0)            // checks whether the column is empty and copies the integer value converted from string to the specific commands struct field otherwise use -1 or 0 to represent an empty value
                commands[i].min = atoi(strMin);
            else
                commands[i].min = 0;
            if (strcmp(strHr, "*") != 0)
                commands[i].hr = atoi(strHr);
            else
                commands[i].hr = -1;
            if (strcmp(strDate, "*") != 0)
                commands[i].day = atoi(strDate);
            else
                commands[i].day = 0;
            if (strcmp(strMonth, "*") != 0)
                commands[i].month = atoi(strMonth);
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

    int days = daysInMonthAmt(month);

    int minute;
    int hour;
    int date = 1;

    hour = minute = 0;
    int amtProcesses = 0;
    int queue = 0;
    int maxQueue = 0;
    int weekday = getDayOfWeek(month, date);
    for (int i = 0; i < linesAmt; i++)
    { // loop through the commands and set the counter field from commands struct to 0 and set the duration for the command using the parseEstimate function to obtain the duration from the estimate file
        commands[i].counter = 0;
        commands[i].duration = parseEstimate(estFile, commands[i].command);
    }

    while (1)
    { // main loop to increment each minute in the month, simulating a clock
        minute++;

        if (minute == max_minutes) // if 60 minutes have passed then increment the hour and reset minutes
        {
            hour += 1;
            minute = 0;
        }
        if (hour == max_hours) // if 24 hours have passed then increment the days and the weekday and reset hours and minutes
        {
            date++;
            weekday++;
            hour = 0;
            minute = 0;
        }
        if (weekday == max_days_in_week) // if 7 weekdays has passed (a week) then reset to the start of next week (sun)
        {
            weekday = 0;
        }

        for (int i = 0; i < linesAmt; i++) // loop through each command and check whether the command needs to be executed at the current minute using the minute, hour, date and month variables
        {

            if (queue > maxQueue) // checks whether the queue is the highest its ever been then replace the highest queue with the current queue
                maxQueue = queue;

            if (minute == commands[i].min &&
                (hour == commands[i].hr || commands[i].hr == -1) &&
                (date == commands[i].day || commands[i].day == 0) &&
                (month == commands[i].month || commands[i].month == -1) &&
                (weekday == commands[i].dayOfWeek || commands[i].dayOfWeek == -1))
            { // checks whether the command needs to be executed at the current minute, if so then increment the queue and copy the command to the status struct with amtProcesses as the index to iterate through each command executed. Change the status state of the command to true, then copy the duration to the remaining time field in the status struct of the command. Then increment the command's count
                queue++;
                strcpy(status[amtProcesses].command, commands[i].command);
                status[amtProcesses].state = true;
                status[amtProcesses].remainingTime = commands[i].duration;
                commands[i].counter++;
                green();
                printf("@ %02d:%02d\t%02d/%02d\tinvoke\t\t(pid=%d, running=%d)\t%s\n", hour, minute, date, month, amtProcesses, queue, commands[i].command);
                reset();
                amtProcesses++;
            }
        }
        for (int j = 0; j < amtProcesses; j++)
        { // loops through the commands executed
            if (status[j].state == true &&
                status[j].remainingTime > 0) // check whether the command is running then decrement the remaining time by 1 minute
                status[j].remainingTime--;
            else if (status[j].state == true &&
                     status[j].remainingTime == 0)
            { // check whether the remaining time for the command has finished then remove the command from the queue count and change the state of the command status to false
                if (queue > 0)
                    queue--;
                status[j].state = false;
                red();
                printf("@ %02d:%02d\t%02d/%02d\thas-terminated\t(pid=%d, running=%d)\t%s\n", hour, minute, date, month, amtProcesses, queue, status[j].command);
                reset();
            }
        }

        if (date > days) // check whether it is the end of the month then stop the loop
            break;
    }
    printf("------------------------------------\n");
    float count[linesAmt];
    for (int i = 0; i < linesAmt; i++)
    { // loop through the commands and print the amount of times the command has executed
        count[i] = commands[i].counter;
        printf("%d\t", commands[i].counter);
        printf("%s\n", commands[i].command);
    }
    int indexOfMaxProcess = getMaxValue(count, linesAmt);

    printf("------------------------------------\n");
    printf("%s %i %i\n", commands[indexOfMaxProcess].command, amtProcesses, maxQueue); // print the final assessed output
    free(status);                                                                      // free the memory for the status struct
}
int main(int argc, char **argv)
{ // main function to initiate the program
    int month;
    if (argc != 4) // check whether there is a valid amount of arguments specified from the command line
    {
        red();
        printf("expected 3 arguments, instead got %d\n", argc - 1);
        reset();
        exit(EXIT_FAILURE);
    }
    else if (!isdigit(*argv[1])) // check whether the month input is a string then convert to integer format
        month = monthNametoInt(argv[1]);
    else
    {
        month = atoi(argv[1]);
        if (rangeChecker(month, 0, 11) != 0)
        { // check whether the month is a valid month
            red();
            printf("%d INVALID MONTH", month);
            reset();
            exit(EXIT_FAILURE);
        }
    }
    char *cronFile = argv[2];
    char *estFile = argv[3];

    crontabValidator(cronFile); // initialise checking functions to verify the validity of the files
    estFileValidator(estFile);

    estimateCron(month, cronFile, estFile); // initialise primary function to execute the program

    return 0;
}