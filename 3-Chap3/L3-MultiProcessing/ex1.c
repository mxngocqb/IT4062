#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();
    if (pid == 0) {
        printf("Child process: PID: %d, fork() value = %d\n", getpid(), pid);
    } else {
        printf("Parent process: PID: %d, fork() value (Children process) = %d\n", getpid(), pid);
    }
    return 0;
}