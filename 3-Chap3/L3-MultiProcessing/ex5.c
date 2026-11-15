#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void signal_handler(int signum) {
    int pid = wait(NULL);
    printf("Child process with PID %d terminated.\n", pid);
}

int main() {
    signal(SIGCHLD, signal_handler);
    if (fork() == 0){
        printf("Child process: PID: %d\n", getpid());
        sleep(10);
        exit(0);
    } else {
        printf("Parent process: PID: %d\n", getpid());
        getchar(); // Wait for user input to terminate the parent process
        return 0;
    }
    return 0;
}