#include "avlTree.h"
#include "bTree.h"
#include "b2Tree.h"

void printMainMenu()
{
    printf("\n");
    printf("=========================================\n");
    printf("|            B Tree Operations           |\n");
    printf("=========================================\n");
    printf("| 1. AVL Tree                            |\n");
    printf("| 2. B Tree                              |\n");
    printf("| 3. B+ Tree                             |\n");
    printf("| 4. Quit                                |\n");
    printf("=========================================\n");
}

int main()
{
    int k;
    do
    {
        printMainMenu();
        scanf("%d", &k);
        switch (k)
        {
        case 1:
            avlTreeFunction();
            break;
        case 2:
            bTreeFunction();
            break;
        case 3:
            BPTreeFunction();
            break;
        case 4:
            printf("Exiting...");
            break;
        default:
            break;
        }
    } while (k != 4);
}