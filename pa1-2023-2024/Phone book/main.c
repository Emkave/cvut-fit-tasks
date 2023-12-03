#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct list {       // This structure is used for storing data in a linked list. The values are self-explanatory
    char * name;
    char * t9_name;
    char * phone_number;

    struct list * next;
    struct list * tail;
} List;


/// This function cleans the linked list iteratively
void clear_list(List * __restrict ptr) {
    List * temp = ptr;
    while (temp) {
        temp = ptr->next;
        free(ptr->name);
        free(ptr->t9_name);
        free(ptr->phone_number);
        free(ptr);                  // Clear self
        ptr = temp;
    }
}


/// This function is used to emplace values into the linked list
bool emplace_trie(List ** list, char * phone_number, char * name, char * t9_name) {
    if (!*list) {                               // Check if the linked list is empty
        *list = (List*)calloc(1, sizeof(List));
        (*list)->next = NULL;
        (*list)->tail = *list;
        (*list)->name = strdup(name);
        (*list)->phone_number = strdup(phone_number);
        (*list)->t9_name = t9_name;
    } else {
        List * new__ = (List*)calloc(1, sizeof(List));  // Emplace at the tail if it is not empty
        new__->next = NULL;
        new__->name = strdup(name);                     // Copy strings
        new__->phone_number = strdup(phone_number);
        new__->t9_name = t9_name;
        (*list)->tail->next = new__;
        (*list)->tail = (*list)->tail->next;
    }
    return true;
}

/// This function is used to extract tokens for the future computation and data manipulation
/// by iterative analysis
bool extract_tokens(const size_t strlen, char * str, char ** name, char ** phone_number, char * oper_) {
    if (str[0] == ' ' || (str[0] != '?' && str[0] != '+') || str[1] != ' ' || !isdigit(str[2]) || str[strlen-1] == ' ') {
        return false;
    }   // Here I check for the valid beginning and ending

    *oper_ = str[0];                // Assign the operator

    size_t num_start = 2;           // using these variables we "highlight" the desired area and copy it to the pointer
    size_t num_end = 2;
    char * iter = str + 2;

    while (*iter != '\0') {         // Iterate over the phone number (or prefix) and check if it is a valid
        if (*iter == ' ') {
            break;
        }

        if (!isdigit(*iter)) {
            return false;
        }
        iter++;
        num_end++;
    }

    *phone_number = strndup(str+2, num_end - num_start);        // After the successfully checking - copy to the pointer

    if (*oper_ == '+') {
        if (num_end - num_start > 20 || *(iter + 1) == ' ' || *(iter+1) == '\0') {  // In case of + operator, we check if it has been presented at all
            return false;
        }

        iter++; // Move iterator to the beginning of the name

        size_t nam_start = num_end - num_start + 3;     // Set starting position for the highlight
        int spaces = 0;                                 // Used to detect at least one space in the name

        while (*iter != '\0') {                         // This cycle iterates over the name and checks its validity
            if (*iter == ' ') {
                spaces++;

                if (spaces > 1) {
                    return false;
                }
            } else if (!isalpha(*iter)) {
                return false;
            }
            iter++;
        }

        *name = strdup(str + nam_start);
    } else {                                            // In case of ? operator we check if the prefix has been presented at all.
        if (strlen - 2 > num_end - num_start) {
            return false;
        }
    }

    return true;
}

/// Function used for dynamic memory allocation and input saving.
char * cin(size_t * len) {
    char * line = (char*)malloc(sizeof(char)*10);   // Set the beginning buffer
    char * tmp;
    size_t capacity = 10, index = 0;
    int ch = EOF;

    while (ch) {                                    // Iterate over every character from standard input
        ch = getc(stdin);

        if (ch == EOF || ch == '\n') {              // Detect the EOF or Enter
            ch = 0;
        }

        if (capacity <= index) {
            capacity *= 2;
            tmp = (char*)realloc(line, capacity);
            line = tmp;
        }

        line[index++] = (char)ch;
    }
    *len = index - 1;
    line[index-1] = '\0';

    return line;
}

/// This function is used to return T9 version of a non-numeric string.
char * get_t9(char * str) {
    size_t len = strlen(str);
    char * t9_name = (char *)malloc(sizeof(char)*len + 1);
    int j = 0;
    for (char * ptr = str; *ptr != '\0'; ptr++) {       // Iterating over each letter and map it to a number according to T9 table.
        switch (*ptr) {
            case 'A':
            case 'a':
            case 'B':
            case 'b':
            case 'C':
            case 'c':
                t9_name[j] = '2';
                break;

            case 'D':
            case 'd':
            case 'E':
            case 'e':
            case 'F':
            case 'f':
                t9_name[j] = '3';
                break;

            case 'G':
            case 'g':
            case 'H':
            case 'h':
            case 'I':
            case 'i':
                t9_name[j] = '4';
                break;

            case 'J':
            case 'j':
            case 'K':
            case 'k':
            case 'L':
            case 'l':
                t9_name[j] = '5';
                break;

            case 'M':
            case 'm':
            case 'N':
            case 'n':
            case 'O':
            case 'o':
                t9_name[j] = '6';
                break;


            case 'P':
            case 'p':
            case 'Q':
            case 'q':
            case 'R':
            case 'r':
            case 'S':
            case 's':
                t9_name[j] = '7';
                break;

            case 'T':
            case 't':
            case 'U':
            case 'u':
            case 'V':
            case 'v':
                t9_name[j] = '8';
                break;


            case 'W':
            case 'w':
            case 'X':
            case 'x':
            case 'Y':
            case 'y':
            case 'Z':
            case 'z':
                t9_name[j] = '9';
                break;

            case ' ':
                t9_name[j] = '1';
                break;

        }
        j++;
    }

    t9_name[j] = '\0';          // Add end of string

    return t9_name;
}

/// This function is used to check if a string starts with a prefix.
bool starts_with(const char * pre, const char * str) {
    if (strlen(pre) > strlen(str)) {        // Check if the prefix is longer than the target string
        return false;
    }

    return !strncmp(pre, str, strlen(pre)); // Checks if the beginning bytes (lenght of prefix) are equal
}

/// Function used to check if the linked list contains the chunk of memory with the phone and name data
bool contains(List * const __restrict list, const char * phone_number, const char * name) {
    for (List * ptr = list; ptr; ptr = ptr->next) {
        if (strcmp(ptr->name, name) == 0 && strcmp(ptr->phone_number, phone_number) == 0) {
            return true;
        }
    }
    return false;
}

/// Function used to search and print the names and phone numbers that fall under the prefix
void search(List * __restrict list, const char * prefix) {
    size_t count = 0;

    for (List * ptr = list; ptr; ptr = ptr->next) {         // Searching for valid
        if (starts_with(prefix, ptr->phone_number) || starts_with(prefix, ptr->t9_name)) {
            count++;
        }
    }

    for (List * ptr = list; count <= 10 && ptr; ptr = ptr->next) {  // this cycle avoids the usage of recursion, preventing stack overflow
        if (starts_with(prefix, ptr->phone_number) || starts_with(prefix, ptr->t9_name)) {
            printf("%s %s\n", ptr->phone_number, ptr->name);
        }
    }

    printf("Total: %zu\n", count);
}


int main(void) {
    List * list = NULL;
    char * name = NULL;
    char * str = NULL;
    char * phone_number = NULL;
    char oper_;
    size_t len = 0;


    while (1) {                 // The main cycle
        str = cin(&len);        // same as std::cin >> str;

        if (feof(stdin)) {      // Detect the EOF
            free(str);
            str = NULL;
            goto end;           // Same as asm ("j end");
        }

        if (!extract_tokens(len, str, &name, &phone_number, &oper_)) { // Extracting the tokens and saving them to the variables
            free(str);                                  // In case of any invalid input - we go here, free the str, and continue the cycle again.
            str = NULL;
            printf("Invalid input.\n");
            continue;
        }

        free(str);
        str = NULL;

        switch (oper_) {
            case '+': {
                if (contains(list, phone_number, name)) {       // Check if the list contains the data
                    printf("Duplicate contact.\n");
                } else {
                    emplace_trie(&list, phone_number, name, get_t9(name));      // Emplace if not
                    printf("OK\n");
                }

                free(name);                     // Release the memory and prepare for the next input
                free(phone_number);
                phone_number = NULL;
                name = NULL;
                break;
            }


            case '?': {                       // Search
                search(list, phone_number);
                free(phone_number);
                phone_number = NULL;
                break;
            }
        }
    }

#define __PRINT_TABLE__ 1
#ifndef __PRINT_TABLE__             // Used to debug and tests
    for (List * ptr = list; ptr; ptr = ptr->next) {
        printf("============\n");
        printf("%p\n", ptr);
        puts(ptr->name);
        puts(ptr->phone_number);
        puts(ptr->t9_name);
    }
#endif


    end:                            // End label designated for end of program

    if (name) {
        free(name);
    }

    if (str) {
        free(str);
    }

    if (phone_number) {
        free(phone_number);
    }

    clear_list(list);

    return 0;
}