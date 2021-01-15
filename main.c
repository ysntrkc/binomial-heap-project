#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct fileInfo {
    char fileName[50];
    int recurrence;
};

int main() {

    int counter;
    struct fileInfo fileList[50];
    char keyword[30] = "\0";
    char currentWord[30] = "\0";
    char currentChar;

    FILE *filePtr;
    struct dirent *dirStr;
    DIR *dirPtr = opendir("files");

    printf("Enter a keyword:");
    scanf("%s", keyword);

    for (int i = 0; i < 50; i++)
        strcpy(fileList[i].fileName, "\0");

    if (dirPtr == NULL) {
        printf("Could not open current directory!");
        return 0;
    }

    for (int i = 0; (dirStr = readdir(dirPtr)) != NULL; i++)
        strcpy(fileList[i].fileName, dirStr->d_name);
    closedir(dirPtr);

    for (int i = 0; strcmp(fileList[i].fileName, "\0") != 0; i++) {
        char fileName[56] = "files/";
        counter = 0;

        if (strcmp(fileList[i].fileName, ".") == 0 || strcmp(fileList[i].fileName, "..") == 0)
            continue;

        strcat(fileName, fileList[i].fileName);
        filePtr = fopen(fileName, "r");
        if (filePtr == NULL) {
            printf("Cannot open file!");
            exit(0);
        }

//        printf("\n%s\n", fileList[i].fileName);

        do {
            currentChar = fgetc(filePtr);

            if ((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) {
                strcat(currentWord, (char[2]) { (char) currentChar, '\0' });
//                printf("%c", currentChar);
            }
            else {
                if (stricmp(currentWord, keyword) == 0)
                    counter++;

                strcpy(currentWord, "\0");
                continue;
            }

        } while (currentChar != EOF);

        fileList[i].recurrence = counter;
//        printf("%d", fileList[i].recurrence);
        fclose(filePtr);
    }


    return 0;
}
