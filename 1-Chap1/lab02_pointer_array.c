/* =====================================================================
 * Lab 02 - Con tro, so hoc con tro, quan he con tro / mang  (slide 9, 11)
 * ===================================================================== */
#include <stdio.h>

/* Mang truyen vao ham suy bien thanh con tro -> phai truyen kem do dai */
static int tong_mang(const int *m, size_t n)
{
    int t = 0;
    for (size_t i = 0; i < n; i++)
        t += m[i];              /* m[i] tuong duong *(m + i) */
    return t;
}

int main(void)
{
    int a[5] = {10, 20, 30, 40, 50};
    int *pa = &a[0];            /* hoac viet gon: pa = a */
    int x = *pa;

    printf("--- Con tro va mang ---\n");
    printf("a = %p, &a[0] = %p, *pa = x = %d\n\n", (void *)a, (void *)&a[0], x);

    printf("--- So hoc con tro ---\n");
    printf("pa     -> %d\n", *pa);
    printf("pa + 2 -> %d   (buoc nhay = sizeof(int) = %zu byte)\n\n",
           *(pa + 2), sizeof(int));

    printf("--- Bon cach viet tuong duong ---\n");
    for (size_t i = 0; i < 3; i++)
        printf("  a[%zu]=%d  *(a+%zu)=%d  pa[%zu]=%d  *(pa+%zu)=%d\n",
               i, a[i], i, *(a + i), i, pa[i], i, *(pa + i));

    printf("\n--- sizeof: mang khac con tro ---\n");
    printf("sizeof(a)  = %zu byte (ca mang)\n", sizeof a);
    printf("sizeof(pa) = %zu byte (chi mot con tro)\n\n", sizeof pa);

    printf("Tong = %d\n", tong_mang(a, sizeof a / sizeof a[0]));
    return 0;
}
