/* =====================================================================
 * Lab 09 - Xu ly xau ky tu   (slide 22-23)
 * ===================================================================== */
#include <stdio.h>
#include <string.h>
#include <strings.h>    /* strcasecmp */

/* Sao chep an toan: luon ket thuc xau, khong bao gio tran bo dem */
static void sao_chep_an_toan(char *dich, size_t co, const char *nguon)
{
    if (co == 0) return;
    strncpy(dich, nguon, co - 1);
    dich[co - 1] = '\0';        /* strncpy KHONG tu them ky tu ket thuc */
}

int main(void)
{
    printf("--- 1. Do dai / noi / sao chep ---\n");
    char s[64] = "IT4062";
    printf("  strlen = %zu, sizeof = %zu\n", strlen(s), sizeof s);
    strcat(s, " - Lap trinh mang");
    printf("  sau strcat: \"%s\"\n", s);

    char nho[8];
    sao_chep_an_toan(nho, sizeof nho, "chuoi rat dai bi cat bot");
    printf("  cat vao bo dem 8 byte: \"%s\"\n\n", nho);

    printf("--- 2. So sanh ---\n");
    printf("  strcmp(\"abc\",\"abd\")     = %d\n", strcmp("abc", "abd"));
    printf("  strncmp(\"LOGIN a\",\"LOGIN\",5) = %d\n", strncmp("LOGIN a", "LOGIN", 5));
    printf("  strcasecmp(\"quit\",\"QUIT\") = %d (khong phan biet hoa thuong)\n\n",
           strcasecmp("quit", "QUIT"));

    printf("--- 3. Tim kiem ---\n");
    const char *http = "GET /index.html HTTP/1.1\r\nHost: hust.edu.vn\r\n\r\n";
    printf("  strchr(s,'/')    tai %ld\n", (long)(strchr(http, '/') - http));
    printf("  strstr(s,\"Host\") tai %ld\n", (long)(strstr(http, "Host") - http));
    printf("  ranh gioi header \\r\\n\\r\\n tai %ld\n\n",
           (long)(strstr(http, "\r\n\r\n") - http));

    printf("--- 4. Cat \\r\\n cuoi dong (hay dung khi doc tu mang) ---\n");
    char bt[64] = "USER sinhvien\r\n";
    bt[strcspn(bt, "\r\n")] = '\0';
    printf("  \"%s\" (do dai %zu)\n\n", bt, strlen(bt));

    printf("--- 5. Tach ban tin bang strtok_r ---\n");
    char lenh[] = "LOGIN sinhvien matkhau";
    char *luu = NULL;
    for (char *tu = strtok_r(lenh, " ", &luu); tu; tu = strtok_r(NULL, " ", &luu))
        printf("  [%s]\n", tu);
    printf("  (strtok_r an toan voi da luong, strtok thi khong)\n");
    return 0;
}
