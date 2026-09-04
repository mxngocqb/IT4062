/* =====================================================================
 * Lab 05 - Con tro ham va co che call-back
 * Nguon: Chapter 2, slide 15 (Pointer to functions)
 *
 *   typedef void (*printer_t)(int);
 *   printer_t p = &print_to_n;
 *   p(5);
 * ===================================================================== */
#include <stdio.h>
#include <string.h>

/* ---------- 1. Vi du trong slide ---------- */
typedef void (*printer_t)(int);

static void print_to_n(int n)
{
    for (int i = 1; i <= n; ++i)
        printf("%d ", i);
    printf("\n");
}

/* ---------- 2. Con tro ham lam tham so: co che call-back ---------- */
static int gap_doi(int x)     { return 2 * x; }
static int binh_phuong(int x) { return x * x; }

static void ap_dung(const int *m, size_t n, int (*f)(int))
{
    for (size_t i = 0; i < n; i++)
        printf("%d ", f(m[i]));       /* goi ham duoc truyen vao */
    printf("\n");
}

/* ---------- 3. Bang dieu phoi lenh (cach server xu ly giao thuc) ---------- */
static void lam_login(void) { printf("  -> dang nhap\n"); }
static void lam_quit(void)  { printf("  -> dong ket noi\n"); }

struct muc_lenh {
    const char *ten;
    void (*xu_ly)(void);          /* con tro ham xu ly lenh do */
};

int main(void)
{
    printf("--- 1. Goi ham qua con tro ---\n");
    printer_t p = &print_to_n;    /* luu dia chi ham vao bien */
    p(5);                         /* goi ham thong qua bien do */

    printf("\n--- 2. Call-back ---\n");
    int m[] = {1, 2, 3, 4, 5};
    size_t n = sizeof m / sizeof m[0];
    printf("  gap doi     : ");  ap_dung(m, n, gap_doi);
    printf("  binh phuong : ");  ap_dung(m, n, binh_phuong);

    printf("\n--- 3. Bang dieu phoi lenh ---\n");
    struct muc_lenh bang[] = {{"LOGIN", lam_login}, {"QUIT", lam_quit}};
    const char *nhan[] = {"LOGIN", "QUIT", "RETR"};

    for (size_t i = 0; i < sizeof nhan / sizeof nhan[0]; i++) {
        printf("Nhan lenh \"%s\":\n", nhan[i]);
        size_t j;
        for (j = 0; j < sizeof bang / sizeof bang[0]; j++)
            if (strcmp(nhan[i], bang[j].ten) == 0) {
                bang[j].xu_ly();
                break;
            }
        if (j == sizeof bang / sizeof bang[0])
            printf("  -> lenh khong ho tro\n");
    }
    return 0;
}
