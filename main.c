#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef struct
{
    struct dirent* file;
    char pathName[260];
    struct fileInfo* next;
}fileInfo;

fileInfo* head = NULL;
fileInfo* tail = NULL;

void stringJoin(char* dest, const char* src1, const char* src2)
{
    int i = 0;
    while(src1[i] != 0)
    {
        dest[i] = src1[i];
        ++i;
    }
    dest[i++] = '/';
    int j = 0;
    while(src2[j] != 0)
    {
        dest[i] = src2[j];
        ++i;
        ++j;
    }
    dest[i] = 0;
}

int filesEqual(const char* path1, const char* path2)
{
    FILE* file1 = fopen(path1, "rb");
    FILE* file2 = fopen(path2, "rb");
    while( !feof(file1) && !feof(file2) )
    {
        int byte1 = fgetc(file1);
        int byte2 = fgetc(file2);
        if(byte1 != byte2)
        {
            return 0;
        }//if
    }
    return feof(file1) && feof(file2);
}

void addToList(struct dirent* file, const char* pathName)
{
    fileInfo* newFile = malloc(sizeof(fileInfo));
    newFile -> file = file;
    strcpy(newFile -> pathName, pathName);
    if(!head)
        head = newFile;
    else
        tail->next = (struct fileInfo*) newFile;
    newFile->next = NULL;
    tail = newFile;
}

void listFiles(const char* pathName)
{
    struct dirent* file;
    DIR* path;
    if( path = opendir(pathName) )
    {
        while(file = readdir(path))
        {
            if(
            ( strlen(file->d_name) == 1 && file->d_name[0] == '.' ) ||
            ( strlen(file->d_name) == 2 && file->d_name[0] == '.' && file->d_name[1]=='.')//DZIWNE POROWNYWANIE, NORMALNE NIE DZIALA(???)
            )
                continue;

            if( (int) file->d_type == 4)
            {
                char newPath[strlen(pathName) + 1 + strlen(file->d_name) + 1];
                stringJoin(newPath, pathName, file->d_name);
                listFiles(newPath);
            }
            else
            {
                char filePath[strlen(pathName) + 1 + strlen(file->d_name) + 1];
                stringJoin(filePath, pathName, file->d_name);
                addToList(file, filePath);
                puts(file->d_name);
            }
        }

    }//otwieranie
    else //blad
    {
        printf("Blad otwarcia sciezki %s\n", pathName);
        return;
    }//blad
}

void dirShow(const char* pathName) //DEBUG
{
    struct dirent* file;
    DIR* path;
    if( path = opendir(pathName) )
    {
        while( file = readdir(path) )
        {
            printf("%s\t%i\n", file->d_name, (int) file->d_type);
        }//while
        closedir(path);
    }//otwieranie
    else
    {
        printf("Blad otwarcia sciezki %s\n", pathName);
    }
}

void listShow()//DEBUG
{
    fileInfo* i = head;
    while(i)
    {
        puts(i->pathName);
        i = i->next;
    }
}

int main(int argc, char* argv[])
{
    listFiles(argv[1]);
    printf("\n");
    listShow();
}
