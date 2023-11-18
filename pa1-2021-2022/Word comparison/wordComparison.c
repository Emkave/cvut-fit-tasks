#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */

char * BegginningFinder(char * string) {
    char * pointer;
    for (pointer = string; * pointer == ' '; pointer++) {if (* pointer == '\0') {return pointer;}}
    return pointer;
}

int AllWordSearching(char * string1, char * string2) {
    char * point11, * point12, * point2; char character;
    point11 = string1;

    while (* point11 != '\0') {
        point11 = BegginningFinder(point11);

        if (* point11 == '\0') {return 1;}
        point12 = strchr(point11, ' ');
        point11--; point12++;
        character = * point12; * point12 = '\0';
        point2 = strstr(string2, point11);

        if (point2 == NULL) {return 0;}
        * point12 = character;
        point11 = point12;
    }
    return 1;
}

char * StringFilter(char * string) {
    while (*string != '\0') {
        if (!isalpha(*string)) {
            *string = ' ';
        } else {
            *string = toupper(*string);
        }
        string++;
    }
    return string;
}

int sameWords( const char * a, const char * b ) {
    int lenA = strlen(a);
    char * PA = (char*)malloc(sizeof(char)*(lenA + 3));  *PA = ' ';
    strcpy(PA+1, a); StringFilter(PA+1); strcat(PA, " ");
    
    int lenB = strlen(b);
    char * PB = (char*)malloc(sizeof(char)*(lenB + 3)); *PB = ' ';
    strcpy(PB+1, b); StringFilter(PB+1); strcat(PB, " ");

    int result = 1;

    if (AllWordSearching(PA, PB) == 0) {result = 0;}
    else {if (AllWordSearching(PB, PA) == 0) {result = 0;}}

    free(PA); free(PB);
    return result;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] ) {
    assert(sameWords("Hello students.", "HELLO studEnts!") == 1); 
    assert(sameWords(" He said 'hello!'", "'Hello.' he   said." ) == 1);
    assert(sameWords("He said he would do it.", "IT said: 'He would do it.'" ) == 1);
    assert(sameWords("one two three", "one two five") == 0);
    assert(sameWords("hgiuhrigu42 fj429qmcmr'33#)R:#MDQPK fkwo3f", "hgiuhrigu fj qmcmr R MDQPK fkwo f") == 1);
    assert(sameWords("   a a &a&a@a(a a a a a31           a               a", "432684825897325891409351a                               a'''''' a a a a a a a !@#$%^&*():><?|}{") == 1);
    return 0;
}
#endif /* __PROGTEST__ */
