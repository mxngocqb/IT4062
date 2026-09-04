/* =====================================================================
 * Lab 06 - Vao/ra chuan, bao loi va cac dong (stream)
 * Nguon: Chapter 2, slide 16-18 (Input and Output)
 *
 *   stdin / stdout / stderr, perror(), errno, exit()
 *   getchar, putchar, printf, scanf, sprintf, sscanf
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(void)
{
    /* ---------- 1. Ba dong chuan ---------- */
    printf("--- 1. Ba dong chuan ---\n");
    fprintf(stdout, "Dong nay di ra stdout (co the chuyen huong: ./lab06 > out.txt)\n");
    fprintf(stderr, "Dong nay di ra stderr (luon hien ra man hinh)\n");
    fflush(stdout);           /* day du lieu con trong bo dem ra ngay */

    /* ---------- 2. Cac dinh dang cua printf ---------- */
    printf("\n--- 2. Dinh dang printf ---\n");
    int    so_nguyen = -1234;
    unsigned cong    = 8080;
    double so_thuc   = 3.14159;
    char   ky_tu     = 'K';
    const char *xau  = "IT4062";
    printf("  %%d   -> %d\n", so_nguyen);
    printf("  %%u   -> %u\n", cong);
    printf("  %%5d  -> [%5d]   (can phai trong 5 o)\n", so_nguyen);
    printf("  %%-8s -> [%-8s] (can trai trong 8 o)\n", xau);
    printf("  %%.2f -> %.2f\n", so_thuc);
    printf("  %%c   -> %c\n", ky_tu);
    printf("  %%x   -> %x   |  %%04X -> %04X\n", cong, cong);
    printf("  %%p   -> %p\n", (void *)&so_nguyen);
    printf("  %%zu  -> %zu  (kieu size_t)\n", sizeof(int));

    /* ---------- 3. sprintf / sscanf: printf va scanf tren xau ---------- */
    printf("\n--- 3. sprintf / sscanf ---\n");
    char bo_dem[128];
    /* snprintf an toan hon sprintf vi co gioi han kich thuoc */
    snprintf(bo_dem, sizeof bo_dem, "GET /index.html HTTP/1.0\r\n");
    printf("  Da tao ban tin: %s", bo_dem);

    char phuong_thuc[16], duong_dan[64], phien_ban[16];
    if (sscanf(bo_dem, "%15s %63s %15s", phuong_thuc, duong_dan, phien_ban) == 3) {
        printf("  Tach duoc: phuong thuc=%s | duong dan=%s | phien ban=%s\n",
               phuong_thuc, duong_dan, phien_ban);
    }

    /* ---------- 4. Bao loi: errno + perror + strerror ---------- */
    printf("\n--- 4. Bao loi ---\n");
    FILE *f = fopen("/khong/ton/tai/abc.txt", "r");
    if (f == NULL) {
        printf("  errno         = %d\n", errno);
        printf("  strerror(errno) = %s\n", strerror(errno));
        perror("  fopen");        /* in: "  fopen: No such file or directory" */
    } else {
        fclose(f);
    }

    /* ---------- 5. Doc du lieu tu stdin ---------- */
    printf("\n--- 5. Doc tu ban phim ---\n");
    printf("Nhap ho ten va tuoi (vi du: Ngoc 20): ");

    char dong[128];
    if (fgets(dong, sizeof dong, stdin) == NULL) {   /* fgets an toan hon gets */
        printf("\n(Khong co du lieu vao)\n");
        return 0;
    }
    dong[strcspn(dong, "\r\n")] = '\0';              /* bo ky tu xuong dong */

    char ten[64];
    int tuoi;
    if (sscanf(dong, "%63s %d", ten, &tuoi) == 2)
        printf("  Ten = %s, tuoi = %d, nam sinh khoang %d\n", ten, tuoi, 2026 - tuoi);
    else
        printf("  Khong tach duoc du lieu tu dong: \"%s\"\n", dong);

    /* ---------- 6. getchar / putchar ---------- */
    printf("\n--- 6. getchar / putchar: go mot dong, ket thuc bang Enter ---\n> ");
    int c, dem = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        putchar(c);
        dem++;
    }
    printf("\n  Da doc %d ky tu\n", dem);

    printf("\nGoi y chuyen huong (stream redirection):\n"
           "  ./lab06_io_basic > ketqua.txt   # dua stdout vao tep\n"
           "  ./lab06_io_basic < dulieu.txt   # lay stdin tu tep\n"
           "  ./lab06_io_basic | grep Ten     # noi stdout vao stdin cua lenh khac\n");
    return 0;
}
