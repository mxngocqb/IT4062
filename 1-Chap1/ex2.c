#include <stdio.h>
int main()
{
    int a[10] = 
            {0x00000001, 
            0x00000002, 
            0x00000003, 
            0x00000004, 
            0x00000005, 
            0x00000006, 
            0x00000007, 
            0x00000008, 
            0x00000009, 
            0x0000000A};
    /*
    khai báo mảng 
    int a[0] <=> a = &a[0]
    int *p = &a[0];
    p += 1; dịch p thêm 1 đơn vị
    p += 2; dịch trỏ p thêm 2 đơn vị

    float* q = (float*)p;
    // cùng 1 vùng nhớ nhưng khác kiểu
    // dữ liệu
    // char <-> int <-> float
    */
    int* p = &a[0]; // p trỏ tới địa chỉ a[0]
    int y = *p; // y = giá trị trị mà con trỏ p trỏ tới
                   // trỏ p trỏ tới địa chỉ của a[0] nên y = a[0] = 1
    // dịch mảng bằng con trỏ
    /*
    |a[0]| <-- p
    |a[1]| <-- p = p + 1
    |a[2]|
    |a[3]|
    ...
    [a[9]]
    */
    p += 1;            
    
    float* q = (float*)p;
    float z = *((int*)q);
    
    unsigned char* k = (unsigned char*)&z;
    printf("0x%02X 0x%02X 0x%02X 0x%02X\n", (unsigned char)k[0], (unsigned char)k[1], (unsigned char)k[2], (unsigned char)k[3]);

    printf("\n");
}