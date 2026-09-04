/* =====================================================================
 * Lab 07 - Tep qua FILE* (vao/ra co bo dem)   (slide 19-20)
 *   fopen, fprintf, fscanf, fgetc, fputc, feof, ferror, fclose
 * ===================================================================== */
#include <stdio.h>

#define TEP "sinhvien.txt"
#define SAO "sinhvien_ban_sao.txt"

int main(void)
{
    /* 1. Ghi tep van ban */
    FILE *f = fopen(TEP, "w");              /* "w" tao moi, "a" ghi noi, "r" doc */
    if (f == NULL) { perror("fopen ghi"); return 1; }
    fprintf(f, "20250001 Nguyen_Van_A 8.50\n");
    fprintf(f, "20250002 Tran_Thi_B 9.00\n");
    fprintf(f, "20250003 Le_Van_C 7.25\n");
    fclose(f);
    printf("Da ghi %s\n\n", TEP);

    /* 2. Doc lai bang fscanf */
    f = fopen(TEP, "r");
    if (f == NULL) { perror("fopen doc"); return 1; }
    char mssv[16], ten[32];
    float diem;
    double tong = 0.0;
    int dem = 0;
    while (fscanf(f, "%15s %31s %f", mssv, ten, &diem) == 3) {
        printf("  %-10s %-14s %.2f\n", mssv, ten, (double)diem);
        tong += diem;
        dem++;
    }
    if (ferror(f)) perror("  loi doc");     /* phan biet loi doc voi het tep */
    else if (feof(f)) printf("  (het tep)\n");
    fclose(f);
    if (dem) printf("  Trung binh = %.2f\n\n", tong / dem);

    /* 3. Sao chep tep bang fgetc / fputc */
    FILE *nguon = fopen(TEP, "r");
    FILE *dich  = fopen(SAO, "w");
    if (nguon == NULL || dich == NULL) { perror("fopen sao chep"); return 1; }
    int c;
    long so_byte = 0;
    while ((c = fgetc(nguon)) != EOF) { fputc(c, dich); so_byte++; }
    fclose(nguon);
    fclose(dich);
    printf("Da sao chep %ld byte sang %s\n", so_byte, SAO);
    return 0;
}
