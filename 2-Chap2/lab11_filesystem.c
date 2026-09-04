/* =====================================================================
 * Lab 11 - Truy cap tep va thu muc   (slide 25-27)
 *   getcwd, chdir, access, stat, scandir, rename, remove
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int main(void)
{
    char cwd[1024];

    printf("--- 1. getcwd / chdir ---\n");
    if (getcwd(cwd, sizeof cwd) == NULL) { perror("getcwd"); return 1; }
    printf("  thu muc hien tai: %s\n", cwd);
    if (chdir("..") == 0) {
        char tam[1024];
        if (getcwd(tam, sizeof tam) != NULL)
            printf("  sau chdir(\"..\"): %s\n", tam);
        if (chdir(cwd) != 0) perror("chdir");   /* quay lai cho cu */
    }

    printf("\n--- 2. access: kiem tra quyen ---\n");
    const char *ds[] = {".", "/etc/hosts", "khong_ton_tai.txt"};
    for (size_t i = 0; i < sizeof ds / sizeof ds[0]; i++)
        printf("  %-20s ton tai:%-4s doc:%-4s ghi:%s\n", ds[i],
               access(ds[i], F_OK) == 0 ? "co" : "khong",
               access(ds[i], R_OK) == 0 ? "co" : "khong",
               access(ds[i], W_OK) == 0 ? "co" : "khong");

    printf("\n--- 3. stat: thong tin tep ---\n");
    struct stat st;
    if (stat("/etc/hosts", &st) == 0)
        printf("  /etc/hosts: %lld byte, quyen %o, %s\n",
               (long long)st.st_size, st.st_mode & 0777,
               S_ISDIR(st.st_mode) ? "thu muc" : "tep thuong");

    printf("\n--- 4. scandir: liet ke thu muc (da sap xep) ---\n");
    struct dirent **ten = NULL;
    int n = scandir(".", &ten, NULL, alphasort);
    if (n < 0) { perror("scandir"); return 1; }
    for (int i = 0; i < n; i++) {
        if (stat(ten[i]->d_name, &st) == 0)
            printf("  %-28s %s %lld byte\n", ten[i]->d_name,
                   S_ISDIR(st.st_mode) ? "[DIR ]" : "[FILE]",
                   (long long)st.st_size);
        free(ten[i]);                     /* scandir cap phat tung phan tu... */
    }
    free(ten);                            /* ...va ca mang con tro */
    printf("  tong %d muc\n", n);

    printf("\n--- 5. rename / remove ---\n");
    FILE *f = fopen("tep_tam.txt", "w");
    if (f != NULL) {
        fputs("tep thu nghiem\n", f);
        fclose(f);
        if (rename("tep_tam.txt", "tep_tam_2.txt") == 0) printf("  da doi ten\n");
        if (remove("tep_tam_2.txt") == 0) printf("  da xoa\n");
    }
    return 0;
}
