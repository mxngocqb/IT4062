/* =====================================================================
 * Lab 04 - Con tro toi con tro (char **, int **)   (slide 14)
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cap phat mang qua tham so ra kieu con tro toi con tro */
static int tao_mang(int **ra, size_t n)
{
    int *m = malloc(n * sizeof(int));
    if (m == NULL) return -1;
    for (size_t i = 0; i < n; i++) m[i] = (int)(i * i);
    *ra = m;                    /* ghi dia chi vung nho vao bien cua ham goi */
    return 0;
}

int main(int argc, char **argv)
{
    printf("--- 1. ch / pch / ppch ---\n");
    char ch = 'A';
    char *pch = &ch;
    char **ppch = &pch;
    printf("ch = %c, *pch = %c, **ppch = %c\n", ch, *pch, **ppch);
    **ppch = 'Z';
    printf("sau **ppch = 'Z' thi ch = %c\n\n", ch);

    printf("--- 2. Tham so dong lenh (argc = %d) ---\n", argc);
    for (int i = 0; i < argc; i++)
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);
    printf("  argv[%d] = NULL (ket thuc danh sach)\n\n", argc);

    printf("--- 3. Con tro toi con tro lam tham so ra ---\n");
    int *m = NULL;
    if (tao_mang(&m, 6) == 0) {          /* truyen DIA CHI cua con tro m */
        printf("  m = ");
        for (size_t i = 0; i < 6; i++) printf("%d ", m[i]);
        printf("\n");
        free(m);
    }
    return 0;
}
