/* =====================================================================
 * Lab 01 - Con tro va bieu dien so thuc theo chuan IEEE 754
 * Nguon: Chapter 2, slide 8-10 (Pointer / Exercise)
 *
 * Yeu cau cua bai tap trong slide:
 *   - Nhap vao mot so thuc (float)
 *   - Dung con tro de truy cap vung nho cua no
 *   - In ra 4 byte bieu dien so thuc do
 *   - Kiem chung bang tay voi gia tri don gian (vi du 1.5)
 *
 * Bien dich: gcc -std=gnu11 -Wall -Wextra -g -o lab01_ieee754 lab01_ieee754.c -lm
 * ===================================================================== */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/* In 8 bit cua mot byte, bit cao nhat truoc */
static void in_byte_nhi_phan(unsigned char b)
{
    for (int i = 7; i >= 0; i--)
        putchar(((b >> i) & 1u) ? '1' : '0');
}

/* In 32 bit, tach thanh: dau | so mu (8 bit) | phan dinh tri (23 bit) */
static void in_32_bit(uint32_t x)
{
    for (int i = 31; i >= 0; i--) {
        putchar(((x >> i) & 1u) ? '1' : '0');
        if (i == 31 || i == 23) putchar(' ');
    }
}

int main(void)
{
    float f;

    printf("Nhap mot so thuc (vi du 1.5): ");
    if (scanf("%f", &f) != 1) {
        fprintf(stderr, "Du lieu vao khong hop le\n");
        return 1;
    }

    /* ---- 1. Dung con tro unsigned char* de doc tung byte cua o nho ---- */
    unsigned char *p = (unsigned char *)&f;   /* tro toi byte dau tien cua f */

    printf("\nf             = %g\n", (double)f);
    printf("dia chi &f    = %p\n", (void *)&f);
    printf("sizeof(float) = %zu byte\n\n", sizeof f);

    printf("Cac byte theo dung thu tu trong bo nho (dia chi tang dan):\n");
    for (size_t i = 0; i < sizeof f; i++) {
        printf("   byte[%zu] @ %p = 0x%02X = ", i, (void *)(p + i), p[i]);
        in_byte_nhi_phan(p[i]);
        putchar('\n');
    }

    /* ---- 2. Ghep 4 byte thanh mot so nguyen 32 bit ----
     * Dung memcpy thay vi ep kieu con tro (*(uint32_t*)&f) de tranh
     * vi pham quy tac strict aliasing cua trinh bien dich.            */
    uint32_t bits;
    memcpy(&bits, &f, sizeof bits);

    printf("\nGia tri 32 bit: 0x%08X\n", (unsigned)bits);
    printf("Nhi phan      : ");
    in_32_bit(bits);
    printf("\n");

    /* ---- 3. Tach ba truong theo chuan IEEE 754 (single precision) ---- */
    uint32_t dau      = (bits >> 31) & 0x1u;
    uint32_t so_mu    = (bits >> 23) & 0xFFu;
    uint32_t dinh_tri =  bits        & 0x7FFFFFu;

    printf("\n--- Phan tich theo IEEE 754 ---\n");
    printf("  dau (1 bit)            = %u        -> so %s\n",
           dau, dau ? "am" : "duong");
    printf("  so mu E (8 bit)        = %u (0x%02X) -> e = E - 127 = %d\n",
           so_mu, so_mu, (int)so_mu - 127);
    printf("  phan dinh tri (23 bit) = 0x%06X\n", dinh_tri);

    /* ---- 4. Dung lai gia tri tu ba truong de kiem chung ---- */
    double gia_tri;
    if (so_mu == 0xFFu) {
        printf("  -> gia tri dac biet: %s\n", dinh_tri ? "NaN" : "vo cuc");
        gia_tri = dinh_tri ? NAN : INFINITY;
    } else if (so_mu == 0) {
        /* So khong chuan hoa: 0.M x 2^-126 */
        gia_tri = ldexp((double)dinh_tri / 8388608.0, -126);
        printf("  -> so khong chuan hoa (subnormal)\n");
    } else {
        /* So chuan hoa: 1.M x 2^(E-127) */
        double phan_le = 1.0 + (double)dinh_tri / 8388608.0;
        gia_tri = ldexp(phan_le, (int)so_mu - 127);
        printf("  -> gia tri = 1.%s x 2^%d = %g\n",
               dinh_tri ? "M" : "0", (int)so_mu - 127, phan_le * pow(2.0, (int)so_mu - 127));
    }
    if (dau) gia_tri = -gia_tri;
    printf("  Dung lai tu cac bit  = %g   (goc: %g)\n", gia_tri, (double)f);

    /* ---- 5. Thu tu byte cua may (endianness) ---- */
    uint32_t mau = 0x01020304u;
    unsigned char *m = (unsigned char *)&mau;
    printf("\nMay nay la: %s\n",
           (m[0] == 0x04) ? "little-endian (byte thap o dia chi thap)"
                          : "big-endian (byte cao o dia chi thap)");

    printf("\n[Kiem chung bang tay] 1.5 = 1.1(2) x 2^0\n"
           "   dau = 0, E = 127 = 0x7F, M = 100...0 (23 bit)\n"
           "   => 0 01111111 10000000000000000000000 = 0x3FC00000\n");
    return 0;
}
