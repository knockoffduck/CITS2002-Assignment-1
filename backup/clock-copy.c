#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define LINE_SIZE 100
#define COMMAND_SIZE 40
#define MAX_COMMANDS 20
#define MINUTES_IN_DAY 1440
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24
#define MONTHS_IN_YEAR 12
#define DAYS_IN_WEEK 7

struct Estimates
{
    char command[COMMAND_SIZE + 1];
    int minutes;
};

struct Crontabs
{
    int minute;
    int hour;
    int date;
    int month;
    int day;
    char command[COMMAND_SIZE + 1];
};

FILE *file_opener(char filename[])
{
    /* Returns the opened file pointer */

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    return fp;
}

int month_num(char *month)
{
    /* Returns the integer of a month given as a string */

    if (isnumber(*month))
    { // Return the month integer if it is already one
        return atoi(month);
    }
    const char *months[12] = {"jan", "feb", "mar",
                              "apr", "may", "jun",
                              "jul", "aug", "sep",
                              "oct", "nov", "dec"};

    for (int i = 0; i < 12; i++)
    {
        if (strcmp(months[i], month) == 0)
        { // Determines whether a given month is a valid month
            return i;
        }
    }
    fprintf(stderr, "%s is an invalid month\n", month);
    exit(EXIT_FAILURE);
}

int day_num(char *day)
{
    /* Returns the integer of a day given as a string */

    if (isnumber(*day))
    { // Return the day integer if it is already one
        return atoi(day);
    }

    const char *days[7] = {"sun", "mon", "tue", "wed",
                           "thu", "fri", "sat"};

    for (int i = 0; i < 7; i++)
    {
        if (strcmp(days[i], day) == 0)
        { // Compares whether given day is a valid day (or substring)
            return i;
        }
    }
    fprintf(stderr, "%s is an invalid day\n", day);
    exit(EXIT_FAILURE);
}

int days_in_month(int month)
{
    /* Returns the amount of days in the month given as an integer [1-12] */

    int daysinmonth;
    switch (month)
    {
    case 0:
        daysinmonth = 31;
        break;
    case 1:
        daysinmonth = 28;
        break;
    case 2:
        daysinmonth = 31;
        break;
    case 3:
        daysinmonth = 30;
        break;
    case 4:
        daysinmonth = 31;
        break;
    case 5:
        daysinmonth = 30;
        break;
    case 6:
        daysinmonth = 31;
        break;
    case 7:
        daysinmonth = 31;
        break;
    case 8:
        daysinmonth = 30;
        break;
    case 9:
        daysinmonth = 31;
        break;
    case 10:
        daysinmonth = 30;
        break;
    case 11:
        daysinmonth = 31;
        break;
    default:
        fprintf(stderr, "%i is an invalid month\n", month);
        exit(EXIT_FAILURE);
    }
    return daysinmonth;
}

int first_day(int month)
{
    /* Returns the first day of the month as an int [0-6] */

    int first_day;
    switch (month)
    {
    case 0:
        first_day = 6;
        break;
    case 1:
        first_day = 2;
        break;
    case 2:
        first_day = 2;
        break;
    case 3:
        first_day = 5;
        break;
    case 4:
        first_day = 0;
        break;
    case 5:
        first_day = 3;
        break;
    case 6:
        first_day = 5;
        break;
    case 7:
        first_day = 1;
        break;
    case 8:
        first_day = 4;
        break;
    case 9:
        first_day = 6;
        break;
    case 10:
        first_day = 2;
        break;
    case 11:
        first_day = 4;
        break;
    default:
        fprintf(stderr, "%i is an invalid month", month);
        exit(EXIT_FAILURE);
    }
    return first_day;
}

int read_crontabs(struct Crontabs crontabs[MAX_COMMANDS], FILE *crontab_file, int given_month)
{
    /* Assigns each crontab entry into an array of structures and
     * returns the number of entries in the array */
    int i = 0;
    int j;
    int line_words;
    char line[LINE_SIZE];
    // Reading crontab file line by line and putting contents into array of structures
    while (fgets(line, sizeof line, crontab_file) != NULL)
    {
        j = 0;
        while (isspace(line[j]))
        { // Test for whitespace at start of sentence
            j++;
        }
        char minute[LINE_SIZE];
        char hour[LINE_SIZE];
        char date[LINE_SIZE];
        char month[LINE_SIZE];
        char day[LINE_SIZE];
        char command[LINE_SIZE];

        if (line[j] != '#')
        {
            line_words = sscanf(line, "%s %s %s %s %s %s",
                                minute, hour, date, month, day, command);
            if (line_words >= 6)
            {
                if (strcmp(minute, "*") == 0)
                {
                    crontabs[i].minute = -1;
                }
                else if (isnumber(*minute))
                {
                    int minute_int = atoi(minute);
                    if (minute_int >= 0 && minute_int < MINUTES_IN_HOUR)
                    {
                        crontabs[i].minute = minute_int;
                    }
                    else
                    {
                        fprintf(stderr, "%s is not a valid minute in line %i\n", minute, j);
                        exit(EXIT_FAILURE);
                    }
                }
                if (strcmp(hour, "*") == 0)
                {
                    crontabs[i].hour = -1;
                }
                else if (isnumber(*hour))
                {
                    int hour_int = atoi(hour);
                    if (hour_int >= 0 && hour_int < HOURS_IN_DAY)
                    {
                        crontabs[i].hour = hour_int;
                    }
                    else
                    {
                        fprintf(stderr, "%s is not a valid hour in line %i\n", hour, j);
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    fprintf(stderr, "%s is not a valid hour in line %i\n", hour, j);
                    exit(EXIT_FAILURE);
                }
                if (strcmp(date, "*") == 0)
                {
                    crontabs[i].date = -1;
                }
                else if (isnumber(*date))
                {
                    int date_int = atoi(date);
                    if (date_int > 0 && date_int <= days_in_month(given_month))
                    {
                        crontabs[i].date = date_int;
                    }
                    else
                    {
                        fprintf(stderr, "%s is not a valid date in line %i\n", date, j);
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    fprintf(stderr, "%s is not a valid date in line %i\n", date, j);
                    exit(EXIT_FAILURE);
                }
                if (strcmp(month, "*") == 0)
                {
                    crontabs[i].month = -1;
                }
                else
                {
                    int month_int = month_num(month);
                    if (month_int >= 0 && month_int < MONTHS_IN_YEAR)
                    {
                        crontabs[i].month = month_int;
                    }
                    else
                    {
                        fprintf(stderr, "%i is not a valid month in line %i\n", month_int, j);
                        exit(EXIT_FAILURE);
                    }
                }
                if (strcmp(day, "*") == 0)
                {
                    crontabs[i].day = -1;
                }
                else
                {
                    int day_int = day_num(day);
                    if (day_int >= 0 && day_int < DAYS_IN_WEEK)
                    {
                        crontabs[i].day = day_int;
                    }
                    else
                    {
                        fprintf(stderr, "%i is not a valid day in line %i\n", day_int, j);
                        exit(EXIT_FAILURE);
                    }
                }
                if (strlen(command) <= COMMAND_SIZE + 1)
                {
                    strcpy(crontabs[i].command, command);
                }
                else
                {
                    fprintf(stderr, "%s is not a valid command in line %i\n", command, j);
                    exit(EXIT_FAILURE);
                }
                i++;
            }
            else
            {
                fprintf(stderr, "Invalid number of words in line %i\n", j);
                exit(EXIT_FAILURE);
            }
        }
    }
    return i;
}

int read_estimates(struct Estimates estimates[MAX_COMMANDS], FILE *estimates_file)
{
    /* Assigns each estimates entry into an array of structures and
     * returns the number of entries in the array */
    int i = 0;
    int j;
    char line[LINE_SIZE];
    int line_words;
    // Reading estimates file line by line and putting contents into array of structures
    while (fgets(line, sizeof line, estimates_file) != NULL)
    {
        j = 0;
        char command[LINE_SIZE];
        char minutes[LINE_SIZE];
        while (isspace(line[j]))
        { // Test for whitespace at start of sentence
            j++;
        }
        if (line[j] != '#')
        {
            line_words = sscanf(line, "%s %s", command, minutes);
            if (line_words >= 2)
            {
                if (strlen(command) <= COMMAND_SIZE + 1)
                {
                    strcpy(estimates[i].command, command);
                }
                else
                {
                    fprintf(stderr, "%s is not a valid command in line %i\n", command, j);
                    exit(EXIT_FAILURE);
                }
                if (isnumber(*minutes))
                {
                    int minutes_int = atoi(minutes);
                    if (minutes_int > 0)
                    {
                        estimates[i].minutes = minutes_int;
                    }
                    else
                    {
                        fprintf(stderr, "%i is not a valid minutes in line %i\n", minutes_int, j);
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    fprintf(stderr, "%s is not a valid minutes in line %i\n", minutes, j);
                    exit(EXIT_FAILURE);
                }
                i++;
            }
            else
            {
                fprintf(stderr, "Invalid number of words in line %i\n", j);
                exit(EXIT_FAILURE);
            }
        }
    }
    return i;
}

bool is_current_time(struct Crontabs crontabs[MAX_COMMANDS], int minute, int current_month, int i)
{
    /* Returns a boolean true if the minute matches that of the crontab at index i */

    int current_day = (minute / MINUTES_IN_DAY + first_day(current_month)) % DAYS_IN_WEEK;
    int current_date = (minute / MINUTES_IN_DAY) + 1;
    int current_hour = ((minute / MINUTES_IN_HOUR) % HOURS_IN_DAY);
    int current_minute = ((minute % MINUTES_IN_DAY) % MINUTES_IN_HOUR);
    bool monthflag = false;
    bool dayflag = false;
    bool dateflag = false;
    bool hourflag = false;
    bool minuteflag = false;

    if (crontabs[i].month == -1 || crontabs[i].month == current_month)
    { // Test if the month is correct
        monthflag = true;
    }
    if (crontabs[i].day == -1 || crontabs[i].day == current_day)
    { // Test if the day is correct
        dayflag = true;
    }
    if (crontabs[i].date == -1 || crontabs[i].date == current_date)
    { // Test if the date is correct
        dateflag = true;
    }
    if (crontabs[i].hour == -1 || crontabs[i].hour == current_hour)
    { // Test if the hour is correct
        hourflag = true;
    }
    if (crontabs[i].minute == -1 || crontabs[i].minute == current_minute)
    { // Test if the minute is correct
        minuteflag = true;
    }
    if (monthflag && dayflag && dateflag && hourflag && minuteflag)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void estimatecron(char *month, FILE *crontab_file, FILE *estimates_file)
{
    /* Calculates and prints:
     * 1. The name of the most frequently executed command
     * 2. The total number of commands invoked
     * 3. The maximum number of commands running at any time
     */

    // Defining variables
    int i;
    int j;
    int pid = 0;                      // Total commands run in the month
    int nrunning = 0;                 // Number of current commands running at a particular time
    int max_nrunning = 0;             // Highest value nrunning reaches
    int crontabs_size;                // Size of crontabs array
    int estimates_size;               // Size of estimates array
    int counter_size = 0;             // Size of counter array
    int timer_size = 0;               // Size of timer array
    int month_int = month_num(month); // Converts given month to an int
    bool errorflag;                   // Flag for errors

    struct
    {
        char command[COMMAND_SIZE + 1];
        int counter;
    } counter[MAX_COMMANDS]; // Array of structures for counting times each command is run

    struct
    {
        char command[COMMAND_SIZE + 1];
        int timer;
    } timer[MAX_COMMANDS]; // Array of structures for assigning timers of when a command will terminate

    struct Crontabs crontabs[MAX_COMMANDS];
    crontabs_size = read_crontabs(crontabs, crontab_file, month_int); // Writes crontabs to array of structures and assigns the size

    struct Estimates estimates[MAX_COMMANDS];
    estimates_size = read_estimates(estimates, estimates_file); // Writes estimates to array of structures and assigns the size

    // Test if each crontabs command is in the estimates file
    for (i = 0; i < crontabs_size; ++i)
    {
        errorflag = true;
        for (j = 0; j < estimates_size; ++j)
        {
            if (strcmp(crontabs[i].command, estimates[j].command) == 0)
            {
                errorflag = false;
            }
        }
        if (errorflag)
        {
            fprintf(stderr, "Command %s is not in both files\n", crontabs[i].command);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < MAX_COMMANDS; i++)
    {
        timer[i].timer = -1;    // Initialise all timers to -1
        counter[i].counter = 0; // Initialise all counters to 0
    }
    for (int minute = 0; minute < (days_in_month(month_int) * MINUTES_IN_DAY) + 1; minute++)
    {
        for (i = 0; i < timer_size + 1; i++)
        {
            if (timer[i].timer >= 0)
            {
                --timer[i].timer; // Decrement each timer every minute
            }
            if (timer[i].timer == 0)
            {
                --nrunning; // Decrement number of command running if its timer finishes
                printf("%s terminated at minute %i, pid = %i, nrunning = %i\n", crontabs[i].command, minute, pid, nrunning);
            }
        }
        timer_size = 0;
        for (i = 0; i < crontabs_size; i++)
        {
            if (is_current_time(crontabs, minute, month_int, i))
            {               // Test if the current time matches any in crontabs
                ++pid;      // Increment total commands run
                ++nrunning; // Increment current running commands
                printf("%s invoked at minute %i, pid = %i, nrunning = %i\n", crontabs[i].command, minute, pid, nrunning);
                if (nrunning > max_nrunning)
                {
                    max_nrunning = nrunning;
                }
                bool duplicate = false; // Duplicate flag
                for (j = 0; j < counter_size + 1; j++)
                {
                    if (strcmp(counter[j].command, crontabs[i].command) == 0)
                    { // Test if command is already in counter array
                        duplicate = true;
                    }
                }
                if (!duplicate)
                {
                    strcpy(counter[counter_size].command, crontabs[i].command); // Add command to counter if it's not a duplicate
                    ++counter[counter_size].counter;
                    ++counter_size;
                }
                else
                {
                    for (j = 0; j < counter_size + 1; j++)
                    {
                        if (strcmp(counter[j].command, crontabs[i].command) == 0)
                        {
                            ++counter[j].counter; // Increment the command counter if it is a duplicate
                        }
                    }
                }
                j = 0;
                while (timer[j].timer != -1)
                { // Find first free timer array
                    ++j;
                    ++timer_size;
                }
                strcpy(timer[j].command, crontabs[i].command); // If it is then store the command in array position
                for (int m = 0; m < estimates_size; m++)
                {
                    if (strcmp(estimates[m].command, timer[j].command) == 0)
                    {
                        timer[j].timer = estimates[m].minutes; // Store commands estimated minute in timer
                    }
                }
            }
        }
    }
    int max_counter = 0;
    char max_counter_name[COMMAND_SIZE];
    for (i = 0; i < counter_size; i++)
    {
        if (counter[i].counter > max_counter)
        {
            max_counter = counter[i].counter;
            strcpy(max_counter_name, counter[i].command);
        }
    }
    printf("The command which ran the most times was %s\n", max_counter_name);
    printf("The total amount of commands run was %i\n", pid);
    printf("The total number of commands running at a single time was %i\n", max_nrunning);
    printf("\n%s       %i       %i\n", max_counter_name, pid, max_nrunning);
}

int main(int argc, char *argv[])
{

    // Check if the correct number of arguments are given
    if (argc != 4)
    {
        fprintf(stderr, "%s expected 3 arguments, instead got %i\n", argv[0], argc - 1);
    }
    // Defining variables
    char *month = argv[1];
    FILE *crontab_file = file_opener(argv[2]);
    FILE *estimates_file = file_opener(argv[3]);

    // Running program
    estimatecron(month, crontab_file, estimates_file);

    exit(EXIT_SUCCESS);
    return 0;
}