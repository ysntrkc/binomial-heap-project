#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {

    char fileList[50][100];

    FILE *filePtr;
    struct dirent *de;
    DIR *dr = opendir("files");

    if (dr == NULL) {
        printf("Could not open current directory");
        return 0;
    }

    for (int i = 0; (de = readdir(dr)) != NULL; i++) {
        strcpy(fileList[i], de->d_name);
    }

//    printf("%s", fileList[0]);

    for (int i = 0; strcmp(fileList[i], NULL) == 0; i++) {
        printf("%s\n", fileList[i]);
    }


    closedir(dr);
    return 0;
}
