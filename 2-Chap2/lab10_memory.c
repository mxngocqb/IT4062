/* =====================================================================
 * Lab 10 - Cac thao tac tren vung nho
 * Nguon: Chapter 2, slide 24 (Memory Operations)
 *
 *   memchr, memcmp, memcpy, memmove, memset
 *
 * Khac biet quan trong: nhom str* dung lai o ky tu '\0' nen chi dung cho
 * van ban; nhom mem* lam viec theo SO BYTE nen dung duoc cho du lieu nhi
 * phan -- dung cai ta gap khi nhan du lieu tu socket.
 * ===================================================================== */
#include <stdio.h>
#include <string.h>
#include <stddef.h>     /* offsetof */

struct ban_tin {
    unsigned char  ma_lenh;
    unsigned short do_dai;
    char           noi_dung[16];
};

static void in_hex(const char *nhan, const void *p, size_t n)
{
    const unsigned char *b = p;
    printf("  %-12s: ", nhan);
    for (size_t i = 0; i < n; i++)
        printf("%02X ", b[i]);
    printf("\n");
}

int main(void)
{
    /* ---------- 1. memset ---------- */
    printf("--- 1. memset: gan n byte cung mot gia tri ---\n");
    char bo_dem[16];
    memset(bo_dem, 0, sizeof bo_dem);          /* xoa sach bo dem */
    in_hex("sau memset 0", bo_dem, sizeof bo_dem);
    memset(bo_dem, 'A', 8);
    in_hex("gan 8 byte A", bo_dem, sizeof bo_dem);

    /* ---------- 2. memcpy ---------- */
    printf("\n--- 2. memcpy: sao chep n byte ---\n");
    char nguon[] = {0x00, 0x01, 'X', 'Y', 0x00, 'Z'};   /* co byte 0 o giua */
    char dich[8];
    memset(dich, 0, sizeof dich);
    memcpy(dich, nguon, sizeof nguon);
    in_hex("nguon", nguon, sizeof nguon);
    in_hex("dich", dich, sizeof nguon);
    printf("  strlen(nguon) = %zu -> str* se DUNG o byte 0 dau tien,\n"
           "  memcpy thi chep du %zu byte.\n", strlen(nguon), sizeof nguon);

    /* ---------- 3. memmove: an toan khi hai vung chong nhau ---------- */
    printf("\n--- 3. memcpy vs memmove khi vung nho chong nhau ---\n");
    char a[12] = "ABCDEFGHIJ";
    char b[12] = "ABCDEFGHIJ";
    memmove(a + 2, a, 8);          /* dich phai 2 vi tri - dung memmove */
    printf("  memmove: %s\n", a);
    memcpy(b + 2, b, 8);           /* ket qua khong xac dinh - CHI de minh hoa */
    printf("  memcpy : %s  <- ket qua khong bao dam\n", b);

    /* ---------- 4. memcmp ---------- */
    printf("\n--- 4. memcmp: so sanh n byte ---\n");
    unsigned char k1[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    unsigned char k2[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    unsigned char k3[4] = {0xDE, 0xAD, 0x00, 0x01};
    printf("  memcmp(k1,k2,4) = %d (bang nhau)\n", memcmp(k1, k2, 4));
    printf("  memcmp(k1,k3,4) = %d (khac nhau)\n", memcmp(k1, k3, 4));
    printf("  memcmp(k1,k3,2) = %d (2 byte dau van bang nhau)\n", memcmp(k1, k3, 2));

    /* ---------- 5. memchr ---------- */
    printf("\n--- 5. memchr: tim mot byte trong vung nho ---\n");
    unsigned char goi_tin[] = {0x01, 0x02, 0x0A, 0x03, 0x0A, 0x04};
    unsigned char *vt = memchr(goi_tin, 0x0A, sizeof goi_tin);
    if (vt != NULL)
        printf("  Tim thay byte 0x0A tai vi tri %ld\n", (long)(vt - goi_tin));

    /* ---------- 6. Struct trong bo nho: padding ---------- */
    printf("\n--- 6. Cau truc trong bo nho ---\n");
    struct ban_tin bt;
    memset(&bt, 0, sizeof bt);         /* cach khoi tao struct thong dung */
    bt.ma_lenh = 0x01;
    bt.do_dai  = 5;
    memcpy(bt.noi_dung, "HELLO", 5);

    printf("  sizeof(struct ban_tin) = %zu byte\n", sizeof bt);
    printf("  offset ma_lenh  = %zu\n", offsetof(struct ban_tin, ma_lenh));
    printf("  offset do_dai   = %zu  <- co byte dem truoc truong nay\n",
           offsetof(struct ban_tin, do_dai));
    printf("  offset noi_dung = %zu\n", offsetof(struct ban_tin, noi_dung));
    in_hex("toan bo struct", &bt, sizeof bt);
    printf("  => vi co byte dem va thu tu byte khac nhau giua cac may,\n"
           "     KHONG gui thang struct qua mang; phai tuan tu hoa tung truong.\n");
    return 0;
}
