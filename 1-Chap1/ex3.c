#include <stdio.h>
#include <stdlib.h>

int main()
{
    float* a = NULL;   // Con trỏ mảng động
    float x = 0;       // Giá trị nhập vào
    int n = 0;         // Số phần tử đã nhập

    do
    {
        scanf("%f", &x);   // Nhập một số thực

        if (x != 0)
        {
            // Tăng kích thước mảng thêm 1 phần tử
            a = realloc(a, (n + 1) * sizeof(float));

            a[n] = x;  // Lưu giá trị vào mảng
            n += 1;    // Tăng số phần tử
        }

    } while (x != 0);  // Dừng khi nhập 0

    float sum = 0;

    // Tính tổng các phần tử
    for (int i = 0; i < n; i++)
    {
        sum += a[i];
    }

    printf("%.2f\n", sum);  // In tổng với 2 chữ số thập phân

    free(a);  // Giải phóng bộ nhớ động
    a = NULL; // Tránh con trỏ treo
}