/* =====================================================================
 * Lab 06 - Vao/ra chuan, bao loi   (slide 16-18)
 *   stdin / stdout / stderr, printf, scanf, sprintf, sscanf, perror, errno
 * ===================================================================== */
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void)
{
    printf("--- 1. Ba dong chuan ---\n");
    fprintf(stdout, "stdout: co the chuyen huong  ./lab06 > out.txt\n");
    fprintf(stderr, "stderr: luon hien ra man hinh\n\n");

    printf("--- 2. Dinh dang printf ---\n");
    printf("  %%d=%d  %%u=%u  %%5d=[%5d]  %%-8s=[%-8s]\n", -12, 8080u, 42, "IT4062");
    printf("  %%.2f=%.2f  %%c=%c  %%04X=%04X  %%zu=%zu\n", 3.14159, 'K', 8080, sizeof(int));

    printf("\n--- 3. sprintf / sscanf ---\n");
    char bo_dem[128];
    snprintf(bo_dem, sizeof bo_dem, "GET /index.html HTTP/1.0");  /* an toan hon sprintf */
    char pt[16], dd[64], pb[16];
    if (sscanf(bo_dem, "%15s %63s %15s", pt, dd, pb) == 3)
        printf("  \"%s\"\n  -> %s | %s | %s\n", bo_dem, pt, dd, pb);

    printf("\n--- 4. Bao loi: errno / perror ---\n");
    FILE *f = fopen("/khong/ton/tai.txt", "r");
    if (f == NULL) {
        printf("  errno = %d -> %s\n", errno, strerror(errno));
        perror("  fopen");
    } else fclose(f);

    printf("\n--- 5. Doc tu ban phim ---\n");
    printf("Nhap ho ten va tuoi (vi du: Ngoc 20): ");
    char dong[128];
    if (fgets(dong, sizeof dong, stdin) == NULL)   /* fgets an toan hon gets */
        return 0;
    dong[strcspn(dong, "\r\n")] = '\0';            /* bo ky tu xuong dong */

    char ten[64];
    int tuoi;
    if (sscanf(dong, "%63s %d", ten, &tuoi) == 2)
        printf("  ten = %s, tuoi = %d\n", ten, tuoi);
    else
        printf("  khong tach duoc: \"%s\"\n", dong);
    return 0;
}
