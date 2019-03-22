/*
 ============================================================================
 Name        : zabbix_fs.c
 Author      : Marcus Burghardt
 Version     :
 Copyright   : MIT
 Description : Created to support FileSystem operations with Zabbix
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Recursive function to follow all subdirs. It gets the pointer address to permit a global increment.
void listdir(const char *dirpath, long unsigned int *count, long unsigned int *size)
{
	DIR *dir;
    struct dirent *entry;

    // Give up if the dir doesn't exist or is not accessible, due to FS permissions, for instance.
    if (!(dir = opendir(dirpath)))
        return;

    // Start the reading of the DIR. Think like a "ls" here. : )
    while ((entry = readdir(dir)) != NULL) {
    	struct stat file_stats; // Created to store the stat buffer.
    	char fullpath[1024];    // Used to concatenate the file and base dir. Lets help the stat(). : )

    	// I am not interested to get the size of directories. Just files (else).
    	if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            // Prepare the path for the next directory and call the function again.
            snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
            listdir(path, count, size);
        } else {
        	// If you are here, you are a file. : )
            strcpy(fullpath,dirpath);
            strcat(fullpath,"/");
            strcat(fullpath,entry->d_name);
            // stat() needs to receive the absolute path.
            stat(fullpath, &file_stats);
            // Remember. I am working with pointers...
            ++*count;
            *size+=file_stats.st_size;
            //printf("FILE: %s - COUNT: %lu - SIZE: %lu\n",fullpath,*count,file_stats.st_size);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    //if (((argc < 2) || (argc > 3)) || ((argc == 3) && (argv[2] != "-v"))) {
    if ((argc < 2) || (argc > 3)) {
        printf("Usage: %s \"<path>\" [-v]\n", argv[0]);
        return 1;
    }
    // I prefers to convert agruments in vars, for organization.
    char *dirpath;
    dirpath=argv[1];

    // This both vars will do the difference. : )
    unsigned long count=0;
    unsigned long size=0;
    // Some smart and simple approach to increment vars in other scopes
    listdir(dirpath, &count, &size);

    //char path=*dirpath;
    //if (verbose == "-v"){
    //replace(s.begin(), s.end(), '/', '_' );
    //printf("%s\n",dirpath);
    printf("This output is only for reference. The data was written in files.\n");
    printf("Files: %lu\nTotal Size: %lu MB\n", count, size/1024/1024); // size in MB.
    //}
    return 0;
}
