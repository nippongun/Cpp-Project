/**
 * Expense tracker application made by Simon Bauer, 2020. University project for the course "C-programming"
 * Course offered and held by the Metropolia University of Applied Science
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_LENGTH 80
#define MAX_ENTRIES 512
typedef struct entry
{
    int type;
    char description[MAX_LENGTH];
    float amount;
    unsigned long date;
} entry;

void serializer(FILE *file, entry *entries, int lines);
entry add_entry(int type);

int initialize(FILE *file, char *filename);
void print_report(entry *entries, int lines);
void validate_input(char *input, int maxLength);
void error(char *error);
void sort(entry *entries, int lines, int (*f)());

void sort_menu(entry *entries, int lines);

int compare_type(const void *a, const void *b);
int compare_amount(const void *a, const void *b);
int compare_description(const void *a, const void *b);
int compare_date(const void *a, const void *b);

char *unix_to_utc(unsigned long time);

int main()
{
    char i = 0;
    FILE *file;
    entry entries[MAX_ENTRIES];
    int lines = 0; // Keeps the count of entries
    char filename[MAX_LENGTH];
    printf("Enter a file name.\n");
    validate_input(filename, MAX_LENGTH);

    file = fopen(filename, "rb");
    // The file opens in rb mode to check whether the file is present
    if (file == NULL)
    {
        // If not, it asks the user initialize the given file.
        printf("Do you want to create a new file instead? [Y/n]: ");
        // If the user agrees, the file gets created.
        if (getchar() == 'y')
        {
            initialize(file, filename);
            fclose(file);
        }
        else
        {
            // ... else the programs exits.
            printf("Exit program.");
            return 1;
        }
    }

    // A second check up in case the file is not loaded
    if (file == NULL)
    {
        printf("Unable to open file %s.\n Exit program.\n", filename);
        return 1;
    }
    else
    {
        // Load the file into the memory
        lines = fread(&entries,sizeof(entry),MAX_ENTRIES,file); // deserializer
        printf("There are currently %d entries in %s\n", lines, filename);
        fclose(file);
        do
        {
            printf("\nEnter one of the following numbers:\n1: Initializes a file (i.e. delets all records).\n2: Save all entered records to the file.\n3: Read/Load all records from the file.\n4: Add income.\n5: Add expense.\n6: Print a report.\n7: Opens  submenu for sorting the entries\n8: Quit\n");

            printf("\nEnter a number\n");
            i = getchar();
            while (getchar() != '\n'); // This voids all potential characters from the buffer of stdin

            // MENU
            switch (i)
            {
            case '1':
                // Clear the file
                if (initialize(file, filename))
                {
                    printf("Error while initializing or clearing file %s", filename);
                }
                fclose(file);
                fread(&entries,sizeof(entry),MAX_ENTRIES,file);
                lines = 0;
                break;
            case '2':
                // Save the array to the file
                if (lines == 0)
                {
                    // if the array is empty, tell user.
                    error("Cannot save records to file because no records are given");
                }
                // Otherwise save to
                file = fopen(filename, "wb");
                serializer(file, entries, lines);
                fclose(file); // Reminder: The file always gets closed to avoid errors.

                break;
            case '3':
                // Load entries from file.
                file = fopen(filename, "rb");
                lines = fread(&entries,sizeof(entry),MAX_ENTRIES,file);
                fclose(file);
                break;
            case '4':
                // Add income entry
                entries[lines] = add_entry(1);
                lines++;
                printf("Remember to save the entry!\n");
                break;
            case '5':
                // Add expense entry
                entries[lines] = add_entry(-1);
                lines++;
                printf("Remember to save the entry!\n");
                break;
            case '6':
                // Print the report
                print_report(entries, lines);
                break;
            case '7':
                // Open sub menu for sorting
                // Extra function to avoid a full main menu
                sort_menu(entries, lines);
                break;
            default:
                break;
            }

        } while (i != '8');
    }
    printf("See you!");
    free(entries);
    return 0;
}

/*----------------------------------------------------------------------*/

/**
 * initialize returns an integer whether the given file was opened successfully. It opens a file in wb+ mode to clear all entries.
*/
int initialize(FILE *file, char *filename)
{
    file = fopen(filename, "wb+");
    if (!file)
    {
        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------*/

/**
 * serializer takes an entry array and saves it to the given file. 
*/
void serializer(FILE *file, entry *entries, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        fwrite(&entries[i],sizeof(entry),1,file);
    }
}


entry * deserializer(FILE * file, int * lines, entry * entries){
    *lines = fread(&entries,sizeof(entry),MAX_ENTRIES,file);
    return entries;
}

/*----------------------------------------------------------------------*/
/**
 * add_entry take an integer indicating the type of entry and returns a newly created entry.
*/
entry add_entry(int type)
{
    char in[MAX_LENGTH];
    entry new_entry;

    new_entry.type = type;

    printf("Enter the description: (MAX 80 characters)\n");
    validate_input(in, MAX_LENGTH);
    // %[^\t\n] Accepts all entered values besides new line feed and tab.
    if (sscanf(in, "%[^\t\n]", new_entry.description) == 0)
    {
        error("Error for description");
    }

    printf("Enter the amount of money:\n");
    validate_input(in, MAX_LENGTH);
    if (sscanf(in, "%f", &new_entry.amount) == 0)
    {
        error("Error for amount");
    }

    // Time is fetched from the computer
    new_entry.date = (unsigned long)time(NULL);
    return new_entry;
}

/*----------------------------------------------------------------------*/

/**
 * print_report returns a void and takes an array of entries and prints all entries. After printing, the function prints also the total sum of expenses and incomes.
*/
void print_report(entry *entries, int lines)
{
    float expense_sum = 0.0;
    float income_sum = 0.0;
    printf("\n%s - %-80s - %-16s - %-s\n", "TYPE", "DESCRIPTION", "MONEY", "DATE");
    for (int i = 0; i < lines; i++)
    {
        if (entries[i].type == -1)
        {
            expense_sum += entries[i].amount;
        }
        else
        {
            income_sum += entries[i].amount;
        }

        if (entries[i].type == -1)
        {
            printf("-    | ");
        }
        else
        {
            printf("+    | ");
        }
        printf("%-80s - %16f - ", entries[i].description, entries[i].amount);
        unix_to_utc(entries[i].date);
        printf("\n");
    }
    printf("Total expenses: %f - Total income: %f\nDifference: %f\n", expense_sum, income_sum, (income_sum - expense_sum));
}

void sort(entry *entries, int lines, int (*f)())
{
    qsort(entries, lines, sizeof(entry), f);
}

/*----------------------------------------------------------------------*/
/*Utility functions*/
/*----------------------------------------------------------------------*/

void validate_input(char *input, int max_length)
{
    if (!fgets(input, max_length, stdin))
    {
        input = "Error";
    }

    if (input[strlen(input) - 1] == '\n')
    {
        input[strlen(input) - 1] = '\0';
    }
}

void error(char *error)
{
    printf("Error: '%s'", error);
}
/**
 * unix_to_utc prints a human readable time from a unix time
*/
char *unix_to_utc(unsigned long time)
{
    char buf[MAX_LENGTH];
    struct tm ts;
    ts = *localtime(&time);
    strftime(buf, sizeof(buf), "%x", &ts);
    printf(buf);
    return buf;
}

/*----------------------------------------------------------------------*/
/*Comparisosn functions and the menu funciton*/
/*----------------------------------------------------------------------*/

void sort_menu(entry *entries, int lines)
{
    printf("Enter a number:\n1: To sort by the type\n2: To sort by amount.\n3: To sort by description.\n4: To sort by date.\n");
    char c = getchar();
    while (getchar() != '\n')
        ;
    switch (c)
    {
    case '1':
        sort(entries, lines, compare_type);
        break;
    case '2':
        sort(entries, lines, compare_amount);
        break;
    case '3':
        sort(entries, lines, compare_description);
        break;
    case '4':
        sort(entries, lines, compare_date);
        break;
    default:
        break;
    }
}

/**
 * Compares the types of entries
*/
int compare_type(const void *a, const void *b)
{
    entry *val1 = (int *)a;
    entry *val2 = (int *)b;

    return (val1->type - val2->type);
}

int compare_amount(const void *a, const void *b)
{
    entry *val1 = (int *)a;
    entry *val2 = (int *)b;

    return (val1->amount - val2->amount);
}

int compare_description(const void *a, const void *b)
{
    entry *val1 = (const char *)a;
    entry *val2 = (const char *)b;

    return (strcmp(val1->description, val2->description));
}

int compare_date(const void *a, const void *b)
{
    entry *val1 = (int *)a;
    entry *val2 = (int *)b;

    return (val1->date - val2->date);
}