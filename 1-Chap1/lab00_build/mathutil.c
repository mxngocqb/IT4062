/* Lab 00b - Cai dat cac ham; dung <math.h> nen phai lien ket voi -lm */
#include <math.h>
#include "mathutil.h"

double chu_vi_hinh_tron(double ban_kinh)
{
    return 2.0 * M_PI * ban_kinh;
}

double khoang_cach(double x1, double y1, double x2, double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
