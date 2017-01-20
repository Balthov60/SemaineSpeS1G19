#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include<stdlib.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

char** getFiles(char* directory, char* filter, int* size);
TCHAR* char2tchar(char* source);
char* tchar2char(TCHAR* source);
void add(char***t,int*n,char*ch);
void destroyTab(char**t, int size);