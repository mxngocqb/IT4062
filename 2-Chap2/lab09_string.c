/* =====================================================================
 * Lab 09 - Xu ly xau ky tu
 * Nguon: Chapter 2, slide 22-23 (String Handling / Searching)
 *
 *   strlen, strcpy, strncpy, strcat, strncat, strcmp, strncmp,
 *   strcasecmp, strncasecmp, strchr, strrchr, strstr, strpbrk,
 *   strspn, strcspn, strtok, strtok_r, strerror
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>    /* strcasecmp, strncasecmp */
#include <errno.h>

/* Sao chep an toan: luon ket thuc xau, khong bao gio tran bo dem */
static void sao_chep_an_toan(char *dich, size_t co, const char *nguon)
{
    if (co == 0) return;
    strncpy(dich, nguon, co - 1);
    dich[co - 1] = '\0';        /* strncpy KHONG tu them ky tu ket thuc */
}

int main(void)
{
    /* ---------- 1. Do dai, sao chep, noi xau ---------- */
    printf("--- 1. Do dai / sao chep / noi ---\n");
    char s1[64] = "IT4062";
    char s2[]   = " - Lap trinh mang";

    printf("  strlen(s1) = %zu   sizeof(s1) = %zu\n", strlen(s1), sizeof s1);
    strcat(s1, s2);                       /* noi s2 vao cuoi s1 */
    printf("  sau strcat : \"%s\"\n", s1);

    char nho[8];
    sao_chep_an_toan(nho, sizeof nho, "chuoi rat dai bi cat bot");
    printf("  cat bot vao bo dem 8 byte: \"%s\" (do dai %zu)\n", nho, strlen(nho));

    char noi[16] = "abc";
    strncat(noi, "defghijklmnop", sizeof noi - strlen(noi) - 1);
    printf("  strncat co gioi han       : \"%s\"\n\n", noi);

    /* ---------- 2. So sanh ---------- */
    printf("--- 2. So sanh ---\n");
    printf("  strcmp(\"abc\",\"abd\")      = %d (< 0: abc dung truoc)\n",
           strcmp("abc", "abd"));
    printf("  strcmp(\"LOGIN\",\"LOGIN\")  = %d (= 0: bang nhau)\n",
           strcmp("LOGIN", "LOGIN"));
    printf("  strncmp(\"LOGIN abc\",\"LOGIN\",5) = %d\n",
           strncmp("LOGIN abc", "LOGIN", 5));
    printf("  strcasecmp(\"quit\",\"QUIT\") = %d (khong phan biet hoa thuong)\n",
           strcasecmp("quit", "QUIT"));
    printf("  strncasecmp(\"User: a\",\"user\",4) = %d\n\n",
           strncasecmp("User: a", "user", 4));

    /* ---------- 3. Tim kiem ---------- */
    printf("--- 3. Tim kiem ---\n");
    const char *http = "GET /tin-tuc/index.html HTTP/1.1\r\nHost: hust.edu.vn\r\n\r\n";

    const char *p;
    p = strchr(http, '/');
    printf("  strchr(s,'/')   -> vi tri %ld: \"%.20s...\"\n", (long)(p - http), p);
    p = strrchr(http, '/');
    printf("  strrchr(s,'/')  -> vi tri %ld: \"%.20s...\"\n", (long)(p - http), p);
    p = strstr(http, "Host:");
    printf("  strstr(s,\"Host:\") -> vi tri %ld: \"%.18s\"\n", (long)(p - http), p);
    p = strstr(http, "\r\n\r\n");
    printf("  ranh gioi header cua HTTP o vi tri %ld\n", (long)(p - http));

    const char *dong = "LOGIN;user,pass";
    p = strpbrk(dong, ";,");
    printf("  strpbrk(s,\";,\") -> ky tu phan cach dau tien: '%c' tai %ld\n",
           *p, (long)(p - dong));
    printf("  strspn(\"   abc\",\" \")  = %zu (so ky tu trang o dau)\n",
           strspn("   abc", " "));
    printf("  strcspn(\"LOGIN abc\",\" \") = %zu (do dai tu dau tien)\n\n",
           strcspn("LOGIN abc", " "));

    /* ---------- 4. Cat ky tu xuong dong (rat hay dung khi doc mang) ---------- */
    printf("--- 4. Cat \\r\\n cuoi dong ---\n");
    char ban_tin[64] = "USER sinhvien\r\n";
    printf("  truoc: do dai %zu\n", strlen(ban_tin));
    ban_tin[strcspn(ban_tin, "\r\n")] = '\0';
    printf("  sau  : do dai %zu -> \"%s\"\n\n", strlen(ban_tin), ban_tin);

    /* ---------- 5. Tach xau bang strtok_r ---------- */
    printf("--- 5. Tach ban tin bang strtok_r ---\n");
    char lenh[] = "LOGIN sinhvien matkhau123";
    char *luu = NULL;                     /* bien trang thai cua strtok_r */
    char *tu = strtok_r(lenh, " ", &luu);
    int i = 0;
    while (tu != NULL) {
        printf("  tu[%d] = \"%s\"\n", i++, tu);
        tu = strtok_r(NULL, " ", &luu);   /* lan sau truyen NULL */
    }
    printf("  (strtok_r an toan voi da luong; strtok thi khong)\n\n");

    /* ---------- 6. Tach bang sscanf ---------- */
    printf("--- 6. Tach bang sscanf ---\n");
    char ma_lenh[16], ten[32], mat_khau[32];
    if (sscanf("LOGIN sinhvien matkhau123", "%15s %31s %31s",
               ma_lenh, ten, mat_khau) == 3)
        printf("  lenh=%s | ten=%s | mat khau=%s\n\n", ma_lenh, ten, mat_khau);

    /* ---------- 7. strerror ---------- */
    printf("--- 7. strerror ---\n");
    FILE *f = fopen("khong_ton_tai.txt", "r");
    if (f == NULL)
        printf("  errno=%d -> \"%s\"\n", errno, strerror(errno));
    else
        fclose(f);

    /* ---------- 8. Vi du xau KHONG an toan ---------- */
    printf("\n--- 8. Vi du nguy hiem (chi de doc, khong chay) ---\n");
    printf("  char ten[16];\n"
           "  strcpy(ten, du_lieu_tu_mang);   // TRAN BO DEM neu dai > 15\n"
           "  => luon dung snprintf() hoac strncpy() + tu them '\\0'\n");
    return 0;
}
