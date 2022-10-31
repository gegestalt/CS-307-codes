#include <string.h>
#include <unistd.h> 
#include <stdio.h>
int main(int argc,char* argv[]){
    execlp("ping","ping","-c","3","google.com",NULL);
    
}