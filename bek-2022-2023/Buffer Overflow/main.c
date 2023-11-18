#include <stdio.h>
#include <stdlib.h>

int main(void) {
/*
    __asm {
		int 3;
	}
    */
    char buffer[80];
    printf("Enter your name: ");
    gets(buffer);
    printf("Hello, %s\n", buffer);
    
    return EXIT_SUCCESS;
}

