#include <stdio.h>
#include <exception>
#include "file1.h"

int main(int argc, char ** argv) {
	int a = File1_Funkce1(1, 2);
	int b = File1_Funkce2(1, 3);

	printf("%d\n", a);
	printf("%d\n", b);
	return 0;
}

