#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Tien trinh ban dau: PID = %d\n", getpid());
    pid_t pid = fork();
    if (pid == 0)
    {
        // Tien trinh con
        printf("[CON] PID = %d, PPID = %d\n",
               getpid(), getppid());
    }
    else
    {
        // Tien trinh cha
        printf("[CHA] PID = %d, PID con = %d\n",
               getpid(), pid);
        wait(NULL);
        printf("[CHA] Tien trinh con da ket thuc\n");
    }

    return 0;
}