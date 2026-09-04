/* =====================================================================
 * Lab 03 - Mang cap phat dong: malloc / realloc / free   (slide 12-13)
 * Nhap cac so thuc den khi gap 0, luu vao mang dong, in tong.
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t n = 0, dung_luong = 4;
    float *ds = malloc(dung_luong * sizeof(float));
    if (ds == NULL) { perror("malloc"); return 1; }

    printf("Nhap cac so thuc, nhap 0 de ket thuc:\n");
    for (;;) {
        float x;
        printf("  [%zu] = ", n);
        if (scanf("%f", &x) != 1 || x == 0.0f)
            break;

        if (n == dung_luong) {              /* het cho -> nhan doi bang realloc */
            float *tam = realloc(ds, dung_luong * 2 * sizeof(float));
            if (tam == NULL) { perror("realloc"); free(ds); return 1; }
            ds = tam;
            dung_luong *= 2;
            printf("  (mo rong mang -> %zu phan tu)\n", dung_luong);
        }
        ds[n++] = x;
    }

    double tong = 0.0;
    for (size_t i = 0; i < n; i++)
        tong += ds[i];

    printf("\nDa nhap %zu so: ", n);
    for (size_t i = 0; i < n; i++)
        printf("%g ", (double)ds[i]);
    printf("\nTONG = %g\n", tong);
    if (n > 0)
        printf("Trung binh = %g\n", tong / (double)n);

    free(ds);       /* moi malloc/realloc phai co dung mot free */
    ds = NULL;
    return 0;
}
