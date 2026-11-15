#include <stdio.h>

int main()
{
    // Nhap gia tri cho bien x
    float x;
    scanf("%f", &x);
    // Lấy địa chỉ của x và ép thành con trỏ 
    // unsigned char*  để đọc từng byte của biến x.
    unsigned char* y = (unsigned char*)&x;
    // In ra cac byte cua bien x theo he thap luc
    printf("0x%02X 0x%02X 0x%02X 0x%02X\n", 
        (unsigned char)y[0], 
        (unsigned char)y[1], 
        (unsigned char)y[2], 
        (unsigned char)y[3]);
}