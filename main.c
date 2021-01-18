#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct node {
    char fileName[50];
    int wordRelScore;
    int degree;

    struct node *parent;
    struct node *sibling;
    struct node *child;
};

struct node *createNode(char *fileName, int wordRelScore) {
    struct node *newNode;
    newNode = malloc(sizeof(struct node));
    strcpy(newNode->fileName, fileName);
    newNode->wordRelScore = wordRelScore;
    newNode->degree = 0;
    newNode->parent = newNode->sibling = newNode->child = NULL;
    return newNode;
}

void *swapNode(struct node *node1, struct node *node2) {
    struct node tempNode;
    tempNode = *node1;
    *node1 = *node2;
    *node2 = tempNode;
}

struct node *mergeBinomTrees(struct node *node1, struct node *node2) {
    if (node1->wordRelScore > node2->wordRelScore)
        swapNode(node1, node2);

    node2->parent = node1;
    node2->sibling = node1->child;
    node1->child = node2;
    node1->degree++;

    return node1;
}

void linkBinomHeap(struct node *heap1, struct node *heap2) {
    heap1->parent = heap2;
    heap1->sibling = heap2->child;
    heap2->child = heap1;
    heap2->degree++;
}

struct node *mergeBinomHeap(struct node *heap1, struct node *heap2) {
    struct node *mergedHeap = NULL;
    struct node *node1, *node2;
    struct node *tempNode1, *tempNode2;
    node1 = heap1;
    node2 = heap2;

    if (node1 != NULL && ((node2 != NULL && (node1->degree <= node2->degree)) || (node2 == NULL))) {
        mergedHeap = node1;
    } else {
        mergedHeap = node2;
    }

    while (node1 != NULL && node2 != NULL) {
        if (node1->degree < node2->degree) {
            node1 = node1->sibling;
        } else if (node1->degree == node2->degree) {
            tempNode1 = node1->sibling;
            node1->sibling = node2;
            node1 = tempNode1;
        } else {
            tempNode2 = node2->sibling;
            node2->sibling = node1;
            node2 = tempNode2;
        }
    }
    return mergedHeap;
}


struct node *unionBinomHeap(struct node *heap1, struct node *heap2) {
    struct node *prevNode, *nextNode, *currentNode;
    struct node *tempHeap = NULL;
    tempHeap = mergeBinomHeap(heap1, heap2);

    if (tempHeap == NULL)
        return tempHeap;

    prevNode = createNode("\0", 0);
    currentNode = tempHeap;
    nextNode = currentNode->sibling;

    while (nextNode != NULL) {
        if ((currentNode->degree != nextNode->degree) ||
            ((nextNode->sibling != NULL) && (nextNode->sibling)->degree == currentNode->degree)) {
            prevNode = currentNode;
            currentNode = nextNode;
        } else {
            if ((currentNode->wordRelScore <= nextNode->wordRelScore)) {
                currentNode->sibling = nextNode->sibling;
                linkBinomHeap(nextNode, currentNode);
            } else {
                if (prevNode->sibling == NULL) {
                    tempHeap = nextNode;
                } else {
                    prevNode->sibling = nextNode;
                }
                linkBinomHeap(currentNode, nextNode);
                currentNode = nextNode;
            }
        }
        nextNode = currentNode->sibling;
    }
    return tempHeap;
}

struct node *insertBinomHeap(struct node *heap, struct node *node) {
    node->parent = node->sibling = node->child = NULL;
    node->degree = 0;
    heap = unionBinomHeap(heap, node);
    return heap;
}

void display(struct node *heap) {
    struct node *tempNode;
    if (heap == NULL) {
        printf("\nThe heap is empty.");
        return;
    }

    printf("\nThe root nodes are:\n");
    tempNode = heap;
    while (tempNode != NULL) {
        printf("%d", tempNode->wordRelScore);
        if (tempNode->sibling != NULL)
            printf("-->");
        tempNode = tempNode->sibling;
    }
}


int main() {

    struct node *heap = NULL;

    int counter;
    struct node fileList[50];
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
        return -1;
    }

    for (int i = 0; (dirStr = readdir(dirPtr)) != NULL; i++)
        strcpy(fileList[i].fileName, dirStr->d_name);
    closedir(dirPtr);

    for (int i = 0; strcmp(fileList[i].fileName, "\0") != 0; i++) {
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

        printf("\n%s\n", fileList[i].fileName);

        do {
            currentChar = fgetc(filePtr);

            if ((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) {
                strcat(currentWord, (char[2]) {(char) currentChar, '\0'});
//                printf("%c", currentChar);
            } else {
                if (currentChar == 39)
                    continue;

                if (stricmp(currentWord, keyword) == 0)
                    counter++;

                strcpy(currentWord, "\0");
            }

        } while (currentChar != EOF);

        fileList[i].wordRelScore = counter;
        printf("%d", fileList[i].wordRelScore);

        tempNode = createNode(fileList[i].fileName, fileList[i].wordRelScore);
        heap = insertBinomHeap(heap, tempNode);

        fclose(filePtr);
    }

    display(heap);

    return 0;
}
