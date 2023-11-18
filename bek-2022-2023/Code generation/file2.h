#ifndef __file2_h__
#define __file2_h__
#if COMPILING_DLL
#define DLL __declspec(dllexport)
#elif IMPORTING_FROM_DLL
#define DLL __declspec(dllimport)
#else
#define DLL
#endif

#ifdef __cplusplus
extern "C" {
#endif
int DLL File2_Funkce1(int a, int b);
int DLL __stdcall File2_Funkce2(int a, int b);
#ifdef __cplusplus
}
#endif
#endif



