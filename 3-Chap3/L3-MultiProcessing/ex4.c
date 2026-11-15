#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(){
    for (int i = 0; i < 5; i++){
        if (fork() == 0) {
            while (1){
                printf("Child process: PID: %d\n", getpid());
                sleep(1);
            }
        }
    }
    printf("Parent process: PID: %d\n", getpid());
    sleep(10);
    killpg(0, SIGKILL); // Terminate all child processes
    return 0;
}