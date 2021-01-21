// Yasin Tarakçı - 150118055

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

// I define a struct that hold the binomial heaps.
struct node {
    char fileName[50];
    int wordRelScore;
    int degree;

    struct node *parent;
    struct node *sibling;
    struct node *child;
};

// I define these globally because I use them in some functions and main.
struct node *HEAP;
struct node *HEAP2;

// This function creates the node with the given fileName and wordRelScore.
struct node *createNode(char *fileName, int wordRelScore) {
    struct node *newNode;                  // First the program creates the template node.
    newNode = malloc(sizeof(struct node)); // Here the program allocates the memory for the new node.
    strcpy(newNode->fileName, fileName);   // In these two line the program copies the given values into the new node.
    newNode->wordRelScore = wordRelScore;
    newNode->degree = 0;                   // In these two line the program initializes these variables with the base case.
    newNode->parent = newNode->sibling = newNode->child = NULL;
    return newNode;                        // At last the program returns the new created node.
}

// This function links the given two nodes.
void linkBinomHeaps(struct node *heap1, struct node *heap2) {
    heap1->parent = heap2;                 // The second node will be the parent of the first node.
    heap1->sibling = heap2->child;         // The child of the second node will be the sibling of the first node.
    heap2->child = heap1;                  // The first node will be the child of the second node.
    heap2->degree = heap2->degree +
                    1;     // And finally, the degree of the second node increases by 1 because it has a new child.
}

// This function merges the given two nodes.
struct node *mergeBinomHeaps(struct node *heap1, struct node *heap2) {
    // At first I creates a lot of templates because the program will use all of them separately.
    struct node *tempHeap1, *tempHeap2;
    struct node *tempNode1, *tempNode2;
    struct node *heap;                     // This node will be the merged node.

    // I do these because I don't want to change heap1 and heap2.
    tempHeap1 = heap1;
    tempHeap2 = heap2;

    if (tempHeap1 != NULL) {
        // If the tempHeap1 and tempHeap2 aren't empty and tempHeap1's degree is smaller or equal to tempHeap2's degree
        // then the program assigns the tempHeap1 to heap.
        if (tempHeap2 != NULL && tempHeap1->degree <= tempHeap2->degree)
            heap = tempHeap1;
        // If the tempHeap1's degree is greater than tempHeap2's degree, then the program assigns the tempHeap2 to heap.
        else heap = tempHeap2 != NULL && tempHeap1->degree > tempHeap2->degree ? tempHeap2 : tempHeap1;
    } else {
        // If tempHeap1 is null then the program assigns the tempHeap2 to the heap.
        heap = tempHeap2;
    }

    // With this loop, the program merges the heap1 and heap2 properly.
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
    // After all the operation the function returns the merged heap.
    return heap;
}

// This function creates a union heap from two heaps or insets the node to main heap.
struct node *unionBinomHeaps(struct node *heap1, struct node *heap2) {
    struct node *prevNode, *nextNode, *currentNode;

    // At first, the program merges two heaps.
    HEAP = mergeBinomHeaps(heap1, heap2);

    if (HEAP == NULL)
        return HEAP;

    // Here, the program assigns the nodes values.
    prevNode = NULL;
    currentNode = HEAP;
    nextNode = currentNode->sibling;

    // With this while loop, the program finds where to put heap1 and heap2 then put them.
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
                    HEAP = nextNode;
                } else {
                    prevNode->sibling = nextNode;
                }
                linkBinomHeaps(currentNode, nextNode);
                currentNode = nextNode;
            }
        }
        nextNode = currentNode->sibling;
    }
    return HEAP;
}

// This recursive function reverts the roots' list recursively.
void revertRootsList(struct node *heap) {
    if (heap->sibling != NULL) {
        revertRootsList(heap->sibling);
        (heap->sibling)->sibling = heap;
    } else {
        HEAP2 = heap;
    }
}

// This function extracts the minimum wordRelScore node from heap and returns it.
struct node *extractMinFromHeap(struct node *heap1) {
    struct node *minNode = heap1;
    struct node *tempNode = NULL;
    struct node *currentNode = heap1;
    struct node *tempNode2;

    HEAP2 = NULL;

    if (currentNode == NULL) {
        printf("\nThere is no node to extract from heap.\n");
        return currentNode;
    }
    tempNode2 = minNode;

    // This loop finds the root which has minimum wordRelScore.
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
        revertRootsList(currentNode->child);
        (currentNode->child)->sibling = NULL;
    }

    // Here, the program makes a union heap with the given heap and reverted list.
    HEAP = unionBinomHeaps(heap1, HEAP2);
    return currentNode;
}

// This function prints the most relevant 5 file or if there is no 5 relevant file, prints all relevant files.
void printMostRelFiveFile(char *keyword, int fileCount) {
    for (int i = 0; i < 5; i++) {
        // I use extractMinFromHeap() method for finding the most relevant node because I subtract the wordRelScore from 300.
        struct node *mostRelNode = extractMinFromHeap(HEAP);
        char fileName[56] = "files/";
        char currentChar;

        // If there is no relevant word then the program continues.
        if (300 - mostRelNode->wordRelScore == 0)
            continue;

        // For open the file the program adds the folder's path to fileName.
        strcat(fileName, mostRelNode->fileName);
        FILE *filePtr = fopen(fileName, "r");

        if (filePtr == NULL) {
            printf("Cannot open file!");
            exit(0);
        }
        puts("\n--------------------------------------------------------------------------------------------------");
        printf("In the '%s' named file there is %d keyword: '%s'.\n\n", mostRelNode->fileName,
               300 - mostRelNode->wordRelScore, keyword);

        // This do-while loop prints the whole file character by character.
        do {
            currentChar = fgetc(filePtr);
            printf("%c", currentChar);
        } while (currentChar != EOF);

        fclose(filePtr);
    }
    puts("\n--------------------------------------------------------------------------------------------------");

    // After printing the files the program prints there is how many relevant file.
    if (fileCount == 0)
        printf("There is no file containing the keyword '%s'.\n", keyword);
    else if (fileCount == 1)
        printf("There is only 1 file containing the keyword '%s'.\n", keyword);
    else
        printf("There is %d files containing the keyword '%s'.\n", fileCount, keyword);
}

int main() {
    int counter;
    int fileCount = 0;
    int relFileCount = 0;
    char keyword[30] = "\0";
    char currentWord[30] = "\0";
    char currentChar;

    DIR *dirPtr;
    FILE *filePtr;
    struct dirent *dirStr;

    // Here, the program gets the keyword from the user.
    printf("Enter a keyword:");
    scanf("%s", keyword);

    // This pointer open the files folder.
    dirPtr = opendir("files");
    if (dirPtr == NULL) {
        printf("Could not open current directory!");
        return -1;
    }

    // With this loop the program counts the number of file in the folder.
    for (int i = 0; readdir(dirPtr) != NULL; i++)
        fileCount++;
    closedir(dirPtr);

    // With the fileCount, I created an array of the size of it.
    struct node fileList[fileCount];

    // Here, I empty the fileName strings.
    for (int i = 0; i < fileCount; i++) {
        strcpy(fileList[i].fileName, "\0");
    }

    dirPtr = opendir("files");
    // With this loop I copy the file names from folder to array.
    for (int i = 0; (dirStr = readdir(dirPtr)) != NULL; i++)
        strcpy(fileList[i].fileName, dirStr->d_name);
    closedir(dirPtr);

    // With this loop the program opens the whole files and one by one and finds the relevancy scores.
    for (int i = 0; i < fileCount; i++) {
        struct node *tempNode;
        char fileName[56] = "files/";
        counter = 0;

        // I don't know why but the first two elements that DIR finds are . and .. so for this I skip these file names.
        if (strcmp(fileList[i].fileName, ".") == 0 || strcmp(fileList[i].fileName, "..") == 0)
            continue;

        // The program concatenates the "files/" and fileName for open the file.
        strcat(fileName, fileList[i].fileName);
        filePtr = fopen(fileName, "r");
        if (filePtr == NULL) {
            printf("Cannot open file!");
            exit(0);
        }

        // With this do-while loop the program reads the file character by character.
        do {
            currentChar = fgetc(filePtr);

            // If character is a letter, the program concatenates it with currentWord.
            if ((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) {
                strcat(currentWord, (char[2]) {(char) currentChar, '\0'});
            } else {
                // If character is not a letter but is apostrophe then it will be ignored and continues.
                if (currentChar == 39) {
                    strcpy(currentWord, "\0");
                    continue;
                }

                // If the concatenated word and keyword are equal then the program increments the counter by 1.
                if (stricmp(currentWord, keyword) == 0)
                    counter++;

                strcpy(currentWord, "\0");
            }

        } while (currentChar != EOF);

        // With this if, the program finds that how many file has the keyword.
        if (counter != 0)
            relFileCount++;

        // This line prints the file name and relevancy score.
//        printf("%s\t%d\n", fileName, counter);

        fileList[i].wordRelScore = counter;

        // Here, the program creates the node with the fileName and 300 - wordRelScore.
        tempNode = createNode(fileList[i].fileName, 300 - fileList[i].wordRelScore);

        // Then the program put the created node into the HEAP.
        HEAP = unionBinomHeaps(HEAP, tempNode);

        fclose(filePtr);
    }
    // After the creating of the heap, the program prints most relevant 5 nodes.
    printMostRelFiveFile(keyword, relFileCount);

    return 0;
}
