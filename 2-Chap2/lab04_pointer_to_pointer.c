/* =====================================================================
 * Lab 04 - Con tro toi con tro (char **, int **)
 * Nguon: Chapter 2, slide 14 (Pointers to pointers)
 *
 *   char ch;      // mot ky tu
 *   char *pch;    // con tro toi ky tu
 *   char **ppch;  // con tro toi con tro toi ky tu
 *   int main(int argc, char **argv)
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Ham cap phat mang qua tham so ra kieu con tro toi con tro.
 * Nho vay ham vua tra ve ma loi, vua "tra ve" ca dia chi mang. */
static int tao_mang(int **ra, size_t n)
{
    int *m = malloc(n * sizeof(int));
    if (m == NULL)
        return -1;
    for (size_t i = 0; i < n; i++)
        m[i] = (int)(i * i);
    *ra = m;            /* ghi dia chi vung nho moi vao bien cua ham goi */
    return 0;
}

/* Nhan ban mot xau (tu cai dat, tuong duong strdup) */
static char *nhan_ban(const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p != NULL)
        memcpy(p, s, n);
    return p;
}

int main(int argc, char **argv)
{
    printf("--- 1. ch / pch / ppch ---\n");
    char ch = 'A';
    char *pch = &ch;
    char **ppch = &pch;
    printf("ch    = %c        (dia chi %p)\n", ch, (void *)&ch);
    printf("*pch  = %c        (pch  = %p)\n", *pch, (void *)pch);
    printf("**ppch= %c        (ppch = %p)\n", **ppch, (void *)ppch);
    **ppch = 'Z';
    printf("Sau **ppch = 'Z' thi ch = %c\n\n", ch);

    printf("--- 2. Tham so dong lenh: argc = %d ---\n", argc);
    for (int i = 0; i < argc; i++)
        printf("  argv[%d] @ %p -> \"%s\"\n", i, (void *)argv[i], argv[i]);
    printf("  argv[%d] = %p (NULL ket thuc danh sach)\n\n",
           argc, (void *)argv[argc]);

    printf("--- 3. Con tro toi con tro lam tham so ra ---\n");
    int *m = NULL;
    if (tao_mang(&m, 6) == 0) {         /* truyen DIA CHI cua con tro m */
        printf("  m = ");
        for (size_t i = 0; i < 6; i++) printf("%d ", m[i]);
        printf("\n");
        free(m);
    }

    printf("\n--- 4. Mang cac xau (char **) cap phat dong ---\n");
    const char *nguon[] = {"LOGIN", "LIST", "RETR", "QUIT"};
    size_t so_lenh = sizeof nguon / sizeof nguon[0];

    char **lenh = malloc(so_lenh * sizeof(char *));
    if (lenh == NULL) { perror("malloc"); return 1; }

    for (size_t i = 0; i < so_lenh; i++) {
        lenh[i] = nhan_ban(nguon[i]);
        if (lenh[i] == NULL) { perror("malloc"); return 1; }
    }
    for (size_t i = 0; i < so_lenh; i++)
        printf("  lenh[%zu] @ %p -> \"%s\"\n", i, (void *)lenh[i], lenh[i]);

    /* Giai phong: tung phan tu truoc, roi den mang con tro */
    for (size_t i = 0; i < so_lenh; i++)
        free(lenh[i]);
    free(lenh);
    return 0;
}
