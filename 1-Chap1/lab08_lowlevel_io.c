/* =====================================================================
 * Lab 08 - Vao/ra muc thap: open / read / write / close   (slide 21)
 *
 * Day chinh la mo hinh cua socket:  read/write  <->  recv/send.
 * Diem quan trong: write() co the ghi IT hon so byte yeu cau
 * => phai lap cho toi khi du (ham ghi_het duoi day).
 *
 * Chay: ./lab08_lowlevel_io [tep_nguon] [tep_dich]
 * ===================================================================== */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Ghi cho DU n byte - giong het vong lap gui du lieu qua socket */
static int ghi_het(int fd, const char *bo_dem, size_t n)
{
    size_t da_ghi = 0;
    while (da_ghi < n) {
        ssize_t k = write(fd, bo_dem + da_ghi, n - da_ghi);
        if (k < 0) { perror("write"); return -1; }
        da_ghi += (size_t)k;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    const char *ten_nguon = (argc > 1) ? argv[1] : "nguon_mau.bin";
    const char *ten_dich  = (argc > 2) ? argv[2] : "ban_sao.bin";

    /* Khong co tham so thi tu tao tep mau */
    if (argc <= 1) {
        int fd = open(ten_nguon, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) { perror("open tao tep mau"); return 1; }
        const char *mau = "IT4062 - low level I/O: open, read, write, close\n";
        ghi_het(fd, mau, strlen(mau));
        close(fd);
    }

    int fd_nguon = open(ten_nguon, O_RDONLY);
    if (fd_nguon < 0) { perror("open nguon"); return 1; }

    int fd_dich = open(ten_dich, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dich < 0) { perror("open dich"); close(fd_nguon); return 1; }

    printf("fd nguon = %d, fd dich = %d  (0=stdin, 1=stdout, 2=stderr)\n",
           fd_nguon, fd_dich);

    char bo_dem[4096];
    ssize_t so_doc;
    long tong = 0;
    while ((so_doc = read(fd_nguon, bo_dem, sizeof bo_dem)) > 0) {
        if (ghi_het(fd_dich, bo_dem, (size_t)so_doc) < 0) return 1;
        tong += so_doc;
    }
    /* so_doc == 0 nghia la HET TEP, giong recv() tra ve 0 khi doi tac dong */
    if (so_doc < 0) { perror("read"); return 1; }

    close(fd_nguon);
    close(fd_dich);
    printf("Da sao chep %ld byte tu %s sang %s\n", tong, ten_nguon, ten_dich);

    struct stat s1, s2;
    if (stat(ten_nguon, &s1) == 0 && stat(ten_dich, &s2) == 0)
        printf("Kich thuoc: %lld / %lld byte -> %s\n",
               (long long)s1.st_size, (long long)s2.st_size,
               s1.st_size == s2.st_size ? "GIONG NHAU" : "KHAC NHAU");
    return 0;
}
