#include "mafunk.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

off_t   FileSizeStat    (int fileDiscript);
int     LineCounter     (char* string);
int     Comparator      (const void* str1, const void* str2);
int     WriteSortFile   (char** stringBuf, int lineNumber, const char* fileName);

int main(int arc, char* argv[]) 
{
    if (arc != 2) {
        printf("main arg error");
        return EXIT_FAILURE;
    }
    
    int fd = open(argv[1], O_RDONLY);
    CheckComError(fd, "open");

    off_t fsize = FileSizeStat(fd);
        printf("Fsize = %li\n", fsize);
    
    char* strFile = (char*)calloc(sizeof(char), (fsize + 2)*sizeof(char));
    int readCheck = read(fd, strFile, fsize);
    close(fd);
    CheckComError(readCheck, "read");
        printf("%s", strFile);
    
    int lineNumber = LineCounter(strFile);
        printf("lineNumber = %i\n", lineNumber);

    char** text = (char**)calloc(sizeof(char*), (lineNumber + 1)*sizeof(char*));
    int k = 1, i = 0;
    text[0] = strFile;
    while (strFile[i] != '\0') {
        if (strFile[i] == '\n') {
            text[k] = strFile + i + 1;
            k++;
            strFile[i] = '\0';
        }
        i++;
    }
    text[lineNumber] = NULL;

    qsort(text, lineNumber, sizeof(text[-1]), Comparator);

    for (i = 0; i < lineNumber; i++) {
        printf("%s\n", text[i]);
    }

    int writeCheck = WriteSortFile(text, lineNumber, "RusLifeEnsyclopedia.txt");

    free(strFile);
    strFile = NULL;
    free(text);
    text = NULL;
    return 0;
}

int WriteSortFile (char** stringBuf, int lineNumber, const char* fileName)
{
    int fd = open(fileName, O_CREAT | O_WRONLY, 0666);
    if (fd < 0) {
        printf("In WriteSortFile %s open error\n", fileName);
        return EXIT_FAILURE;
    }
    int i = 0;
    for (i = 0; i < lineNumber; i++) {
        printf("%ld\n", strlen(stringBuf[i]));
        write(fd, stringBuf[i], strlen(stringBuf[i]));
        write(fd, "\n", 1);
    }
    close(fd);
    return 0;
}

int Comparator (const void* str1, const void* str2)
{
    return strcmp(*(const char**)str1, *(const char**)str2);    
}

off_t FileSizeStat (int fileDiscript)
{
    struct stat stat = {};
    size_t statCheck = fstat(fileDiscript, &stat);
    if (statCheck < 0) {
        printf("In FileSizeStat fstat\n");
        return EXIT_FAILURE;
    }
    return stat.st_size;      
}

int LineCounter (char* string) 
{
    if (string == NULL) { 
        return -1;
    }
    int lineNumber = 0, i = 0;
    while (string[i] != '\0') {
        if (string[i] == '\n') {
            lineNumber++;
        }
        i++;
    }
    if (string[i - 1] != '\n') {
        lineNumber++;
    }
    return lineNumber;
}
