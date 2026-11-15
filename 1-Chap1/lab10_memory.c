/* =====================================================================
 * Lab 10 - Thao tac tren vung nho   (slide 24)
 *   memset, memcpy, memmove, memcmp, memchr
 *   str* dung lai o '\0' (van ban); mem* lam viec theo SO BYTE (nhi phan).
 * ===================================================================== */
#include <stdio.h>
#include <string.h>
#include <stddef.h>     /* offsetof */

struct ban_tin { unsigned char ma_lenh; unsigned short do_dai; char noi_dung[16]; };

static void in_hex(const char *nhan, const void *p, size_t n)
{
    const unsigned char *b = p;
    printf("  %-14s: ", nhan);
    for (size_t i = 0; i < n; i++) printf("%02X ", b[i]);
    printf("\n");
}

int main(void)
{
    printf("--- 1. memset / memcpy ---\n");
    char bo_dem[12];
    memset(bo_dem, 0, sizeof bo_dem);        /* xoa sach bo dem */
    char nguon[] = {0x00, 0x01, 'X', 'Y', 0x00, 'Z'};   /* co byte 0 o giua */
    memcpy(bo_dem, nguon, sizeof nguon);
    in_hex("bo dem", bo_dem, sizeof nguon);
    printf("  strlen(nguon) = %zu -> str* dung o byte 0 dau tien,\n"
           "  memcpy chep du %zu byte.\n\n", strlen(nguon), sizeof nguon);

    printf("--- 2. memmove khi hai vung chong nhau ---\n");
    char a[12] = "ABCDEFGHIJ";
    memmove(a + 2, a, 8);                    /* an toan */
    printf("  memmove: %s\n\n", a);

    printf("--- 3. memcmp / memchr ---\n");
    unsigned char k1[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    unsigned char k2[4] = {0xDE, 0xAD, 0x00, 0x01};
    printf("  memcmp(k1,k2,4) = %d, memcmp(k1,k2,2) = %d\n",
           memcmp(k1, k2, 4), memcmp(k1, k2, 2));
    unsigned char *vt = memchr(k1, 0xBE, sizeof k1);
    printf("  memchr tim 0xBE tai vi tri %ld\n\n", (long)(vt - k1));

    printf("--- 4. Struct trong bo nho: byte dem (padding) ---\n");
    struct ban_tin bt;
    memset(&bt, 0, sizeof bt);               /* cach khoi tao struct thong dung */
    bt.ma_lenh = 1;
    bt.do_dai  = 5;
    memcpy(bt.noi_dung, "HELLO", 5);
    printf("  sizeof = %zu, offset: ma_lenh=%zu do_dai=%zu noi_dung=%zu\n",
           sizeof bt, offsetof(struct ban_tin, ma_lenh),
           offsetof(struct ban_tin, do_dai), offsetof(struct ban_tin, noi_dung));
    in_hex("toan bo struct", &bt, sizeof bt);
    printf("  => co byte dem va thu tu byte khac nhau giua cac may,\n"
           "     nen KHONG gui thang struct qua mang.\n");
    return 0;
}
