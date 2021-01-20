#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

struct node {
    char fileName[50];
    int wordRelScore;
    int degree;

    struct node *parent;
    struct node *sibling;
    struct node *child;
};

struct node *Head;
struct node *Head2;

struct node *createNode(char *fileName, int wordRelScore) {
    struct node *newNode;
    newNode = malloc(sizeof(struct node));
    strcpy(newNode->fileName, fileName);
    newNode->wordRelScore = wordRelScore;
    newNode->degree = 0;
    newNode->parent = newNode->sibling = newNode->child = NULL;
    return newNode;
}

void linkBinomHeaps(struct node *heap1, struct node *heap2) {
    heap1->parent = heap2;
    heap1->sibling = heap2->child;
    heap2->child = heap1;
    heap2->degree = heap2->degree + 1;
}

struct node *mergeBinomHeaps(struct node *heap1, struct node *heap2) {
    struct node *tempHeap1, *tempHeap2;
    struct node *tempNode1, *tempNode2;
    struct node *head;

    tempHeap1 = heap1;
    tempHeap2 = heap2;

    if (tempHeap1 != NULL) {
        if (tempHeap2 != NULL && tempHeap1->degree <= tempHeap2->degree) {
            head = tempHeap1;
        } else if (tempHeap2 != NULL && tempHeap1->degree > tempHeap2->degree) {
            head = tempHeap2;
        } else {
            head = tempHeap1;
        }
    } else {
        head = tempHeap2;
    }

    while (tempHeap1 != NULL && tempHeap2 != NULL) {
        if (tempHeap1->degree < tempHeap2->degree) {
            tempHeap1 = tempHeap1->sibling;
        } else if (tempHeap1->degree == tempHeap2->degree) {
            tempNode1 = tempHeap1->sibling;
            tempHeap1->sibling = tempHeap2;
            tempHeap1 = tempNode1;
        } else {
            tempNode2 = tempHeap2->sibling;
            tempHeap2->sibling = tempHeap1;
            tempHeap2 = tempNode2;
        }
    }
    return head;
}

struct node *unionBinomHeaps(struct node *heap1, struct node *heap2) {
    struct node *prevNode, *nextNode, *currentNode;

    Head = mergeBinomHeaps(heap1, heap2);

    if (Head == NULL)
        return Head;

    prevNode = NULL;
    currentNode = Head;
    nextNode = currentNode->sibling;

    while (nextNode != NULL) {
        if ((currentNode->degree != nextNode->degree) ||
            ((nextNode->sibling != NULL) && (nextNode->sibling)->degree == currentNode->degree)) {
            prevNode = currentNode;
            currentNode = nextNode;
        } else {
            if (currentNode->wordRelScore <= nextNode->wordRelScore) {
                currentNode->sibling = nextNode->sibling;
                linkBinomHeaps(nextNode, currentNode);
            } else {
                if (prevNode == NULL) {
                    Head = nextNode;
                } else {
                    prevNode->sibling = nextNode;
                }
                linkBinomHeaps(currentNode, nextNode);
                currentNode = nextNode;
            }
        }
        nextNode = currentNode->sibling;
    }
    return Head;
}

struct node *insertToBinomHeap(struct node *head, struct node *node) {
    struct node *tempNode = NULL;
    tempNode = node;
    head = unionBinomHeaps(head, tempNode);
    return head;
}

void revertList(struct node *heap) {
    if (heap->sibling != NULL) {
        revertList(heap->sibling);
        (heap->sibling)->sibling = heap;
    } else {
        Head2 = heap;
    }
}

struct node *extractMinFromHeap(struct node *heap1) {
    struct node *minNode = heap1;
    struct node *tempNode = NULL;
    struct node *currentNode = heap1;
    struct node *tempNode2;

    Head2 = NULL;

    if (currentNode == NULL) {
        printf("\nThere is no node to extract from heap.\n");
        return currentNode;
    }

    tempNode2 = minNode;

    while (tempNode2->sibling != NULL) {
        if ((tempNode2->sibling)->wordRelScore < minNode->wordRelScore) {
            minNode = tempNode2->sibling;
            tempNode = tempNode2;
            currentNode = tempNode2->sibling;
        }
        tempNode2 = tempNode2->sibling;
    }

    if (tempNode == NULL && currentNode->sibling == NULL) {
        heap1 = NULL;
    } else if (tempNode == NULL) {
        heap1 = currentNode->sibling;
    } else if (tempNode->sibling == NULL) {
        tempNode = NULL;
    } else {
        tempNode->sibling = currentNode->sibling;
    }

    if (currentNode->child != NULL) {
        revertList(currentNode->child);
        (currentNode->child)->sibling = NULL;
    }

    Head = unionBinomHeaps(heap1, Head2);
    return currentNode;
}

void printMostRelFiveFile(char *keyword) {
    int fileCount = 0;
    for (int i = 0; i < 5; i++) {
        struct node *minNode = extractMinFromHeap(Head);
        char fileName[56] = "files/";
        char currentChar;

        if (300 - minNode->wordRelScore == 0)
            continue;

        strcat(fileName, minNode->fileName);
        FILE *filePtr = fopen(fileName, "r");

        if (filePtr == NULL) {
            printf("Cannot open file!");
            exit(0);
        }
        puts("\n--------------------------------------------------------------------------------------------------");
        printf("In the '%s' named file there is %d keyword: '%s'.\n\n", minNode->fileName,
               300 - minNode->wordRelScore, keyword);

        do {
            currentChar = fgetc(filePtr);
            printf("%c", currentChar);

        } while (currentChar != EOF);

        fileCount++;
        fclose(filePtr);
    }

    puts("\n--------------------------------------------------------------------------------------------------");

    if (fileCount == 0)
        printf("\nThere is no file containing the keyword '%s'.\n", keyword);
    else if (fileCount == 1)
        printf("\nThere is only 1 file containing the keyword '%s'.\n",keyword);
    else if (fileCount < 5)
        printf("\nThere is only %d files containing the keyword '%s'.\n", fileCount, keyword);
}

int main() {
    int counter;
    int fileCount = 0;
    char keyword[30] = "\0";
    char currentWord[30] = "\0";
    char currentChar;

    DIR *dirPtr;
    FILE *filePtr;
    struct dirent *dirStr;

    printf("Enter a keyword:");
    scanf("%s", keyword);

    dirPtr = opendir("files");
    if (dirPtr == NULL) {
        printf("Could not open current directory!");
        return -1;
    }

    for (int i = 0; readdir(dirPtr) != NULL; i++)
        fileCount++;
    closedir(dirPtr);
    struct node fileList[fileCount];

    for (int i = 0; i < fileCount; i++) {
        strcpy(fileList[i].fileName, "\0");
    }

    dirPtr = opendir("files");
    for (int i = 0; (dirStr = readdir(dirPtr)) != NULL; i++)
        strcpy(fileList[i].fileName, dirStr->d_name);
    closedir(dirPtr);

    for (int i = 0; i < fileCount; i++) {
        struct node *tempNode;
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

        do {
            currentChar = fgetc(filePtr);

            if ((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) {
                strcat(currentWord, (char[2]) {(char) currentChar, '\0'});
            } else {
                if (currentChar == 39) {
                    continue;
                }

                if (stricmp(currentWord, keyword) == 0) {
                    counter++;
                }

                strcpy(currentWord, "\0");
            }

        } while (currentChar != EOF);

        fileList[i].wordRelScore = counter;

        tempNode = createNode(fileList[i].fileName, 300 - fileList[i].wordRelScore);
        Head = insertToBinomHeap(Head, tempNode);

        fclose(filePtr);
    }

    printMostRelFiveFile(keyword);

    return 0;
}

