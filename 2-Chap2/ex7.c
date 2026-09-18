#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Tien trinh ban dau: PID = %d\n", getpid());
    // fork() tạo ra một tiến trình con.
    // Giá trị trả về của fork():
    //   pid < 0  : fork thất bại
    //   pid == 0 : đang ở tiến trình con
    //   pid > 0  : đang ở tiến trình cha,
    //             pid chính là PID của tiến trình con
    pid_t pid = fork();
    if (pid == 0)
    {
        // Tien trinh con
        printf("[CON] PID = %d, PPID = %d\n",
               getpid(), getppid());
    }
    else
    {
        for (int i = 0; i < 100; i++)
        {
            printf("[CHA] PID = %d, PID con = %d, i = %d\n",
                   getpid(), pid, i);
            sleep(1);
        }
        // Tien trinh cha
        printf("[CHA] PID = %d, PID con = %d\n",
               getpid(), pid);
        // Tiến trình cha chờ tiến trình con kết thúc
        wait(NULL);
        printf("[CHA] Tien trinh con da ket thuc\n");
    }

    return 0;
}