/* =====================================================================
 * Lab 11 - Truy cap tep va thu muc bang loi goi he thong
 * Nguon: Chapter 2, slide 25-27 (File Access & Directory System Calls)
 *
 *   getcwd, chdir, access, stat, fstat, scandir, rename, remove, tmpfile
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

static void in_quyen(mode_t m)
{
    char q[10] = "---------";
    if (m & S_IRUSR) q[0] = 'r';
    if (m & S_IWUSR) q[1] = 'w';
    if (m & S_IXUSR) q[2] = 'x';
    if (m & S_IRGRP) q[3] = 'r';
    if (m & S_IWGRP) q[4] = 'w';
    if (m & S_IXGRP) q[5] = 'x';
    if (m & S_IROTH) q[6] = 'r';
    if (m & S_IWOTH) q[7] = 'w';
    if (m & S_IXOTH) q[8] = 'x';
    printf("%s", q);
}

static void thong_tin_tep(const char *duong_dan)
{
    struct stat st;
    if (stat(duong_dan, &st) != 0) {
        perror(duong_dan);
        return;
    }
    printf("  %-24s ", duong_dan);
    if (S_ISDIR(st.st_mode))       printf("[thu muc] ");
    else if (S_ISREG(st.st_mode))  printf("[tep    ] ");
    else if (S_ISLNK(st.st_mode))  printf("[lien ket] ");
    else                           printf("[khac   ] ");
    in_quyen(st.st_mode);
    printf(" %8lld byte  ", (long long)st.st_size);

    char thoi_gian[32];
    struct tm tm_buf;
    time_t t = st.st_mtime;
    localtime_r(&t, &tm_buf);
    strftime(thoi_gian, sizeof thoi_gian, "%Y-%m-%d %H:%M", &tm_buf);
    printf("%s\n", thoi_gian);
}

int main(void)
{
    char duong_dan[1024];

    /* ---------- 1. Thu muc lam viec hien tai ---------- */
    printf("--- 1. getcwd / chdir ---\n");
    if (getcwd(duong_dan, sizeof duong_dan) == NULL) {
        perror("getcwd");
        return 1;
    }
    printf("  Thu muc hien tai: %s\n", duong_dan);

    if (chdir("..") == 0) {
        char tam[1024];
        getcwd(tam, sizeof tam);
        printf("  Sau chdir(\"..\") : %s\n", tam);
        if (chdir(duong_dan) != 0) perror("chdir tro lai");
        printf("  Quay lai         : %s\n", duong_dan);
    }

    /* ---------- 2. access: kiem tra quyen truy cap ---------- */
    printf("\n--- 2. access ---\n");
    const char *cac_tep[] = {".", "/etc/hosts", "/bin/ls", "khong_ton_tai.txt"};
    for (size_t i = 0; i < sizeof cac_tep / sizeof cac_tep[0]; i++) {
        printf("  %-20s ton tai:%s  doc:%s  ghi:%s  chay:%s\n", cac_tep[i],
               access(cac_tep[i], F_OK) == 0 ? "co " : "khong",
               access(cac_tep[i], R_OK) == 0 ? "co " : "khong",
               access(cac_tep[i], W_OK) == 0 ? "co " : "khong",
               access(cac_tep[i], X_OK) == 0 ? "co " : "khong");
    }

    /* ---------- 3. stat: thong tin chi tiet ---------- */
    printf("\n--- 3. stat ---\n");
    thong_tin_tep(".");
    thong_tin_tep("/etc/hosts");

    /* ---------- 4. fstat: thong tin qua mo ta tep dang mo ---------- */
    printf("\n--- 4. fstat ---\n");
    int fd = open("/etc/hosts", O_RDONLY);
    if (fd >= 0) {
        struct stat st;
        if (fstat(fd, &st) == 0)
            printf("  fd = %d, kich thuoc = %lld byte, so inode = %llu\n",
                   fd, (long long)st.st_size, (unsigned long long)st.st_ino);
        close(fd);
    } else {
        perror("open /etc/hosts");
    }

    /* ---------- 5. scandir: liet ke thu muc, da sap xep ---------- */
    printf("\n--- 5. scandir(\".\") ---\n");
    struct dirent **danh_sach = NULL;
    int n = scandir(".", &danh_sach, NULL, alphasort);
    if (n < 0) {
        perror("scandir");
    } else {
        for (int i = 0; i < n; i++) {
            struct stat st;
            const char *ten = danh_sach[i]->d_name;
            if (stat(ten, &st) == 0)
                printf("  %-30s %s %lld byte\n", ten,
                       S_ISDIR(st.st_mode) ? "[DIR ]" : "[FILE]",
                       (long long)st.st_size);
            free(danh_sach[i]);         /* scandir cap phat tung phan tu... */
        }
        free(danh_sach);                /* ...va ca mang con tro */
        printf("  Tong cong %d muc\n", n);
    }

    /* ---------- 6. rename / remove ---------- */
    printf("\n--- 6. rename / remove ---\n");
    FILE *f = fopen("tep_tam.txt", "w");
    if (f != NULL) {
        fprintf(f, "tep tam de thu nghiem\n");
        fclose(f);
        printf("  Da tao tep_tam.txt\n");

        if (rename("tep_tam.txt", "tep_tam_doi_ten.txt") == 0)
            printf("  rename  -> tep_tam_doi_ten.txt\n");
        else
            perror("rename");

        if (remove("tep_tam_doi_ten.txt") == 0)
            printf("  remove  -> da xoa\n");
        else
            perror("remove");
    }

    /* ---------- 7. tmpfile: tep tam tu xoa khi dong ---------- */
    printf("\n--- 7. tmpfile ---\n");
    FILE *tam = tmpfile();
    if (tam != NULL) {
        fputs("du lieu tam thoi\n", tam);
        rewind(tam);
        char dong[64];
        if (fgets(dong, sizeof dong, tam) != NULL)
            printf("  Doc lai tu tep tam: %s", dong);
        fclose(tam);        /* tep bi xoa tu dong */
        printf("  Da dong -> tep tu dong bi xoa\n");
    }
    return 0;
}
