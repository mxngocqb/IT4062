/* =====================================================================
 * Lab 13 - BAI TAP LON CUA CHUONG (Exercise 1, slide 29)
 *
 * Yeu cau trong slide:
 *   1) Dung system() de chay "ifconfig" va hien thi cac dia chi IP cua may
 *   2) Dung system() de chay "ls -a -l" va hien thi danh sach tep (ten, kich thuoc)
 *   3) Dung scandir() de xay dung danh sach thu muc con va tep, hien thi
 *      ket qua duoi dang HTML:
 *          <a href="FolderA"><b>FolderA</b></a>
 *          <a href="FileName1"><i>FileName1</i></a>
 *   4) Cho nguoi dung nhap mot xau:
 *        - neu la thu muc trong duong dan hien tai -> di vao va liet ke lai
 *        - neu la ".."  -> di len (neu chua o thu muc goc) va liet ke lai
 *
 * Cach chay: ./lab13_dir_browser
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#define TEP_HTML "listing.html"

/* ---------------------------------------------------------------------
 * Phan 1: hien thi dia chi IP
 * ------------------------------------------------------------------ */
static void hien_thi_ip_bang_system(void)
{
    printf("\n=== 1a. Dia chi IP bang system(\"ifconfig\") ===\n");
    fflush(stdout);

    /* Ubuntu moi khong cai san ifconfig -> thu ca hai lenh */
    int ma = system("ifconfig 2>/dev/null | grep 'inet '");
    if (ma != 0) {
        printf("(khong co ifconfig, thu lenh 'ip addr')\n");
        fflush(stdout);
        if (system("ip -4 addr show 2>/dev/null | grep 'inet '") != 0)
            printf("(khong chay duoc ca hai lenh)\n");
    }
}

/* Cach lam "dung C" hon: doc truc tiep danh sach giao dien mang */
static void hien_thi_ip_bang_getifaddrs(void)
{
    printf("\n=== 1b. Dia chi IP bang getifaddrs() ===\n");

    struct ifaddrs *ds = NULL;
    if (getifaddrs(&ds) != 0) {
        perror("getifaddrs");
        return;
    }
    for (struct ifaddrs *p = ds; p != NULL; p = p->ifa_next) {
        if (p->ifa_addr == NULL)
            continue;
        char dia_chi[INET6_ADDRSTRLEN];
        if (p->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *a = (struct sockaddr_in *)p->ifa_addr;
            inet_ntop(AF_INET, &a->sin_addr, dia_chi, sizeof dia_chi);
            printf("  %-10s IPv4  %s\n", p->ifa_name, dia_chi);
        } else if (p->ifa_addr->sa_family == AF_INET6) {
            struct sockaddr_in6 *a = (struct sockaddr_in6 *)p->ifa_addr;
            inet_ntop(AF_INET6, &a->sin6_addr, dia_chi, sizeof dia_chi);
            printf("  %-10s IPv6  %s\n", p->ifa_name, dia_chi);
        }
    }
    freeifaddrs(ds);
}

/* ---------------------------------------------------------------------
 * Phan 2: liet ke thu muc bang system("ls -a -l")
 * ------------------------------------------------------------------ */
static void liet_ke_bang_ls(void)
{
    printf("\n=== 2. system(\"ls -a -l\") ===\n");
    fflush(stdout);
    if (system("ls -a -l") != 0)
        fprintf(stderr, "Khong chay duoc lenh ls\n");
}

/* ---------------------------------------------------------------------
 * Phan 3: liet ke bang scandir va xuat ra dang HTML
 * ------------------------------------------------------------------ */
static int la_thu_muc(const char *ten)
{
    struct stat st;
    if (stat(ten, &st) != 0)
        return 0;
    return S_ISDIR(st.st_mode);
}

/* Bo qua muc "." cho gon (van giu ".." de di len) */
static int loc(const struct dirent *e)
{
    return strcmp(e->d_name, ".") != 0;
}

static void liet_ke_html(void)
{
    printf("\n=== 3. scandir() -> dang HTML ===\n");

    struct dirent **ds = NULL;
    int n = scandir(".", &ds, loc, alphasort);
    if (n < 0) {
        perror("scandir");
        return;
    }

    FILE *f = fopen(TEP_HTML, "w");
    if (f != NULL) {
        char cwd[1024];
        getcwd(cwd, sizeof cwd);
        fprintf(f, "<html>\n<head><meta charset=\"utf-8\">"
                   "<title>%s</title></head>\n<body>\n<h3>%s</h3>\n", cwd, cwd);
    }

    for (int i = 0; i < n; i++) {
        const char *ten = ds[i]->d_name;
        char dong[1024];

        if (la_thu_muc(ten))
            snprintf(dong, sizeof dong,
                     "<a href = \"%s\"><b>%s</b></a>", ten, ten);
        else
            snprintf(dong, sizeof dong,
                     "<a href = \"%s\"><i>%s</i></a>", ten, ten);

        printf("  %s\n", dong);
        if (f != NULL)
            fprintf(f, "%s<br>\n", dong);

        free(ds[i]);
    }
    free(ds);

    if (f != NULL) {
        fprintf(f, "</body>\n</html>\n");
        fclose(f);
        printf("  (da ghi ket qua ra tep %s)\n", TEP_HTML);
    }
    printf("  Tong cong %d muc\n", n);
}

/* ---------------------------------------------------------------------
 * Phan 4: di chuyen giua cac thu muc
 * ------------------------------------------------------------------ */
static int dang_o_thu_muc_goc(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof cwd) == NULL)
        return 0;
    return strcmp(cwd, "/") == 0;
}

static void in_thu_muc_hien_tai(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof cwd) != NULL)
        printf("\n[Thu muc hien tai] %s\n", cwd);
}

static void in_menu(void)
{
    printf("\n---------------------------------------------------------\n"
           "  Nhap ten thu muc  : di vao thu muc do va liet ke lai\n"
           "  ..                : di len thu muc cha\n"
           "  ip                : hien thi dia chi IP cua may\n"
           "  ls                : chay lenh system(\"ls -a -l\")\n"
           "  html              : liet ke bang scandir va xuat HTML\n"
           "  q                 : thoat\n"
           "---------------------------------------------------------\n"
           "> ");
    fflush(stdout);
}

int main(void)
{
    /* Chay lan luot ba phan dau nhu yeu cau cua bai tap */
    hien_thi_ip_bang_system();
    hien_thi_ip_bang_getifaddrs();
    liet_ke_bang_ls();
    in_thu_muc_hien_tai();
    liet_ke_html();

    /* Phan 4: vong lap tuong tac */
    char dong[512];
    for (;;) {
        in_menu();

        if (fgets(dong, sizeof dong, stdin) == NULL) {
            printf("\n(Het du lieu vao) Ket thuc.\n");
            break;
        }
        dong[strcspn(dong, "\r\n")] = '\0';      /* bo ky tu xuong dong */
        if (dong[0] == '\0')
            continue;

        if (strcmp(dong, "q") == 0 || strcmp(dong, "quit") == 0) {
            printf("Tam biet!\n");
            break;
        }
        if (strcmp(dong, "ip") == 0) {
            hien_thi_ip_bang_getifaddrs();
            continue;
        }
        if (strcmp(dong, "ls") == 0) {
            liet_ke_bang_ls();
            continue;
        }
        if (strcmp(dong, "html") == 0) {
            in_thu_muc_hien_tai();
            liet_ke_html();
            continue;
        }
        if (strcmp(dong, "..") == 0) {
            if (dang_o_thu_muc_goc()) {
                printf("Dang o thu muc goc, khong di len duoc nua.\n");
                continue;
            }
            if (chdir("..") != 0) {
                perror("chdir");
                continue;
            }
            in_thu_muc_hien_tai();
            liet_ke_html();
            continue;
        }

        /* Truong hop con lai: coi nhu ten thu muc con */
        if (!la_thu_muc(dong)) {
            printf("\"%s\" khong phai la thu muc trong duong dan hien tai.\n", dong);
            continue;
        }
        if (chdir(dong) != 0) {
            perror("chdir");
            continue;
        }
        in_thu_muc_hien_tai();
        liet_ke_html();
    }
    return 0;
}
