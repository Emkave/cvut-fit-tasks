#include "file1.h"

int File1_Funkce1(int a, int b) {
	return a+b;
}

int __stdcall File1_Funkce2(int a, int b) {
	return a+b;
}

int __stdcall Checkpoint(void) {
	return 1337;
}
