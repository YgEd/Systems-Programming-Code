//Joshua Meharg
//I pledge my honor I have abided by the stevens honor system

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>
#include <string.h>
#include <fcntl.h>

void invalid_string(char* istring){
    fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", istring);
}

int main(int argc, char* argv[]){

    //variable to hold exit status of chil processes
    int status;
    if (argc != 5){
        fprintf(stderr, "Usage: ./spfind -d <directory> -p <permissions string>\n");
        return EXIT_FAILURE;
    }

    
    //Error checking if argv[4] is a valid permission string
    int j=0;
    int place = 4;
    while (j<=6){
    if (*(argv[place]+j) != 'r' && *(argv[place]+j) != '-'){
        invalid_string(argv[place]);
        // printf("error on 1\n");
        return EXIT_FAILURE;
    }
    if (*(argv[place]+1+j) != 'w' && *(argv[place]+1+j) != '-'){
        invalid_string(argv[place]);
        // printf("error on 2\n");
        return EXIT_FAILURE;
    }

    if (*(argv[place]+2+j) != 'x' && *(argv[place]+2+j) != '-'){
        invalid_string(argv[place]);
        // printf("error on 3\n");
        return EXIT_FAILURE;
    }
    j+=3;

    }

    //int array holding child processes' fds
    pid_t childs[2];
    int c2p_fds[2];
    int c2c_fds[2];
    //pipe between child process and parent
    pipe(c2c_fds);
    //pipe between child processes
    pipe(c2p_fds);
        

    childs[0] = fork();

    if (childs[0] == -1){
        fprintf(stderr, "fork() failed\n");
        return EXIT_FAILURE;
    }

    if (childs[0] == 0){
        //first child writes to child pipe
        //puts("in first child");
        close(c2p_fds[0]);
        close(c2p_fds[1]);
        close(c2c_fds[0]);
        dup2(c2c_fds[1], 1);
        
        execl("pfind", "pfind", argv[1], argv[2], argv[3], argv[4], NULL);
        fprintf(stderr, "Error: pfind failed\n");
        return EXIT_FAILURE;

    }

    childs[1] = fork();

    if (childs[1] == -1){
        fprintf(stderr, "fork() failed\n");
        return EXIT_FAILURE;
    }

    if(childs[1] == 0){
        //second child reads from pipe and writes to parent pipe
        //puts("in second child");
        close(c2c_fds[1]);
        dup2(c2c_fds[0], 0);

        
        close(c2p_fds[0]);
        dup2(c2p_fds[1], 1);
        
        execl("/bin/sort", "sort", NULL);
        fprintf(stderr, "Error: sort failed\n");
        return EXIT_FAILURE;
    }

    else{
        wait(&status);
        //make sure child processes exited normally
        if (status == 0){
            //puts("in parent");
            close(c2c_fds[0]);
            close(c2c_fds[1]);
            close(c2p_fds[1]);
            dup2(c2p_fds[0], 0);
            
            char car;
            int counter = 0;
            //reads each char at a time and updates
            //counter with the amount of lines/matches
            while (read(0, &car, 1) != 0){
                if(car == '\n'){
                    counter++;
                }
                printf("%c", car);
                
            }

            printf("Total matches: %d\n", counter);
            return 0;
        }
        return EXIT_FAILURE;
    
    }
    return EXIT_FAILURE;
    
}