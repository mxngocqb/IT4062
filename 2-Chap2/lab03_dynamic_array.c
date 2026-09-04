/* =====================================================================
 * Lab 03 - Cap phat bo nho dong: malloc / calloc / realloc / free
 * Nguon: Chapter 2, slide 12-13 (Dynamic Memory Allocation / Exercise)
 *
 * Yeu cau cua bai tap trong slide:
 *   - Nhap cac so thuc tu stdin cho toi khi nhap so 0
 *   - Luu tat ca gia tri vao mot mang cap phat dong (malloc/calloc/realloc)
 *   - In ra tong cua cac so do
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DUNG_LUONG_DAU 4

int main(void)
{
    size_t n = 0;                       /* so phan tu dang co */
    size_t dung_luong = DUNG_LUONG_DAU; /* so phan tu toi da hien tai */

    /* malloc: cap phat, KHONG khoi tao gia tri */
    float *ds = malloc(dung_luong * sizeof(float));
    if (ds == NULL) {                   /* luon kiem tra ket qua cap phat */
        perror("malloc");
        return 1;
    }

    printf("Nhap cac so thuc, nhap 0 de ket thuc:\n");
    for (;;) {
        float x;
        printf("  [%zu] = ", n);
        if (scanf("%f", &x) != 1) {
            printf("\n(Het du lieu vao)\n");
            break;
        }
        if (x == 0.0f)
            break;

        /* Het cho -> nhan doi dung luong bang realloc */
        if (n == dung_luong) {
            size_t moi = dung_luong * 2;
            float *tam = realloc(ds, moi * sizeof
                (float));
            if (tam == NULL) {          /* realloc that bai: ds cu VAN con */
                perror("realloc");
                free(ds);
                return 1;
            }
            ds = tam;
            printf("  (mo rong mang: %zu -> %zu phan tu)\n", dung_luong, moi);
            dung_luong = moi;
        }
        ds[n++] = x;
    }

    if (n == 0) {
        printf("Khong co so nao duoc nhap.\n");
        free(ds);
        return 0;
    }

    /* Tinh toan tren mang dong */
    double tong = 0.0;
    float nho_nhat = ds[0], lon_nhat = ds[0];
    for (size_t i = 0; i < n; i++) {
        tong += ds[i];
        if (ds[i] < nho_nhat) nho_nhat = ds[i];
        if (ds[i] > lon_nhat) lon_nhat = ds[i];
    }

    printf("\nDa nhap %zu so (dung luong mang: %zu phan tu = %zu byte)\n",
           n, dung_luong, dung_luong * sizeof(float));
    printf("Cac gia tri: ");
    for (size_t i = 0; i < n; i++)
        printf("%g ", (double)ds[i]);
    printf("\nTONG        = %g\n", tong);
    printf("Trung binh  = %g\n", tong / (double)n);
    printf("Nho nhat    = %g\n", (double)nho_nhat);
    printf("Lon nhat    = %g\n", (double)lon_nhat);

    /* ---- Minh hoa calloc: cap phat va gan 0 san ---- */
    int *dem = calloc(5, sizeof(int));
    if (dem != NULL) {
        printf("\ncalloc(5, sizeof(int)) -> ");
        for (int i = 0; i < 5; i++) printf("%d ", dem[i]);   /* deu bang 0 */
        printf("(da duoc gan 0 san)\n");
        free(dem);
    }

    /* ---- Minh hoa memset tren vung nho dong ---- */
    memset(ds, 0, n * sizeof(float));
    printf("Sau memset, ds[0] = %g\n", (double)ds[0]);

    free(ds);     /* moi malloc/calloc/realloc phai co dung mot free */
    ds = NULL;    /* tranh con tro "treo" (dangling pointer) */
    return 0;
}
