#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Before fork\n");
    printf("PID: %d\n", getpid());
    printf("After fork\n");
    fork();
    fork();
    printf("PID: %d\n", getpid());
    sleep(100);
    return 0;
}