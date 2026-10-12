#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    if (fork() == 0){
        printf("Child process: PID: %d\n", getpid());
        sleep(5);
        exit(0);
    } else {
        printf("Parent process: PID: %d\n", getpid());
        sleep(20);
        int status;
        wait(&status);
        printf("Child process finished with status: %d\n", WEXITSTATUS(status));
    }
    return 0;
}