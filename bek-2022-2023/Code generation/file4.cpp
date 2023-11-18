#include <windows.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	HMODULE hModule = NULL;
	int (*pfnFile1_Funkce1)(int, int) = NULL;
	int (__stdcall *pfnFile1_Funkce2)(int,int) = NULL;
	int (*pfnFile2_Funkce1)(int, int) = NULL;
	int (__stdcall *pfnFile2_Funkce2)(int, int) = NULL;

	hModule = LoadLibrary(TEXT("new_dll.dll"));
	if (hModule) {
		pfnFile1_Funkce1 = reinterpret_cast<int (*)(int, int)>(GetProcAddress(hModule, "File1_Funkce1"));
		pfnFile1_Funkce2 = reinterpret_cast<int (__stdcall *)(int, int)>(GetProcAddress(hModule, "File1_Funkce2"));
		pfnFile2_Funkce1 = reinterpret_cast<int (*)(int, int)>(GetProcAddress(hModule, "File2_Funkce1"));
		pfnFile2_Funkce2 = reinterpret_cast<int (__stdcall *)(int, int)>(GetProcAddress(hModule, "File2_Funkce2"));

		if (pfnFile1_Funkce1 != NULL) 
			printf("F1_F1: %d.\n", pfnFile1_Funkce1(1, 2));
		else 
			printf("F1_F1 not found: %d.\n", GetLastError());

		if (pfnFile1_Funkce2 != NULL)
			printf("F1_F2: %d.\n", pfnFile1_Funkce2(1, 2));
		else
			printf("F1_F2 not found: %d.\n", GetLastError());
		
		if (pfnFile2_Funkce1 != NULL)
			printf("F2_F1: %d.\n", pfnFile2_Funkce1(1, 2));
		else
			printf("F2_F1 not found: %d.\n", GetLastError());
		
		if (pfnFile2_Funkce2 != NULL)
			printf("F2_F2: %d.\n", pfnFile2_Funkce2(1, 2));
		else
			printf("F2_F2 not found: %d.\n", GetLastError());

		FreeLibrary(hModule);
		hModule = NULL;
	} else {
		printf("DLL not found.\n");
	}
	return 0;
}