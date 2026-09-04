/* =====================================================================
 * Lab 02 - Con tro, so hoc con tro va quan he con tro / mang
 * Nguon: Chapter 2, slide 8-9-11
 *
 *   int a[10], x;
 *   int *pa;
 *   pa = &a[0];      // pa tro toi dia chi cua a[0]
 *   x  = *pa;        // x = noi dung tai pa (chinh la a[0])
 * ===================================================================== */
#include <stdio.h>

/* Mang truyen vao ham se "suy bien" thanh con tro
 * => ben trong ham khong con biet do dai, phai truyen kem n */
static int tong_mang(const int *m, size_t n)
{
    int t = 0;
    for (size_t i = 0; i < n; i++)
        t += m[i];              /* m[i] tuong duong *(m + i) */
    return t;
}

/* Dao nguoc mang chi bang con tro, khong dung chi so */
static void dao_mang(int *dau, size_t n)
{
    int *cuoi = dau + n - 1;
    while (dau < cuoi) {
        int tam = *dau;
        *dau = *cuoi;
        *cuoi = tam;
        dau++;
        cuoi--;
    }
}

int main(void)
{
    int a[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int x;
    int *pa;

    pa = &a[0];      /* hoac viet gon: pa = a; */
    x  = *pa;

    printf("--- 1. Con tro va mang ---\n");
    printf("a       = %p   (ten mang chinh la dia chi phan tu dau)\n", (void *)a);
    printf("&a[0]   = %p\n", (void *)&a[0]);
    printf("pa      = %p\n", (void *)pa);
    printf("*pa = x = %d\n\n", x);

    printf("--- 2. So hoc con tro ---\n");
    printf("pa     = %p  -> *pa     = %d\n", (void *)pa, *pa);
    printf("pa + 1 = %p  -> *(pa+1) = %d\n", (void *)(pa + 1), *(pa + 1));
    printf("Khoang cach = %ld byte = sizeof(int) = %zu\n\n",
           (long)((char *)(pa + 1) - (char *)pa), sizeof(int));

    printf("--- 3. Bon cach viet tuong duong ---\n");
    for (size_t i = 0; i < 4; i++)
        printf("  a[%zu]=%d   *(a+%zu)=%d   pa[%zu]=%d   *(pa+%zu)=%d\n",
               i, a[i], i, *(a + i), i, pa[i], i, *(pa + i));
    printf("\n");

    printf("--- 4. sizeof: mang khac con tro ---\n");
    printf("sizeof(a)  = %zu byte (ca mang, %zu phan tu)\n",
           sizeof a, sizeof a / sizeof a[0]);
    printf("sizeof(pa) = %zu byte (chi la mot con tro)\n\n", sizeof pa);

    printf("--- 5. Truyen mang cho ham ---\n");
    printf("Tong = %d\n", tong_mang(a, sizeof a / sizeof a[0]));

    dao_mang(a, sizeof a / sizeof a[0]);
    printf("Sau khi dao: ");
    for (size_t i = 0; i < sizeof a / sizeof a[0]; i++)
        printf("%d ", a[i]);
    printf("\n\n");

    printf("--- 6. Buoc nhay phu thuoc kieu du lieu ---\n");
    char   c[3] = {'A', 'B', 'C'};
    double d[3] = {1.0, 2.0, 3.0};
    printf("char  : c   = %p, c+1 = %p  (buoc %zu byte)\n",
           (void *)c, (void *)(c + 1), sizeof(char));
    printf("double: d   = %p, d+1 = %p  (buoc %zu byte)\n",
           (void *)d, (void *)(d + 1), sizeof(double));
    return 0;
}
