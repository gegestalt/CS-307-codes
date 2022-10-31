#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
int main() {
    printf("I am SHELL process, with PID: %d - Main command is: man -P cat ls | grep -e -B -B 1 -m 1 \n", getpid());
    int fd[2]; pipe(fd);
    //child for man commmand 
    int pid1 = fork();
    if (pid1 < 0) {
        exit(1);
    } 
    else if (pid1 == 0) {
        printf("I’m MAN process, with PID: %d - My command is: man -P cat ls \n", getpid());
        dup2(fd[1], STDOUT_FILENO);
        execlp("man", "man", "-P", "cat", "ls", NULL);
    } 
    
    else{//return shell process
        int pid2 = fork();
        if (pid2 < 0) {
            exit(1);
        } 
        else if (pid2 == 0) {

            printf("I’m GREP process, with PID: %d - My command is: grep -e -B -B 1 -m 1 \n", getpid());
            int filefd = open("output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(fd[0], STDIN_FILENO);
            dup2(filefd, STDOUT_FILENO);
            execlp("grep", "grep", "-e", "-B", "-B", "1", "-m", "1", NULL);
        } 
        else{
            printf("I’m SHELL process, with PID: %d - execution is completed, you can find the results in output.txt \n", getpid());
        }
    }
    return 0;
    
}