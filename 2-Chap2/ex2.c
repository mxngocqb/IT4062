#include <stdio.h>
int main()
{
    int a[10] = {0x00000001, 0x0000C03F, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000008, 0x00000009, 0x0000000A};
    int* p = &a[0];
    int y = *p;
    p += 1;
    float* q = (float*)p;
    float z = *((float*)q);
    
    unsigned char* k = (unsigned char*)&z;
    printf("0x%02X 0x%02X 0x%02X 0x%02X\n", (unsigned char)k[0], (unsigned char)k[1], (unsigned char)k[2], (unsigned char)k[3]);

    printf("\n");
}