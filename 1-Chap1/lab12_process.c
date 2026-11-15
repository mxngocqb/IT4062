/* =====================================================================
 * Lab 12 - Dieu khien tien trinh   (slide 28, 30-32)
 *   system(), fork(), execl(), wait(), exit()
 * Server da tien trinh o chuong 3 dung dung mo hinh fork() + wait() nay.
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    /* 1. system() = fork() + execl() + wait() gop lai */
    printf("=== 1. system(\"ls -l | head -3\") ===\n");
    fflush(stdout);                       /* day bo dem truoc khi tao tien trinh con */
    if (system("ls -l | head -3") != 0)
        fprintf(stderr, "khong chay duoc lenh ls\n");

    printf("\n=== 2. fork() ===\n");
    printf("PID cua toi = %d\n", (int)getpid());
    fflush(stdout);

    pid_t pid = fork();                   /* tu day co HAI tien trinh cung chay */
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        /* ----- tien trinh CON: fork() tra ve 0 ----- */
        printf("  [CON] PID = %d, cha = %d\n", (int)getpid(), (int)getppid());
        fflush(stdout);
        execl("/bin/echo", "echo", "  [CON] xin chao tu /bin/echo", (char *)NULL);
        perror("  [CON] execl that bai");  /* chi toi day khi execl loi */
        _exit(127);
    }

    /* ----- tien trinh CHA: fork() tra ve PID cua con ----- */
    printf("  [CHA] PID = %d, con = %d, dang cho...\n", (int)getpid(), (int)pid);
    fflush(stdout);

    int trang_thai;
    if (wait(&trang_thai) < 0) { perror("wait"); return 1; }

    if (WIFEXITED(trang_thai))
        printf("  [CHA] con thoat binh thuong, ma tra ve = %d\n",
               WEXITSTATUS(trang_thai));
    else if (WIFSIGNALED(trang_thai))
        printf("  [CHA] con bi ket thuc boi tin hieu %d\n", WTERMSIG(trang_thai));
    return 0;
}
