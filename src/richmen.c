#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Declaring struct date to save date of birth
typedef struct {
    int year;    //Year of birth
    int month;   //Month of birth
    int day;     //Day of birth
} date;

// Declare struct btime to save time of birth
typedef struct {
    int hour;    //Hour of birth
    int minutes; //Minutes of birth
} btime;

// Declare struct billionaire to save the information 
typedef struct bilList {
    char name[100];         //Name
    char city[50];          //City
    char country[50];       //Country
    char activity[100];     //Activity
    char origin_country[50];//Country of origin
    char gender[10];        //Gender
    date birthdate;         //Birthdate
    btime birthtime;        //Birthtime
    double net_worth;       //Net worth in billions
    char account_number[14];//Bank account number
    struct bilList *next;   //Pointer to the next record in the list
    struct bilList *prev;   //Pointer to the previous record in the list
} billionaire;

//Function to create a new billionaire
billionaire* create_billionaire(char *name, char *city, char *country, char *activity, char *origin_country, char *gender, date birthdate, btime birthtime, double net_worth, char *account_number) {
    // Memory allocation for the new record
    billionaire *new_billionaire = (billionaire *)malloc(sizeof(billionaire));
    if (new_billionaire == NULL) {
        printf("Memory allocation failed.\n");
        exit(1); // Exit the program if memory allocation fails
    }
    //Copy the values to the corresponding structure variables
    strcpy(new_billionaire->name, name);
    strcpy(new_billionaire->city, city);
    strcpy(new_billionaire->country, country);
    strcpy(new_billionaire->activity, activity);
    strcpy(new_billionaire->origin_country, origin_country);
    strcpy(new_billionaire->gender, gender);
    new_billionaire->birthdate = birthdate;
    new_billionaire->birthtime = birthtime;
    new_billionaire->net_worth = net_worth;
    strcpy(new_billionaire->account_number, account_number);

    //Initialize the new record's pointers to point to itself (circular list)
    new_billionaire->next = new_billionaire;
    new_billionaire->prev = new_billionaire;

    return new_billionaire; //Return the address of the new record
}

//Function to print billionaire details with proper alignment
void print_billionaire(billionaire *b) {
    if (b != NULL) {
        //Print details with formatted alignment
        printf("%-20s %-15s %-15s %-30s %-15s %-10s %02d/%02d/%04d %02d:%02d %-10.2f %s\n",
            b->name, b->city, b->country, b->activity, b->origin_country, b->gender,
            b->birthdate.day, b->birthdate.month, b->birthdate.year,
            b->birthtime.hour, b->birthtime.minutes, b->net_worth, b->account_number);
    } else {
        //Message when there is no data to display
        printf("No data to display.\n");
    }
}

//Function to insert a new billionaire at the end of the list
void insert_billionaire(billionaire **head, billionaire *new_billionaire) {
    if (*head == NULL) {
        //If the list is empty, the new record becomes the head of the list
        *head = new_billionaire;
    } else {
        //Insert at the end of the list
        billionaire *last = (*head)->prev;
        last->next = new_billionaire;
        new_billionaire->prev = last;
        new_billionaire->next = *head;
        (*head)->prev = new_billionaire;
    }
}

//Function to free memory of the list
void free_list(billionaire *head) {
    if (head == NULL) return; //If the list is empty, no need to free
    billionaire *current = head;
    do {
        billionaire *next = current->next;
        free(current); //Free the current record
        current = next;
    } while (current != head); //Continue until we return to the head of the list
}

//Function to capitalize the first letter and lowercase the rest
void capitalize_name(char *name) {
    int len = strlen(name);
    int capitalize = 1; //Flag for capitalization
    int i;
    for (i = 0; i < len; i++) {
        if (isspace(name[i])) {
            capitalize = 1; // After a space, the next letter should be capitalized
        } else if (capitalize) {
            name[i] = toupper(name[i]); //Convert to uppercase
            capitalize = 0;
        } else {
            name[i] = tolower(name[i]); //Convert to lowercase
        }
    }
}

//Function to create a valid account number
void generate_account_number(char *account_number) {
    int digits[10];
    int sum = 0;
    int i;  //Counter for the for loop

    //Generate the first 10 digits randomly
    for (i = 0; i < 10; i++) {
        digits[i] = rand() % 10;
        //Double every second digit from the right (including the last digit)
        int temp = (i % 2 == 1) ? digits[i] * 2 : digits[i];
        //If doubling gives a number greater than 9, add the digits of the result
        if (temp > 9) {
            temp = (temp / 10) + (temp % 10);
        }
        sum += temp;
    }

    //Calculate the check digit
    int check_digit = (sum * 9) % 10;

    //Format the account number
    snprintf(account_number, 14, "%05d-%05d-%1d", rand() % 100000, rand() % 100000, check_digit);
}

//Function to add a new billionaire from the command input
void add_billionaire_from_command(billionaire **head, char *command) {
    char *name = strtok(command, ";");
    char *city_country = strtok(NULL, ";");
    char *activity = strtok(NULL, ";");
    char *origin_country = strtok(NULL, ";");
    char *gender = strtok(NULL, ";");
    char *birth_datetime = strtok(NULL, ";");
    char *net_worth_str = strtok(NULL, ";");

    //Check the validity of input data
    if (!name || !city_country || !activity || !origin_country || !gender || !birth_datetime || !net_worth_str) {
        printf("Invalid input format.\n");
        return;
    }

    capitalize_name(name);

    char *city = strtok(city_country, "/");
    char *country = strtok(NULL, "/");
    if (!city) city = "Unknown";
    if (!country) country = "Unknown";
    if (strcmp(city, "?") == 0) city = "Unknown";
    if (strcmp(country, "?") == 0) country = "Unknown";

    //Convert the net worth from string to double and check if it's negative
    double net_worth = atof(net_worth_str);
    if (net_worth < 0) {
        printf("Net worth cannot be negative.\n");
        return;
    }

    //Read the birthdate and birthtime from the string
    date birthdate;
    btime birthtime;
    if (sscanf(birth_datetime, "%d/%d/%d %d:%d", &birthdate.day, &birthdate.month, &birthdate.year, &birthtime.hour, &birthtime.minutes) != 5) {
        birthdate.day = 1;
        birthdate.month = 1;
        birthdate.year = 1900;
        birthtime.hour = 0;
        birthtime.minutes = 0;
    }

    //Create a valid account number
    char account_number[14];
    generate_account_number(account_number);

    //Create a new billionaire and add them to the list
    billionaire *new_billionaire = create_billionaire(name, city, country, activity, origin_country, gender, birthdate, birthtime, net_worth, account_number);
    insert_billionaire(head, new_billionaire);
    //Print the details of the new billionaire added
    print_billionaire(new_billionaire);
}

//Function to print the total net worth of billionaires
void print_sum(billionaire *head) {
    double total_net_worth = 0.0;
    if (head != NULL) {
        billionaire *current = head;
        do {
            total_net_worth += current->net_worth; //Add each billionaire's net worth to the total
            current = current->next;
        } while (current != head); //Continue until we return to the head of the list
    }
    //Print the sum of net worth
    printf("Total Net Worth: %.2f billion USD\n", total_net_worth);
}

//Function to find a substring case-insensitively
char *substring_finder(const char *haystack, const char *needle) {
    if (!*needle) {
        return (char *)haystack; //Return the haystack if the needle is empty
    }
    for (; *haystack; ++haystack) {
        if (toupper((unsigned char)*haystack) == toupper((unsigned char)*needle)) {
            const char *h, *n;
            for (h = haystack, n = needle; *h && *n; ++h, ++n) {
                if (toupper((unsigned char)*h) != toupper((unsigned char)*n)) {
                    break; //Break if there is no match
                }
            }
            if (!*n) {
                return (char *)haystack; //Return the starting point if the needle is found
            }
        }
    }
    return NULL; //Return NULL if the needle is not found
}

//Function to print billionaires matching the search criteria
void find_billionaires(billionaire *head, const char *query, char field) {
    if (head == NULL) {
        printf("No billionaires in the list.\n");
        return; //Exit if the list is empty
    }

    billionaire *current = head;
    do {
        char *field_value;
        switch (field) {
            case 'N':
                field_value = current->name;
                break;
            case 'C':
                //Check both city and country
                if (substring_finder(current->city, query) || substring_finder(current->country, query)) {
                    print_billionaire(current);
                }
                current = current->next;
                continue;
            case 'S':
                field_value = current->activity;
                break;
            default:
                printf("Invalid field specified.\n");
                return; //Exit if the field is invalid
        }
        if (strcmp(query, "*") == 0 || substring_finder(field_value, query)) {
            print_billionaire(current); //Print if the query is "*" or if it matches the field_value
        }
        current = current->next;
    } while (current != head); //Continue until we return to the head of the list
}

//Function to print billionaires based on net worth or birthdate criteria
void print_by_criteria(billionaire *head, const char *criteria, char op) {
    if (head == NULL) {
        printf("No billionaires in the list.\n");
        return; //Exit if the list is empty
    }

    double net_worth_criteria = 0.0;
    date date_criteria = {0};
    btime time_criteria = {0};

    //Check if the criteria is a date
    if (strchr(criteria, '/') != NULL) {
        sscanf(criteria, "%d/%d/%d", &date_criteria.day, &date_criteria.month, &date_criteria.year);
    //Check if the criteria is a time
    } else if (strchr(criteria, ':') != NULL) {
        sscanf(criteria, "%d:%d", &time_criteria.hour, &time_criteria.minutes);
    //If the criteria is neither date nor time, consider it as net worth
    } else {
        net_worth_criteria = atof(criteria);
    }

    billionaire *current = head;
    do {
        int match = 0;
        //Check if the criteria is net worth
        if (net_worth_criteria > 0) {
            if ((op == '+' && current->net_worth > net_worth_criteria) ||
                (op == '-' && current->net_worth < net_worth_criteria) ||
                (op == '=' && current->net_worth == net_worth_criteria)) {
                match = 1;
            }
        //Check if the criteria is date
        } else if (date_criteria.year > 0) {
            int cmp = (current->birthdate.year * 10000 + current->birthdate.month * 100 + current->birthdate.day) -
                      (date_criteria.year * 10000 + date_criteria.month * 100 + date_criteria.day);
            if ((op == '+' && cmp > 0) || (op == '-' && cmp < 0) || (op == '=' && cmp == 0)) {
                match = 1;
            }
        //Check if the criteria is time
        } else if (time_criteria.hour >= 0) {
            int cmp = (current->birthtime.hour * 100 + current->birthtime.minutes) -
                      (time_criteria.hour * 100 + time_criteria.minutes);
            if ((op == '+' && cmp > 0) || (op == '-' && cmp < 0) || (op == '=' && cmp == 0)) {
                match = 1;
            }
        }
        //Print if there is a match with the criteria
        if (match) {
            print_billionaire(current);
        }
        current = current->next;
    } while (current != head); //Continue until we return to the head of the list
}

//Function to delete the most recent entry
void delete_newest(billionaire **head) {
    if (*head == NULL) {
        printf("The list is empty.\n");
        return; //Exit if the list is empty
    }
    billionaire *last = (*head)->prev;

    //If there is only one item in the list
    if (*head == last) {
        free(*head);
        *head = NULL;
    } else {
        last->prev->next = *head;
        (*head)->prev = last->prev;
        free(last);
    }
    printf("Deleted the newest billionaire.\n");
}

//Function to delete the oldest entry
void delete_oldest(billionaire **head) {
    if (*head == NULL) {
        printf("The list is empty.\n");
        return; //Exit if the list is empty
    }
    billionaire *oldest = *head;

    //If there is only one item in the list
    if (oldest->next == oldest) {
        free(oldest);
        *head = NULL;
    } else {
        oldest->prev->next = oldest->next;
        oldest->next->prev = oldest->prev;
        *head = oldest->next;
        free(oldest);
    }
    printf("Deleted the oldest billionaire.\n");
}

//Function to save data to a file
void save_to_file(billionaire *head, const char *filename) {
    if (head == NULL) {
        printf("No data to save.\n");
        return; //Exit if the list is empty
    }

    FILE *file = fopen(filename, "w"); //Open the file for writing
    if (file == NULL) {
        printf("Failed to open file %s for writing.\n", filename);
        return; //Exit if opening the file fails
    }

    billionaire *current = head;
    do {
        //Write the billionaire's data to the file
        fprintf(file, "%-20s;%-15s;%-15s;%-30s;%-15s;%-10s;%02d/%02d/%04d %02d:%02d;%-10.2f;%s\n",
                current->name, current->city, current->country, current->activity, current->origin_country, current->gender,
                current->birthdate.day, current->birthdate.month, current->birthdate.year,
                current->birthtime.hour, current->birthtime.minutes, current->net_worth, current->account_number);
        current = current->next;
    } while (current != head); //Continue until we return to the head of the list

    fclose(file); //Close the file
    printf("Data saved to file %s successfully.\n", filename);
}

//Function to load data from a file
void load_from_file(billionaire **head, const char *filename, int count) {
    FILE *file = fopen(filename, "r"); //Open the file for reading
    if (file == NULL) {
        printf("Failed to open file %s.\n", filename);
        return; //Exit if opening the file fails
    }

    char line[512];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) && line_count < count) {
        //Split the line into fields
        char *name = strtok(line, ";");
        char *city_country = strtok(NULL, ";");
        char *activity = strtok(NULL, ";");
        char *origin_country = strtok(NULL, ";");
        char *gender = strtok(NULL, ";");
        char *birth_datetime = strtok(NULL, ";");
        char *net_worth_str = strtok(NULL, ";");

        //Check the validity of the data
        if (!name || !city_country || !activity || !origin_country || !gender || !birth_datetime || !net_worth_str) {
            printf("Invalid data format in line %d.\n", line_count + 1);
            break;
        }

        capitalize_name(name); //Capitalize the name

        //Split the city and country
        char *city = strtok(city_country, "/");
        char *country = strtok(NULL, "/");
        if (!city) city = "Unknown";
        if (!country) country = "Unknown";
        if (strcmp(city, "?") == 0) city = "Unknown";
        if (strcmp(country, "?") == 0) country = "Unknown";

        //Convert the net worth from string to double and check if it's negative
        double net_worth = atof(net_worth_str);
        if (net_worth < 0) {
            printf("Invalid net worth in line %d.\n", line_count + 1);
            break;
        }

        //Read the birthdate and birthtime from the string
        date birthdate;
        btime birthtime;
        if (sscanf(birth_datetime, "%d/%d/%d %d:%d", &birthdate.day, &birthdate.month, &birthdate.year, &birthtime.hour, &birthtime.minutes) != 5) {
            birthdate.day = 1;
            birthdate.month = 1;
            birthdate.year = 1900;
            birthtime.hour = 0;
            birthtime.minutes = 0;
        }

        //Create a valid account number
        char account_number[14];
        generate_account_number(account_number);

        //Create a new billionaire and add them to the list
        billionaire *new_billionaire = create_billionaire(name, city, country, activity, origin_country, gender, birthdate, birthtime, net_worth, account_number);
        insert_billionaire(head, new_billionaire);

        line_count++;
    }

    fclose(file); //Close the file
    printf("Loaded %d billionaires from file %s.\n", line_count, filename);
}

//Function to process commands
void process_command(billionaire **head, char *command) {
    if (strncmp(command, "sum", 3) == 0) {
        print_sum(*head); //Print the total net worth of billionaires
    } else if (strncmp(command, "findN ", 6) == 0) {
        find_billionaires(*head, command + 6, 'N'); //Search by name
    } else if (strncmp(command, "findC ", 6) == 0) {
        find_billionaires(*head, command + 6, 'C'); //Search by city or country
    } else if (strncmp(command, "findS ", 6) == 0) {
        find_billionaires(*head, command + 6, 'S'); //Search by activity
    } else if (strncmp(command, "print+", 6) == 0) {
        print_by_criteria(*head, command + 6, '+'); //Print with net worth or birthdate greater than the criteria
    } else if (strncmp(command, "print-", 6) == 0) {
        print_by_criteria(*head, command + 6, '-'); //Print with net worth or birthdate less than the criteria
    } else if (strncmp(command, "print=", 6) == 0) {
        print_by_criteria(*head, command + 6, '='); //Print with net worth or birthdate equal to the criteria
    } else if (strncmp(command, "deleteN", 7) == 0) {
        delete_newest(head);                        //Delete the most recent entry
    } else if (strncmp(command, "deleteO", 7) == 0) {
        delete_oldest(head);                       //Delete the oldest entry
    } else if (strncmp(command, "save ", 5) == 0) {
        save_to_file(*head, command + 5);           //Save data to a file
    } else if (strncmp(command, "load ", 5) == 0) {
        int count;
        char filename[256];
        if (sscanf(command + 5, "%d %s", &count, filename) == 2) {
            load_from_file(head, filename, count); //Load data from a file
        } else {
            printf("Invalid load command format.\n");
        }
    } else {
        printf("Unknown command: %s\n", command); //Message for unknown command
    }
}

int main() {
    billionaire *head = NULL; //Initialize the head of the list
    char command[256]; //Variable to store the command

    srand(time(NULL)); //Initialize the seed for rand()

    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin); //Read the command from the user
        command[strcspn(command, "\n")] = '\0'; //Remove the newline character to avoid errors in functions

        if (strcmp(command, "exit") == 0) {
            break; //Exit the program if the command is "exit"
        } else if (strncmp(command, "newbil ", 7) == 0) {
            add_billionaire_from_command(&head, command + 7); //Add a new billionaire
        } else {
            process_command(&head, command); //Process other commands
        }
    }

    free_list(head); //Free the memory of the list before exiting

    return 0; //End of program
}
