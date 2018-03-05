#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#ifndef FILE_NAME_MAX
#define FILE_NAME_MAX 1024
#endif

int processDirectory(int*, char*);

int main(int argc, char* argv[])
{
    char* dirName = argv[1];
    int dirSize;
    
    if(argc != 2){
        puts("Invalid command line argument.");
        exit(0);
    }
    
    if (processDirectory(&dirSize, dirName) == -1)
    {
        printf("Error reading folder size!");
    }
    else
    {
        printf("%d\n", dirSize);
    }
    
    return 0;
  
}

int processDirectory(int* totalSize, char* dirName)
{
    char filePath[FILE_NAME_MAX];
    struct dirent *dirEntry;
    struct stat st;
    int result;
    DIR *dir;
    int response = EXIT_SUCCESS;
    
    dir = opendir(dirName);
    
    if (dir == NULL)
    {
        perror("Directory PATH");
        exit(0);
    }
    
    
    
    while ((dirEntry = readdir(dir)))
    {
        
        lstat(".", &st);
        (*totalSize) += st.st_size;
        
        if (dirEntry->d_type == DT_DIR)
        {
            
            if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0)
                continue;
            
            sprintf(filePath, "%s/%s", dirName, dirEntry->d_name);
                
            if (processDirectory(totalSize,filePath) == EXIT_FAILURE)
                response = EXIT_FAILURE;
            
        }
        else
        {
            sprintf(filePath, "%s/%s", dirName, dirEntry->d_name);
            
            result = lstat(filePath, &st);
            if (result == -1)
            {
                perror("Directory PATH");
                response = EXIT_FAILURE;
                continue;
            }
            
            (*totalSize) += st.st_size;
            
        }
    }
    
    closedir(dir);
    
    return *totalSize;
}


