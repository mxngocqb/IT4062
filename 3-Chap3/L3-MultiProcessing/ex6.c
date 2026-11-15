#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void signal_handler(int sig)
{
    int pid = wait(NULL);
    printf("Child process with PID %d has terminated.\n", pid);
}

int main() {
    signal(SIGCHLD, signal_handler);
    printf("Parent process: PID: %d\n", getpid());
    if (fork() == 0){
        printf("Child process: PID: %d\n", getpid());
        sleep(10);
        exit(0);
    }
    getchar();
    return 0;
}