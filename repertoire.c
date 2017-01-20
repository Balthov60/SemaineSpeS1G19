#include"repertoire.h"

TCHAR* char2tchar(char* source){
	TCHAR* tch;
	tch=malloc(sizeof(TCHAR)*(strlen(source)+1));
	mbstowcs(tch,source,strlen(source)+1);
	return tch;
}

char* tchar2char(TCHAR* source){
	char* ch;
	ch=malloc(sizeof(char)*(wcslen(source)+1));
	wcstombs(ch,source,wcslen(source)+1);
	return ch;
}

void add(char***t,int*n,char*ch){
	char**tmp;
	int i;
	tmp=malloc(sizeof(char*)*((*n)+1));
	if(t!=NULL)
		for(i=0;i<*n;tmp[i]=(*t)[i],i++);
	tmp[*n]=malloc(sizeof(char)*(strlen(ch)+1));
	strcpy(tmp[*n],ch);
	(*n)++;
	free(*t);
	*t=tmp;
}

void destroyTab(char**t, int size){
	int i;
	for(i=0;i<size;free(t[i]),i++);
	free(t);
}

char** getFiles(char* directory, char* filter, int* size)
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	TCHAR* tDirectory;
	char**tab=NULL;
	char*ch;
	char*tmp;
	int pos;
	char*extension;

	(*size)=0;
	tDirectory = char2tchar(directory);

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.
	StringCchLength(tDirectory, MAX_PATH, &length_of_arg);
	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}
	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopy(szDir, MAX_PATH, tDirectory);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) 
	{
		return NULL;
	} 
	// Liste des dossiers
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			tmp=tchar2char(ffd.cFileName);
			ch=malloc(sizeof(char)*(strlen(tmp)+3));
			ch[0]='[';
			strcpy(ch+1,tmp);
			ch[strlen(tmp)+1]=']';
			ch[strlen(tmp)+2]='\0';
			free(tmp);
			add(&tab,size,ch);
			free(ch);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	//fichiers
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) 
	{
		return NULL;
	} 
	// Liste des fichiers
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			ch=tchar2char(ffd.cFileName);
			if(strcmp(filter,"*")==0)
				add(&tab,size,ch);
			else{
				//recherche de l'extension
				pos=strlen(ch);
				while(pos>=0 && ch[pos]!='.')
					pos--;
				if(pos>0){
					extension=malloc(sizeof(char)*(strlen(ch)-pos+1));
					strcpy(extension,ch+pos+1);
					if(strcmp(extension,filter)==0)
						add(&tab,size,ch);
					free(extension);
				}
			}
			free(ch);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	free(tDirectory);
	FindClose(hFind);
	return tab;
}