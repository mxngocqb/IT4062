/* =====================================================================
 * Lab 13 - BAI TAP LON CUA CHUONG (Exercise 1, slide 29)
 *
 *   1) system("ifconfig")  -> hien thi dia chi IP cua may
 *   2) system("ls -a -l")  -> danh sach tep va kich thuoc
 *   3) scandir()           -> xuat danh sach dang HTML:
 *          <a href = "FolderA"><b>FolderA</b></a>
 *          <a href = "FileName1"><i>FileName1</i></a>
 *   4) Nhap mot xau: la thu muc thi di vao va liet ke lai,
 *      la ".." thi di len (neu chua o thu muc goc).
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define TEP_HTML "listing.html"

static int la_thu_muc(const char *ten)
{
    struct stat st;
    return stat(ten, &st) == 0 && S_ISDIR(st.st_mode);
}

static int loc(const struct dirent *e)      /* bo qua muc "." cho gon */
{
    return strcmp(e->d_name, ".") != 0;
}

static void hien_thi_ip(void)
{
    printf("\n=== Dia chi IP ===\n");
    fflush(stdout);
    /* Ubuntu moi khong con cai san ifconfig -> thu tiep lenh ip */
    if (system("ifconfig 2>/dev/null | grep 'inet '") != 0)
        if (system("ip -4 addr show 2>/dev/null | grep 'inet '") != 0)
            printf("(khong chay duoc ifconfig lan ip)\n");
}

static void liet_ke_bang_ls(void)
{
    printf("\n=== system(\"ls -a -l\") ===\n");
    fflush(stdout);
    if (system("ls -a -l") != 0)
        fprintf(stderr, "khong chay duoc lenh ls\n");
}

static void liet_ke_html(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof cwd) != NULL)
        printf("\n[Thu muc] %s\n", cwd);

    struct dirent **ds = NULL;
    int n = scandir(".", &ds, loc, alphasort);
    if (n < 0) { perror("scandir"); return; }

    FILE *f = fopen(TEP_HTML, "w");
    if (f != NULL)
        fprintf(f, "<html><head><meta charset=\"utf-8\"></head><body>\n<h3>%s</h3>\n", cwd);

    for (int i = 0; i < n; i++) {
        const char *ten = ds[i]->d_name;
        char dong[1024];
        /* thu muc in dam, tep in nghieng */
        snprintf(dong, sizeof dong, "<a href = \"%s\">%s%s%s</a>", ten,
                 la_thu_muc(ten) ? "<b>" : "<i>", ten,
                 la_thu_muc(ten) ? "</b>" : "</i>");
        printf("  %s\n", dong);
        if (f != NULL) fprintf(f, "%s<br>\n", dong);
        free(ds[i]);
    }
    free(ds);

    if (f != NULL) {
        fprintf(f, "</body></html>\n");
        fclose(f);
        printf("  (%d muc, da ghi ra %s)\n", n, TEP_HTML);
    }
}

int main(void)
{
    hien_thi_ip();
    liet_ke_bang_ls();
    liet_ke_html();

    char dong[512];
    for (;;) {
        printf("\nNhap: <ten thu muc> | .. | ip | ls | q\n> ");
        fflush(stdout);

        if (fgets(dong, sizeof dong, stdin) == NULL) break;
        dong[strcspn(dong, "\r\n")] = '\0';
        if (dong[0] == '\0') continue;

        if (strcmp(dong, "q") == 0) break;
        if (strcmp(dong, "ip") == 0) { hien_thi_ip();       continue; }
        if (strcmp(dong, "ls") == 0) { liet_ke_bang_ls();   continue; }

        if (strcmp(dong, "..") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof cwd) != NULL && strcmp(cwd, "/") == 0) {
                printf("Dang o thu muc goc.\n");
                continue;
            }
        } else if (!la_thu_muc(dong)) {
            printf("\"%s\" khong phai thu muc trong duong dan hien tai.\n", dong);
            continue;
        }

        if (chdir(dong) != 0) { perror("chdir"); continue; }
        liet_ke_html();
    }
    printf("Tam biet!\n");
    return 0;
}
