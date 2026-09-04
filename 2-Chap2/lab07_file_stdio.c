/* =====================================================================
 * Lab 07 - Lam viec voi tep qua FILE* (vao/ra co bo dem)
 * Nguon: Chapter 2, slide 19-20 (Files / Stream Status Enquiries)
 *
 *   fopen, fprintf, fscanf, fgetc, fputc, feof, ferror, fflush, fclose
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEP_DL   "sinhvien.txt"
#define TEP_SAO  "sinhvien_ban_sao.txt"

struct sinh_vien {
    char  mssv[16];
    char  ten[32];
    float diem;
};

int main(void)
{
    struct sinh_vien ds[] = {
        {"20250001", "Nguyen_Van_A", 8.5f},
        {"20250002", "Tran_Thi_B",   9.0f},
        {"20250003", "Le_Van_C",     7.25f},
    };
    size_t n = sizeof ds / sizeof ds[0];

    /* ---------- 1. Ghi tep van ban bang fprintf ---------- */
    printf("--- 1. Ghi tep %s ---\n", TEP_DL);
    FILE *f = fopen(TEP_DL, "w");        /* "w": tao moi / ghi de */
    if (f == NULL) {
        perror("fopen ghi");
        return 1;
    }
    for (size_t i = 0; i < n; i++)
        fprintf(f, "%s %s %.2f\n", ds[i].mssv, ds[i].ten, (double)ds[i].diem);
    fflush(f);                            /* day bo dem xuong dia */
    fclose(f);
    printf("  Da ghi %zu ban ghi\n", n);

    /* ---------- 2. Ghi them bang che do "a" ---------- */
    f = fopen(TEP_DL, "a");               /* "a": ghi noi vao cuoi */
    if (f != NULL) {
        fprintf(f, "%s %s %.2f\n", "20250004", "Pham_Thi_D", 6.75);
        fclose(f);
        printf("  Da ghi noi them 1 ban ghi\n");
    }

    /* ---------- 3. Doc lai bang fscanf ---------- */
    printf("\n--- 2. Doc lai bang fscanf ---\n");
    f = fopen(TEP_DL, "r");
    if (f == NULL) {
        perror("fopen doc");
        return 1;
    }
    struct sinh_vien sv;
    double tong = 0.0;
    int dem = 0;
    while (fscanf(f, "%15s %31s %f", sv.mssv, sv.ten, &sv.diem) == 3) {
        printf("  %-10s %-14s %.2f\n", sv.mssv, sv.ten, (double)sv.diem);
        tong += sv.diem;
        dem++;
    }
    if (ferror(f))                        /* phan biet loi doc voi het tep */
        perror("  loi khi doc");
    else if (feof(f))
        printf("  (da doc het tep)\n");
    fclose(f);
    if (dem > 0)
        printf("  Diem trung binh = %.2f\n", tong / dem);

    /* ---------- 4. Sao chep tep bang fgetc / fputc ---------- */
    printf("\n--- 3. Sao chep tep bang fgetc/fputc ---\n");
    FILE *nguon = fopen(TEP_DL, "r");
    FILE *dich  = fopen(TEP_SAO, "w");
    if (nguon == NULL || dich == NULL) {
        perror("fopen sao chep");
        if (nguon) fclose(nguon);
        if (dich)  fclose(dich);
        return 1;
    }
    int c;
    long so_byte = 0;
    while ((c = fgetc(nguon)) != EOF) {
        fputc(c, dich);
        so_byte++;
    }
    printf("  Da sao chep %ld byte sang %s\n", so_byte, TEP_SAO);
    fclose(nguon);
    fclose(dich);

    /* ---------- 5. Doc tung dong bang fgets ---------- */
    printf("\n--- 4. Doc tung dong bang fgets ---\n");
    f = fopen(TEP_SAO, "r");
    if (f != NULL) {
        char dong[128];
        int stt = 0;
        while (fgets(dong, sizeof dong, f) != NULL) {
            dong[strcspn(dong, "\r\n")] = '\0';
            printf("  dong %d: %s\n", ++stt, dong);
        }
        fclose(f);
    }

    printf("\nHai tep da tao: %s, %s (xoa bang: rm %s %s)\n",
           TEP_DL, TEP_SAO, TEP_DL, TEP_SAO);
    return 0;
}
