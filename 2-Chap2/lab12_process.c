/* =====================================================================
 * Lab 12 - Dieu khien tien trinh
 * Nguon: Chapter 2, slide 28, 30-32 (Process Control)
 *
 *   system(), execl(), fork(), wait(), exit()
 *
 * Y nghia trong lap trinh mang: server theo co che da tien trinh
 * (chuong 3) dung dung mo hinh fork() + wait() nay.
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    /* ---------- 1. system(): chay mot lenh cua he dieu hanh ---------- */
    printf("=== 1. system(\"ls -l\") ===\n");
    fflush(stdout);                 /* day bo dem truoc khi tao tien trinh con */
    int ma = system("ls -l | head -5");
    printf("system() tra ve %d\n", ma);
    printf("(system = fork() + execl() + wait() gop lai)\n");

    /* ---------- 2. Thong tin tien trinh hien tai ---------- */
    printf("\n=== 2. Thong tin tien trinh ===\n");
    printf("PID cua toi        = %d\n", (int)getpid());
    printf("PID cua tien trinh cha = %d\n", (int)getppid());

    /* ---------- 3. fork(): tach lam hai tien trinh ---------- */
    printf("\n=== 3. fork() ===\n");
    fflush(stdout);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* ----- Tien trinh CON ----- */
        printf("  [CON ] PID = %d, cha = %d\n", (int)getpid(), (int)getppid());
        printf("  [CON ] fork() tra ve %d (bang 0 o tien trinh con)\n", (int)pid);
        printf("  [CON ] sap goi execl(\"/bin/echo\", ...)\n");
        fflush(stdout);

        /* execl thay the toan bo anh tien trinh hien tai:
         * neu thanh cong thi cac dong lenh phia sau KHONG bao gio chay */
        execl("/bin/echo", "echo", "  [CON ] xin chao tu /bin/echo",
              (char *)NULL);

        perror("  [CON ] execl that bai");   /* chi toi day khi execl loi */
        _exit(127);
    }

    /* ----- Tien trinh CHA ----- */
    printf("  [CHA ] PID = %d, con = %d\n", (int)getpid(), (int)pid);
    printf("  [CHA ] dang cho con ket thuc...\n");
    fflush(stdout);

    int trang_thai;
    pid_t con = wait(&trang_thai);           /* cho mot tien trinh con bat ky */
    if (con < 0) {
        perror("wait");
        return 1;
    }

    printf("  [CHA ] con %d da ket thuc: ", (int)con);
    if (WIFEXITED(trang_thai))
        printf("thoat binh thuong, ma tra ve = %d\n", WEXITSTATUS(trang_thai));
    else if (WIFSIGNALED(trang_thai))
        printf("bi ket thuc boi tin hieu %d\n", WTERMSIG(trang_thai));
    else
        printf("khong ro\n");

    /* ---------- 4. Nhieu tien trinh con ---------- */
    printf("\n=== 4. Tao 3 tien trinh con ===\n");
    fflush(stdout);

    for (int i = 1; i <= 3; i++) {
        pid_t p = fork();
        if (p < 0) {
            perror("fork");
            break;
        }
        if (p == 0) {
            printf("  [CON %d] PID = %d, thoat voi ma %d\n",
                   i, (int)getpid(), i * 10);
            fflush(stdout);
            _exit(i * 10);          /* _exit trong tien trinh con sau fork */
        }
    }

    int con_lai;
    while ((con_lai = (int)wait(&trang_thai)) > 0) {
        if (WIFEXITED(trang_thai))
            printf("  [CHA ] thu duoc con %d, ma tra ve = %d\n",
                   con_lai, WEXITSTATUS(trang_thai));
    }
    printf("  [CHA ] khong con tien trinh con nao. Ket thuc.\n");

    /* exit(0) tra ve ma trang thai 0 cho he dieu hanh (kiem tra bang: echo $?) */
    exit(0);
}
