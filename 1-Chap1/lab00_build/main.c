/* Lab 00b - Tep chinh cua chuong trinh nhieu tep nguon
 *
 * Ba cach bien dich (slide 5):
 *   gcc main.c mathutil.c -o calc -lm
 *   gcc -c main.c ; gcc -c mathutil.c ; gcc main.o mathutil.o -o calc -lm
 *   make
 */
#include <stdio.h>
#include "mathutil.h"

int main(void)
{
    double r = 2.5;
    printf("Chu vi hinh tron ban kinh %.2f = %.4f\n", r, chu_vi_hinh_tron(r));
    printf("Khoang cach (0,0)-(3,4)       = %.4f\n", khoang_cach(0, 0, 3, 4));
    return 0;
}
