#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main() {

    char fileList[50][100], c;

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

    for (int i = 2; strcmp(fileList[i], "\0") != 0; i++) {
        char fileName[] = "files/";

        strcat(fileName, fileList[i]);
        filePtr = fopen(fileName, "r");
        if (filePtr == NULL) {
            printf("Cannot open file \n");
            exit(0);
        }

        printf("\n%s\n", fileList[i]);

        // Read contents from file
        c = fgetc(filePtr);
        while (c != EOF) {
            printf("%c", c);
            c = fgetc(filePtr);
        }

        fclose(filePtr);
    }


    closedir(dr);
    return 0;
}
