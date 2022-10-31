#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main() {

    
    printf("I'm SHELL process, with PID: %d - Main command is: man -P cat ls | grep -e -A -A 1 -m 1 \n", getpid());
    //file descriptor creation.
    int fd[2]; pipe(fd);
    //child for man commmand 
    int pid1 = fork();
    if (pid1 < 0) {
        exit(1);
    } 
    else if (pid1 == 0) {

        printf("I’m MAN process, with PID: %d - My command is: man -P cat ls \n", getpid());

        // Writes man process result to write-end of file descriptor
        dup2(fd[1], STDOUT_FILENO);
        execlp("man", "man", "-P", "cat", "ls", NULL);
    } 
    
    // Returns to main "SHELL" process
    else{

        // Creates a child process for GREP process
        int pid2 = fork();

        // Make sure that the fork is successful 
        if (pid2 < 0) {
            exit(1);
        } 
        
        // Child "GREP" process
        else if (pid2 == 0) {

            printf("I’m GREP process, with PID: %d - My command is: grep -e -A -A 1 -m 1 \n", getpid());

            // Creates output.txt file
            int filefd = open("output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

            // Gets man process output from read-end of file descriptor
            dup2(fd[0], STDIN_FILENO);

            // Writes grep process result to output.txt
            dup2(filefd, STDOUT_FILENO);
            execlp("grep", "grep", "-e", "-A", "-A", "1", "-m", "1", NULL);
        } 

        else{

            //wait for child and print last msg.
            waitpid(pid1, NULL, 0); 
            waitpid(pid2, NULL, 0);            
            printf("I’m SHELL process, with PID: %d - execution is completed, you can find the results in output.txt \n", getpid());
        }
    }
    return 0;
    
}