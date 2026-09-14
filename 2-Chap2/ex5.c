#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("ls -al > ls.txt");
    FILE* f = fopen("ls.txt","rt");
    while (!feof(f))
    {
        char line[1024] = { 0 };
        fgets(line, sizeof(line) - 1, f);
        if (line[0] == 'd')
        {
            //TODO
        }else if (line[0] == '-')
        {
            char type_perm[1024] = { 0 };
            char id[1024] = { 0 };
            char owner[1024] = { 0 };
            char group[1024] = { 0 };
            char fsize[1024] = { 0 };
            char month[1024] = { 0 };
            char date[1024] = { 0 };
            char time[1024] = { 0 };
            char name[1024] = { 0 };
            sscanf(line, "%s%s%s%s%s%s%s%s%s", type_perm, id, owner, group, fsize, month, date, time, name);
            printf("%s: %s\n", name, fsize);
        }
    }
    fclose(f);
}