/* =====================================================================
 * Lab 01 - Con tro va bieu dien so thuc IEEE 754   (slide 8-10)
 * Nhap mot so thuc, dung con tro doc 4 byte cua no, phan tich cac truong.
 * ===================================================================== */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

static void in_nhi_phan(uint32_t x, int so_bit)
{
    for (int i = so_bit - 1; i >= 0; i--)
        putchar(((x >> i) & 1u) ? '1' : '0');
}

int main(void)
{
    float f;
    printf("Nhap mot so thuc (vi du 1.5): ");
    if (scanf("%f", &f) != 1) {
        fprintf(stderr, "Du lieu vao khong hop le\n");
        return 1;
    }

    /* 1. Con tro unsigned char* de doc tung byte trong o nho */
    unsigned char *p = (unsigned char *)&f;
    printf("\n4 byte cua f (dia chi tang dan):\n");
    for (size_t i = 0; i < sizeof f; i++) {
        printf("  byte[%zu] = 0x%02X = ", i, p[i]);
        in_nhi_phan(p[i], 8);
        putchar('\n');
    }

    /* 2. Ghep 4 byte thanh so 32 bit (memcpy de khong vi pham strict aliasing) */
    uint32_t bits;
    memcpy(&bits, &f, sizeof bits);
    printf("\n0x%08X = ", bits);
    in_nhi_phan(bits >> 31, 1);  putchar(' ');
    in_nhi_phan(bits >> 23, 8);  putchar(' ');
    in_nhi_phan(bits, 23);       putchar('\n');

    /* 3. Tach ba truong theo chuan IEEE 754 */
    uint32_t dau = bits >> 31, so_mu = (bits >> 23) & 0xFFu, dinh_tri = bits & 0x7FFFFFu;
    printf("  dau       = %u (%s)\n", dau, dau ? "am" : "duong");
    printf("  so mu E   = %u -> e = E - 127 = %d\n", so_mu, (int)so_mu - 127);
    printf("  dinh tri  = 0x%06X\n", dinh_tri);

    /* 4. Dung lai gia tri de kiem chung */
    double gt;
    if (so_mu == 0xFFu)     gt = dinh_tri ? NAN : INFINITY;
    else if (so_mu == 0)    gt = ldexp((double)dinh_tri / 8388608.0, -126);
    else                    gt = ldexp(1.0 + (double)dinh_tri / 8388608.0, (int)so_mu - 127);
    printf("  dung lai  = %g   (goc: %g)\n", dau ? -gt : gt, (double)f);

    printf("\n[Kiem chung tay] 1.5 = 1.1(2) x 2^0 -> 0 01111111 100...0 = 0x3FC00000\n");
    return 0;
}
