/* =====================================================================
 * Lab 08 - Vao/ra muc thap (khong co bo dem): open/read/write/close
 * Nguon: Chapter 2, slide 21 (Low Level I/O)
 *
 * Day chinh la mo hinh ma cac ham socket su dung:
 *   read()/write() tren mo ta tep  <->  recv()/send() tren socket.
 * Diem quan trong: read/write co the lam viec voi IT BYTE HON so byte
 * yeu cau => phai lap cho toi khi du.
 *
 * Cach chay:  ./lab08_lowlevel_io [tep_nguon] [tep_dich]
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define KICH_THUOC_DEM 4096

/* Ghi cho DU n byte: xu ly truong hop write() ghi thieu.
 * Ham nay giong het vong lap gui du lieu qua socket. */
static ssize_t ghi_het(int fd, const char *bo_dem, size_t n)
{
    size_t da_ghi = 0;
    while (da_ghi < n) {
        ssize_t k = write(fd, bo_dem + da_ghi, n - da_ghi);
        if (k < 0) {
            perror("write");
            return -1;
        }
        da_ghi += (size_t)k;
    }
    return (ssize_t)da_ghi;
}

/* In noi dung dang hexa + ky tu, giong lenh hexdump -C */
static void do_hex(const unsigned char *p, size_t n)
{
    for (size_t i = 0; i < n; i += 16) {
        printf("  %08zx  ", i);
        for (size_t j = 0; j < 16; j++) {
            if (i + j < n) printf("%02X ", p[i + j]);
            else           printf("   ");
            if (j == 7) putchar(' ');
        }
        printf(" |");
        for (size_t j = 0; j < 16 && i + j < n; j++) {
            unsigned char c = p[i + j];
            putchar((c >= 32 && c < 127) ? c : '.');
        }
        printf("|\n");
    }
}

int main(int argc, char *argv[])
{
    const char *ten_nguon = (argc > 1) ? argv[1] : "nguon_mau.bin";
    const char *ten_dich  = (argc > 2) ? argv[2] : "ban_sao.bin";

    /* ---------- 0. Neu khong co tham so: tu tao tep mau ---------- */
    if (argc <= 1) {
        int fd = open(ten_nguon, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) { perror("open tao tep mau"); return 1; }
        const char *mau = "IT4062 - Thuc hanh Lap trinh mang\n"
                          "Low level I/O: open, read, write, close\n";
        ghi_het(fd, mau, strlen(mau));
        close(fd);
        printf("Da tao tep mau: %s\n\n", ten_nguon);
    }

    /* ---------- 1. Mo tep nguon de doc ---------- */
    int fd_nguon = open(ten_nguon, O_RDONLY);
    if (fd_nguon < 0) {
        perror("open nguon");
        return 1;
    }

    /* ---------- 2. Mo/tao tep dich de ghi ---------- */
    int fd_dich = open(ten_dich, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dich < 0) {
        perror("open dich");
        close(fd_nguon);
        return 1;
    }

    printf("Mo ta tep (file descriptor): nguon = %d, dich = %d\n",
           fd_nguon, fd_dich);
    printf("(0 = stdin, 1 = stdout, 2 = stderr nen tep dau tien thuong la 3)\n\n");

    /* ---------- 3. Vong lap sao chep ---------- */
    char bo_dem[KICH_THUOC_DEM];
    ssize_t so_doc;
    long tong = 0;
    int lan = 0;

    while ((so_doc = read(fd_nguon, bo_dem, sizeof bo_dem)) > 0) {
        if (ghi_het(fd_dich, bo_dem, (size_t)so_doc) < 0) {
            close(fd_nguon);
            close(fd_dich);
            return 1;
        }
        tong += so_doc;
        lan++;
    }
    if (so_doc < 0) {
        perror("read");
        close(fd_nguon);
        close(fd_dich);
        return 1;
    }
    /* so_doc == 0 nghia la HET TEP -- tuong duong recv() tra ve 0
     * khi doi tac da dong ket noi. */

    printf("Da sao chep %ld byte qua %d lan read/write\n", tong, lan);
    close(fd_dich);

    /* ---------- 4. Doc lai 96 byte dau de xem dang hexa ---------- */
    if (lseek(fd_nguon, 0, SEEK_SET) == (off_t)-1)
        perror("lseek");
    unsigned char dau[96];
    ssize_t k = read(fd_nguon, dau, sizeof dau);
    if (k > 0) {
        printf("\n%zd byte dau cua %s:\n", k, ten_nguon);
        do_hex(dau, (size_t)k);
    }
    close(fd_nguon);

    /* ---------- 5. So sanh kich thuoc bang stat ---------- */
    struct stat st1, st2;
    if (stat(ten_nguon, &st1) == 0 && stat(ten_dich, &st2) == 0)
        printf("\nKich thuoc: %s = %lld byte, %s = %lld byte -> %s\n",
               ten_nguon, (long long)st1.st_size,
               ten_dich,  (long long)st2.st_size,
               (st1.st_size == st2.st_size) ? "GIONG NHAU" : "KHAC NHAU");
    return 0;
}
