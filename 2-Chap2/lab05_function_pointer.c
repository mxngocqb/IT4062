/* =====================================================================
 * Lab 05 - Con tro ham va co che call-back
 * Nguon: Chapter 2, slide 15 (Pointer to functions)
 *
 *   typedef void (*printer_t)(int);
 *   printer_t p = &print_to_n;
 *   p(5);
 *
 * Y nghia trong lap trinh mang: bang dieu phoi lenh (command dispatch
 * table) cua mot server duoc xay dung bang con tro ham.
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>   /* strcasecmp */

/* ---------- 1. Vi du trong slide ---------- */
typedef void (*printer_t)(int);

static void print_to_n(int n)
{
    for (int i = 1; i <= n; ++i)
        printf("%d ", i);
    printf("\n");
}

static void print_squares_to_n(int n)
{
    for (int i = 1; i <= n; ++i)
        printf("%d ", i * i);
    printf("\n");
}

/* ---------- 2. Con tro ham lam tham so (call-back) ---------- */
static void ap_dung(const int *m, size_t n, int (*bien_doi)(int))
{
    for (size_t i = 0; i < n; i++)
        printf("%d ", bien_doi(m[i]));
    printf("\n");
}

static int gap_doi(int x) { return 2 * x; }
static int binh_phuong(int x) { return x * x; }

/* ---------- 3. qsort dung con tro ham so sanh ---------- */
static int so_sanh_tang(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int so_sanh_giam(const void *a, const void *b)
{
    return so_sanh_tang(b, a);
}

/* ---------- 4. Bang dieu phoi lenh cua mot "server" ---------- */
static void xu_ly_login(const char *tham_so)
{
    printf("  [LOGIN] dang nhap voi tham so: \"%s\"\n", tham_so);
}

static void xu_ly_list(const char *tham_so)
{
    (void)tham_so;                       /* lenh nay khong dung tham so */
    printf("  [LIST] tra ve danh sach thu\n");
}

static void xu_ly_quit(const char *tham_so)
{
    (void)tham_so;
    printf("  [QUIT] dong ket noi\n");
}

struct muc_lenh {
    const char *ten;
    void (*xu_ly)(const char *tham_so);
};

static const struct muc_lenh BANG_LENH[] = {
    {"LOGIN", xu_ly_login},
    {"LIST",  xu_ly_list},
    {"QUIT",  xu_ly_quit},
};

static void dieu_phoi(const char *dong)
{
    char ban_sao[128];
    snprintf(ban_sao, sizeof ban_sao, "%s", dong);

    char *luu = NULL;
    char *ten = strtok_r(ban_sao, " \t\r\n", &luu);
    if (ten == NULL) return;
    char *tham_so = strtok_r(NULL, "\r\n", &luu);

    for (size_t i = 0; i < sizeof BANG_LENH / sizeof BANG_LENH[0]; i++) {
        if (strcasecmp(ten, BANG_LENH[i].ten) == 0) {
            BANG_LENH[i].xu_ly(tham_so ? tham_so : "");
            return;
        }
    }
    printf("  [?] lenh khong ho tro: %s\n", ten);
}

int main(void)
{
    printf("--- 1. Vi du trong slide ---\n");
    printer_t p = &print_to_n;
    p(5);                       /* goi ham qua con tro */
    p = print_squares_to_n;     /* dau & la tuy chon */
    p(5);

    printf("\n--- 2. Call-back ---\n");
    int m[] = {1, 2, 3, 4, 5};
    size_t n = sizeof m / sizeof m[0];
    printf("gap doi     : "); ap_dung(m, n, gap_doi);
    printf("binh phuong : "); ap_dung(m, n, binh_phuong);

    printf("\n--- 3. qsort voi ham so sanh ---\n");
    int a[] = {42, 7, 19, 3, 25, 11};
    size_t na = sizeof a / sizeof a[0];
    qsort(a, na, sizeof a[0], so_sanh_tang);
    printf("tang dan : ");
    for (size_t i = 0; i < na; i++) printf("%d ", a[i]);
    qsort(a, na, sizeof a[0], so_sanh_giam);
    printf("\ngiam dan : ");
    for (size_t i = 0; i < na; i++) printf("%d ", a[i]);
    printf("\n");

    printf("\n--- 4. Bang dieu phoi lenh (mo phong server) ---\n");
    const char *cac_dong[] = {
        "LOGIN sinhvien matkhau",
        "LIST",
        "retr 3",
        "QUIT",
    };
    for (size_t i = 0; i < sizeof cac_dong / sizeof cac_dong[0]; i++) {
        printf("Nhan: \"%s\"\n", cac_dong[i]);
        dieu_phoi(cac_dong[i]);
    }
    return 0;
}
