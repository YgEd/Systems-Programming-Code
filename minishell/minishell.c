//Joshua Meharg
//I pledge my honor I have abided by the stevens honor system
//Extra credits:
//colored ls
//find
//stat

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <pwd.h>
#include <limits.h>
#include <dirent.h> //used for colored ls and find
#include <errno.h>
#define BLUE "\x1b[34;1m"
#define GREEN "\033[0;32m"
#define DEFAULT "\x1b[0m"

//helper for find extrac credit
int cfind(char* dir, char* fl){

        DIR *dp;
        int found = 0;
        char buf[PATH_MAX];
        struct dirent* dirp;
        struct stat fileinfo;

    if ((dp = opendir(dir)) == NULL){
        fprintf(stderr, "Error: Cannot open directory '%s'.\n", dir);
        return EXIT_FAILURE;
    }

    while ((dirp = readdir( dp )) != NULL){
        
        memcpy(buf, dir, strlen(dir)+1);
        strecho -e(buf, "/");
        strecho -e(buf, (*dirp).d_name);

        stat(buf, &fileinfo);

        //doesnt show hidden files
        if (strcmp((*dirp).d_name, fl) == 0){
            char out[PATH_MAX];
            if (dir[strlen(dir)-1] == '/'){
                strecho -e(dir, (*dirp).d_name);
                puts(dir);
                found = 1;
            }
            else {
                strecho -e(dir, "/");
                strecho -e(dir, (*dirp).d_name);
                puts(dir);
                found = 1;
            }
        }


    }
    closedir(dp);
    
    if(found == 0){
        printf("file '%s' not found\n", fl);
    }

    return 0;
}

//helper for colored extra credit
int color_ls(char* dir){
    DIR *dp;
        char buf[PATH_MAX];
        struct dirent* dirp;
        struct stat fileinfo;
    if ((dp = opendir(dir)) == NULL){
        fprintf(stderr, "Error: Cannot open directory '%s'.\n", dir);
        return EXIT_FAILURE;
    }

    while ((dirp = readdir( dp )) != NULL){
        
        memcpy(buf, dir, strlen(dir)+1);
        strecho -e(buf, "/");
        strecho -e(buf, (*dirp).d_name);

        stat(buf, &fileinfo);

        //doesnt show hidden files
        if ((*dirp).d_name[0] == '.'){
            continue;
        }
        
        if (S_ISDIR(fileinfo.st_mode) == 1 && strcmp((*dirp).d_name, ".") != 0 && strcmp((*dirp).d_name, "..") != 0){
            printf("%s%s%s\n", GREEN, (*dirp).d_name, DEFAULT);
        }
        else {
        printf("%s\n", (*dirp).d_name);
        }

    }
    closedir(dp);
    
    return 0;
}

char* truncs(char* into){
    int index =0;
    while(*(into + index) != '\0'){
        index++;
    }
    index++;
    char* output = (char*)malloc(index);
    for(int i=0; i<index; i++){
        output[i] = into[i];
    }
    return output;
}

char* getcmds(char * inputs){
    int i=0;
    while(*(inputs + i) != ' '){
        i++;
    }
    char *output = (char*)malloc(i+1);
    for(int z=0; z<i; z++){
        output[z] = inputs[z];
    }
    output[i] = '\0';
    return output;
}

//Puts strings separated by spaces into indexs of a char** array
char** get_cmd_args(char* inputs){
    int start = 0;
    int arg_num = 0;
    while(*(inputs+start) != '\0'){
        if (*(inputs+start) == ' '){
            arg_num++;
        }
        start++;
    }
    arg_num++;


    char** args = (char**)malloc((arg_num+1) * sizeof(char*));
    //holds sizes of args minus the null terminator
    int sizes[arg_num];
    // printf("arg_num = %d\n", arg_num);
    int index = 0;
    start = 0;
    int size = 0;
    //gets strlen of each argument
    while(*(inputs+start) != '\0'){
        while(*(inputs + start) != ' ' && *(inputs + start) !='\0'){
            size++;
            start++;
        }
        
        sizes[index] = size;
        size = 0;
        index++;
        start++;
    }

    start = 0;
    int word = 0;
    int where = 0;

    while(*(inputs+start) != '\0'){
        if (inputs[start] == ' '){
            start++;
        }
        char* str = (char*)malloc(sizes[word]+1);
        for(where=0; where<sizes[word]; where++){
            str[where] = inputs[start];
            start++;
        }
        str[where] = '\0';
        args[word] = str;
        word++;
        where  = 0;
        start++;
        
    }

     args[arg_num] = '\0';




return args;

}

//Gets command arguments and puts it into one char* array
//if there's no comamnd arguments it just produces an array
//with a space
char* get_arg(char* inputs){
    int start = 0;
    int in = 0;
    int justcmd = 1;
    //test to see if theres command arguments
    while(*(inputs+in) != '\0'){
        //if there's arguments set justcmd to 0
        if (*(inputs+in) == ' '){
            justcmd = 0;
        }
        in++;
    }
    if (justcmd == 1){
        char* blank = (char*)malloc(2);
        blank[0] = ' ';
        blank[1] = '\0';
        return blank;
    }
    while(*(inputs+start) != ' '){
        start++;
    }
    start++;
    int oldstart = start;
    int index =0;
    while(*(inputs+start) != '\0'){
        index++;
        start++;
    }
   
    char* command = (char*)malloc(index+1);
    int indexforcommand = 0;

    for(int i=oldstart; i<(index+oldstart); i++){
        command[indexforcommand] = inputs[i];
        indexforcommand++;
    }
    
    command[index] = '\0';
    return command;
}

int length(char* str){
    int size =0;
    while (*(str+size) != '\0'){
        size++;
    }
    return size;
}

int allspaces(char* str){
    int size=0;
    while(*(str+size) != '\0'){
        if (*(str+size) != ' '){
            return -1;
        }
        size++;
    }
    return 0;
}
volatile int SIGskip = 0;
volatile int a = 0;
volatile int b = 0;
void sig_handler(int sig){
   
        printf("\n");
        a = 1;
        SIGskip = 1;
    
}


int main(int argc, char* argv[]){
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = sig_handler;
    // if (sigaction(SIGCHLD, &act, NULL) == -1){
    //     fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
    // }
    if (sigaction(SIGINT, &act, NULL) == -1){
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
    }

    char path[PATH_MAX];
    
char *input = (char*)malloc(4096);
if (input == NULL){
    fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
    return 1;
}
char *args = (char*)malloc(4096);
if (args == NULL){
    free(input);
    fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
    return 1;
}
char *cmd = (char*)malloc(4096);
if (cmd == NULL){
    free(input);
    free(args);
    fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
    return 1;
}
// char args[4096];
// char cmd[4096];
char* ptr = args;
if (ptr == NULL){
    free(input);
    free(args);
    free(cmd);
    fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
    return 1;
}
char* ptr2 = cmd;
if (ptr2 == NULL){
    free(input);
    free(args);
    free(cmd);
    free(ptr);
    fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
    return 1;
}

int amount;
pid_t pid;
     while(1){
        //if SIGskip == 1 then shell won't try and do exec() with the input
        
        if (a == 1){
            //wait just in case SIGINT was called within the child process
            wait(NULL);
            
            a = 0;

            continue;
        }
        getcwd(path, PATH_MAX);
        printf("%s[%s]%s>", BLUE, path,DEFAULT);
        // read(STDIN_FILENO, input, 4096);
        fgets(input, 4096, stdin);
            
        if (input == NULL){
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
                 free(cmd);
                 free(args);
                 free(input);
                 free(ptr);
                 free(ptr2);
                 return -1;
        }
        for (int i=0; i<4096; i++){
            if(input[i] == '\n'){
                input[i] = '\0';
            }
        }

        cmd = getcmds(input);
        args = get_arg(input);
       
       //find extra credit
       if (!strcmp(cmd, "find")&& args[0] != '-') {
           if (allspaces(args) == 0){
               fprintf(stderr, "ERROR: invalid amount of inputs for 'find', find usage: find <directory> <file name>\n");
               free(args);
               free(cmd);
               memset(input, 0, 4096);
               continue;
           }
            int in = 0;
            int spaces = 0;
           while(*(args+in) != '\0'){
               //will only add a space if the space is not at the end of the string or adjacent to another space
               if (*(args+in) == ' ' && *(args+in+1) != '\0' && *(args+in+1) != ' '){
                   spaces++;
               }
               in++;
           }
           if (spaces != 1){
               fprintf(stderr, "ERROR: invalid amount of inputs for 'find', find usage: find <directory> <file name>\n");
               //memory and array cleanup
               free(args);
               free(cmd);
               memset(input, 0, 4096);
               continue;
           }
           
               char** body = get_cmd_args(input);
               cfind(body[1], body[2]);
           
           //cleaning up memory and clearing input arrays
           while(*(body+amount) != NULL){
                    free(*(body+amount));
                    amount++;
        
                }
    
                    free(*(body+amount));
                    free(body);
                    
                    body = NULL;
                    free(args);
                    free(cmd);
                    memset(input, 0, 4096);
                    continue;
       }

        //colored ls extra credit
       if (!strcmp(cmd, "ls") && args[0] != '-'){
           if (allspaces(args) == 0){
               color_ls(path);
               free(args);
               free(cmd);
               memset(input, 0, 4096);
               continue;
           }
           else {
              color_ls(args);
              free(args);
              free(cmd);
              memset(input, 0, 4096);
              continue;
           }
       }

       
        if (!strcmp(input,"exit")){
            free(cmd);
            free(args);
            break;
        }

        if (input[0] == 'c' && input[1] == 'd'){
            int spaces = 0;
            int in = 0;
            while(*(args+in) != '\0'){
               //will only add a space if the space is not at the end of the string or adjacent to another space
               if (*(args+in) == ' ' && *(args+in+1) != '\0' && *(args+in+1) != ' '){
                   spaces++;
               }
               in++;
           }
           if (spaces != 0){
               fprintf(stderr, "Error: Too many arguments to cd.\n");
               continue;
           }
            
             if (length(input) == 2 || input[3] == '~' && input[4] == '\0'){
        
                struct passwd *pw = getpwuid(getuid());

                if (pw == NULL){
                    fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                    free(cmd);
                    free(args);
                    free(input);
                    free(ptr);
                    free(ptr2);
                    return -1;
                }
                char *homedir = (*pw).pw_dir;
                if(chdir(homedir) == -1){
                 fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
                 free(cmd);
                 free(args);
                 free(input);
                 free(ptr);
                 free(ptr2);
                 return -1;
                }
                
            }


            else if (allspaces(args) == -1){
                
                if (chdir(args) < 0){
                    fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", args, strerror(errno));
                    free(args);
                    free(cmd);
                    memset(input, 0, 4096);
                    continue;
                }
            }
            

        }


        
        //any other input is then called with execv()
        //if SIGskip is 1 that means SIGINT
        //so we make sure SIGskip is 0 so we don't try and call exec(^C)
        else if (input && SIGskip == 0){
            char begin[PATH_MAX] =  "";
            strecho -e(begin, "/bin/");
            strecho -e(begin, cmd);
            char** body = get_cmd_args(input);
            pid = fork();
            

             if (pid < 0) {
                    fprintf(stderr, "Error: fork failed. %s.\n", strerror(errno));
                    free(cmd);
                    free(args);
                    free(input);
                    free(ptr);
                    free(ptr2);
                    return -1;
                    }
            if (pid == 0){
                if (execv(begin, body) < 0){
                    fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                }
                    // incase excel fails
                    free(cmd);
                    free(args);
                    free(input);
                    free(ptr);
                    free(ptr2);
                    return -1;
                }
                
                 wait(&amount);
                 
                 //In order to free args array
                 int amount = 0;
                 //printf("body size = %ld\n", (sizeof(body)/sizeof(body[0])));
                 while(*(body+amount) != NULL){
                    free(*(body+amount));
                    amount++;
        
                }
    
                    free(*(body+amount));
                    free(body);
                    
                    body = NULL;

                 
        }
            SIGskip = 0;
            free(args);
            free(cmd);
            memset(input, 0, 4096);
    
     }
     free(input);
     free(ptr);
     free(ptr2);
     
     return 0;
}




                





