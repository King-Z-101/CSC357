/*This program prints out the pathname of the current directory where 
it is executed*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
//#define PATH_MAX 2048


int main(void)
{
    //starting variables
    char buffer[PATH_MAX] = "";

    //cwd info
    char* cwd = ".";
    struct stat prevStat;
    //stat ourselves
    lstat(cwd, &prevStat);

    //printf("cwd: %ld, %ld\n", prevStat.st_dev, prevStat.st_ino);

    //get root stat
    struct stat rootStat;
    lstat("/", &rootStat);
    //printf("ROOT: %ld, %ld\n", rootStat.st_dev, rootStat.st_ino);



    //go up the hierarchy to parent directory
    chdir("..");
    //open our parent
    struct stat parentStat;
    lstat(".", &parentStat);
    //printf("Parent: %ld, %ld\n", parentStat.st_dev, parentStat.st_ino);

    struct dirent *parentEntry;
    DIR *parentDir = opendir(".");
    if (parentDir == NULL) 
    {
        perror("mypwd");
        printf("cannot get current directory.");
        exit(EXIT_FAILURE);
    }
    int pathSize = 0;

    //go through all entries in the current directory and stat them
    while ((parentEntry = readdir(parentDir)) != NULL)
    {
        if (pathSize > PATH_MAX)
        {
            perror("mypwd");
            printf("path too long");
            exit(EXIT_FAILURE);
        }
        lstat(parentEntry->d_name, &parentStat);
        // printf("Previous/CWD: %ld, %ld\n", prevStat.st_dev, 
        //prevStat.st_ino);
        // printf("Parent: %ld, %ld\n", parentStat.st_dev, parentStat.st_ino);
        
        //check if i_nodes and device nums match with prevDir/directory u 
        //come from
        if ((parentStat.st_dev == prevStat.st_dev) && 
        (parentStat.st_ino == prevStat.st_ino))
        {
            char tempPath[PATH_MAX] = "";
            strcpy(tempPath, buffer);
            //free buffer
            strcpy(buffer, "");
            strcat(buffer, "/");
            strcat(buffer, parentEntry->d_name);
            strcat(buffer, tempPath);

            //move up the directory hierarchy
            lstat(".", &prevStat);
            chdir("..");
            parentDir = opendir(".");
            if (parentDir == NULL) 
            {
                perror("mypwd");
                printf("cannot get current directory.");
                exit(EXIT_FAILURE);
            }
            pathSize++;
        }

        //check if you have reached the root
        if ((prevStat.st_dev == rootStat.st_dev) && 
        (prevStat.st_ino == rootStat.st_ino))
        {
            break;
        }

    }
   
    printf("%s\n", buffer);
    return 0; //success
}
