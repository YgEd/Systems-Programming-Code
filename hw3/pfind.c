// Joshua Meharg
// CS 392, March 7, 2022
// I pledge my honor I have abided by the Stevens Honor System
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <limits.h>



void invalid_string(char* istring){
    fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", istring);

}

void printstring(char* s, int length){
    for (int z=0; z<length; z++){
        printf("%c", *(s+z));
    }
    printf("\n");
}

// testperms retrives the permission bits of inputted file
// and stores it into a char array pstring
char* testperms(struct stat fileinfo){
    char* pstring = (char*)malloc(10*sizeof(char));
    if (fileinfo.st_mode & S_IRUSR){
        // printf("r");
        pstring[0] = 'r';
    }
    else{
        // printf("-");
        pstring[0] = '-';
    }
    if (fileinfo.st_mode & S_IWUSR){
        // printf("w");
        pstring[1] = 'w';
    }
    else{
        // printf("-");
        pstring[1] = '-';
    }
    if (fileinfo.st_mode & S_IXUSR){
        // printf("x");
        pstring[2] = 'x';
    }
    else{
        // printf("-");
        pstring[2] = '-';
    }
    if (fileinfo.st_mode & S_IRGRP){
        // printf("r");
        pstring[3] = 'r';
    }
    else{
        // printf("-");
        pstring[3] = '-';
    }
    if (fileinfo.st_mode & S_IWGRP){
        // printf("w");
        pstring[4] = 'w';
    }
    else{
        // printf("-");
        pstring[4] = '-';
    }
    if (fileinfo.st_mode & S_IXGRP){
        // printf("x");
        pstring[5] = 'x';
    }
    else{
        // printf("-");
        pstring[5] = '-';
    }
    if (fileinfo.st_mode & S_IROTH){
        // printf("r");
        pstring[6] = 'r';
    }
    else{
        // printf("-");
        pstring[6] = '-';
    }
    if (fileinfo.st_mode & S_IWOTH){
        // printf("w");
        pstring[7] = 'w';
    }
    else{
        // printf("-");
        pstring[7] = '-';
    }
    if (fileinfo.st_mode & S_IXOTH){
        // printf("x");
        pstring[8] = 'x';
    }
    else{
        // printf("-");
        pstring[8] = '-';
    }
    //null terminator is needed as the permission bits is a string and will not strcmp properly without it
    pstring[9] = '\0';
    
    return pstring;
    }


// compares the permission bits of inputted in argv[2] and eacch file
// if the file is a directory it recursively calls 'dohw' within the files in its directory
void dohw(char* argv, char* perms){

    DIR* dp;
    struct dirent* dirp;
    struct stat fileinfo; //fileinfo is not a pointer bc you use it as an object
    

    if ((dp = opendir(argv)) == NULL){
        // fprintf(stderr, "Error: Cannot open directory '%s'. Permission denied.\n", argv[1]);
        // return EXIT_FAILURE;
    }
 
    //found value is how many files match the permission string (argv[2])
    int found = 0;

    //sets buf that will hold the path to the current file to the size of the largest possible path
    char buf[PATH_MAX+1];

    //homedir holds the home directory + argv[1]
    
    strcat(argv, "/");

    while ((dirp = readdir( dp )) != NULL){
        //sets up the path of the current file
        memcpy(buf, argv, strlen(argv)+1);
        strcat(buf, (*dirp).d_name);
        // puts(buf);
        // printf("%s has value %d.\n", (*dirp).d_name, stat(buf, &fileinfo));
        
        
        stat(buf, &fileinfo);
        char* arrptr = testperms(fileinfo);
        if (S_ISDIR(fileinfo.st_mode) == 1 && strcmp((*dirp).d_name, ".") != 0 && strcmp((*dirp).d_name, "..") != 0){
            dohw(buf, perms);
            free(arrptr);
            continue;
        }
        if (strcmp(perms, arrptr) == 0 ){
            puts(buf);
            found++;
        }
        // printf("%s ", buf);
        // printf("%s ", testperms(fileinfo));
        //printf("%d\n", strcmp(argv[2], testperms(fileinfo)));
        // printstring(testperms(fileinfo),9);
        
        free(arrptr);
        memcpy(buf, argv, strlen(argv)+1);
    }
    closedir(dp);
}

int main (int argc, char* argv[]){

if (argc <= 2){
    printf("%s <directory> <permissions string>\n", argv[0]);
    return EXIT_FAILURE;
}
if (argc > 3){
    fprintf(stderr, "Error: too many files inputted.\n");
    return EXIT_FAILURE;
}

// printf("sizeof argv[1] = %ld\n", strlen(argv[1]));
if (strlen(argv[2]) != 9){
    invalid_string(argv[2]);
    return EXIT_FAILURE;
}

int j=0;

//Error checking if argv[2] is a valid permission string
while (j<=6){
if (*(argv[2]+j) != 'r' && *(argv[2]+j) != '-'){
    invalid_string(argv[2]);
    // printf("error on 1\n");
    return EXIT_FAILURE;
}
if (*(argv[2]+1+j) != 'w' && *(argv[2]+1+j) != '-'){
    invalid_string(argv[2]);
    // printf("error on 2\n");
    return EXIT_FAILURE;
}

if (*(argv[2]+2+j) != 'x' && *(argv[2]+2+j) != '-'){
    invalid_string(argv[2]);
    // printf("error on 3\n");
    return EXIT_FAILURE;
}
j+=3;

}

    DIR* dp;
    struct dirent* dirp;
    struct stat fileinfo; //fileinfo is not a pointer bc you use it as an object
    // struct passwd *pw = getpwuid(getuid());
    // char *homedir = (*pw).pw_dir;
    // strcat(homedir,"/");
    // strcat(homedir, argv[1]);


    if ((dp = opendir(argv[1])) == NULL){
        fprintf(stderr, "Error: Cannot open directory '%s'. Permission denied.\n", argv[1]);
        return EXIT_FAILURE;
    }
 
    //found value is how many files match the permission string (argv[2])
    int found = 0;

    //sets buf that will hold the path to the current file to the size of the largest possible path
    char buf[PATH_MAX+1];
    char buf2[PATH_MAX+1];

    //homedir holds the home directory + argv[1]
    

    while ((dirp = readdir( dp )) != NULL){
        //sets up the path of the current file
        memcpy(buf, argv[1], strlen(argv[1])+1);
        strcat(buf, "/");
        strcat(buf, (*dirp).d_name);
        // puts(buf);
        // printf("%s has value %d.\n", (*dirp).d_name, stat(buf, &fileinfo));
        
        
        stat(buf, &fileinfo);
        
        char* arrptr = testperms(fileinfo);
        
        // Makes sure directory's . and .. are not recursively calling dohw as it would add duplicated information
        if (S_ISDIR(fileinfo.st_mode) == 1 && strcmp((*dirp).d_name, ".") != 0 && strcmp((*dirp).d_name, "..") != 0){
            char buffer[PATH_MAX+1];
            
            // if just the name of a directory was inputted into argv[1]
            // realpath retries its full path and concats it to the front
            // of the name in argv[1]
            if (*(argv[1]) != '/'){
                realpath(argv[1],buffer);
                strcat(buffer, "/");
                // printf("this is buf = %s\n", buf);
                // printf("this is buffer = %s\n", buffer);
                strcat(buffer, (*dirp).d_name);
                dohw(buffer, argv[2]);
                memcpy(buffer, buf2, strlen(buf2)+1);
                free(arrptr);
                continue;
            }
            else{
            dohw(buf, argv[2]);
            free(arrptr);
            continue;
            }
        }

        if (strcmp(argv[2], arrptr) == 0 ){
            char buf2[PATH_MAX+1];
            char buffer[PATH_MAX+1];
            
            // if just the name of a directory was inputted into argv[1]
            // realpath retries its full path and concats it to the front
            // of the name in argv[1]
            if (*(argv[1]) != '/'){
                realpath(argv[1],buffer);
                strcat(buffer, "/");
                // printf("this is buf = %s\n", buf);
                // printf("this is buffer = %s\n", buffer);
                strcat(buffer, (*dirp).d_name);
                puts(buffer);
                found++;
                memcpy(buffer, buf2, strlen(buf2)+1);
            }
            else{
            
            puts(buf);
            found++;
            }
            
        }
        
        
        
        free(arrptr);
        memcpy(buf, argv[1], strlen(argv[1])+1);
    }
    if (found == 0){
        printf("<no output>\n");
    }
    

    closedir(dp);

    


return 0;
}