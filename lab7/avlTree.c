#include "avlTree.h"

AVLTree *createAVLTree()
{
    AVLTree *avlTree = (AVLTree *)malloc(sizeof(AVLTree));
    avlTree->head = NULL;
    avlTree->size = 0;
    return avlTree;
}

NodeAVLTree *createNodeAVLTree(AVLTree *avlTree, int data)
{
    NodeAVLTree *newNode = (NodeAVLTree *)malloc(sizeof(NodeAVLTree));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    avlTree->size++;
    return newNode;
}

void displayInOrder(NodeAVLTree *node, int depth)
{
    if (node != NULL)
    {
        displayInOrder(node->right, depth + 1);

        for (int i = 0; i < depth; i++)
            printf("     ");

        printf("%d\n", node->data);

        displayInOrder(node->left, depth + 1);
    }
}

void dispAVLTree(AVLTree *avlTree)
{
    printf("AVL Tree:\n");
    displayInOrder(avlTree->head, 0);
    printf("\nSize of AVLTree: %d", avlTree->size);
}

int height(NodeAVLTree *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

void updateHeight(NodeAVLTree *node)
{
    if (node != NULL)
        node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));
}

int balanceFactor(NodeAVLTree *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

NodeAVLTree *rotateRight(NodeAVLTree *y)
{
    NodeAVLTree *x = y->left;
    NodeAVLTree *T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

NodeAVLTree *rotateLeft(NodeAVLTree *x)
{
    NodeAVLTree *y = x->right;
    NodeAVLTree *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

NodeAVLTree *insertAVLTreeUtil(AVLTree *avlTree, NodeAVLTree *node, int data)
{
    if (node == NULL)
        return createNodeAVLTree(avlTree, data);

    if (data < node->data)
        node->left = insertAVLTreeUtil(avlTree, node->left, data);
    else if (data > node->data)
        node->right = insertAVLTreeUtil(avlTree, node->right, data);
    else
        return node;

    updateHeight(node);

    int balance = balanceFactor(node);

    if (balance > 1 && data < node->left->data)
        return rotateRight(node);

    if (balance < -1 && data > node->right->data)
        return rotateLeft(node);

    if (balance > 1 && data > node->left->data)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && data < node->right->data)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void insertAVLTree(AVLTree *avlTree, int element)
{
    avlTree->head = insertAVLTreeUtil(avlTree, avlTree->head, element);
}

void popRight(AVLTree *avlTree)
{
    NodeAVLTree *temp = avlTree->head;
    NodeAVLTree *prev = NULL;
    while (temp->right != NULL)
    {
        prev = temp;
        temp = temp->right;
    }

    if (prev != NULL)
        prev->right = NULL;
    else
        avlTree->head = NULL;

    free(temp);
    avlTree->size--;
}

void popLeft(AVLTree *avlTree)
{
    NodeAVLTree *temp = avlTree->head;
    NodeAVLTree *prev = NULL;
    while (temp->left != NULL)
    {
        prev = temp;
        temp = temp->left;
    }

    if (prev != NULL)
        prev->left = NULL;
    else
        avlTree->head = NULL;

    free(temp);
}

NodeAVLTree *searchRecursiveValue(NodeAVLTree *node, int n)
{
    if (node == NULL)
        return NULL;

    if (node->data == n)
    {
        return node;
    }
    else
    {
        searchRecursiveValue(node->left, n);
        searchRecursiveValue(node->right, n);
    }
}

void searchByValue(AVLTree *avlTree, int n)
{
    NodeAVLTree *node = searchRecursiveValue(avlTree->head, n);
    if (node == NULL)
    {
        printf("\nElement %d was not found", n);
    }
    else
    {
        printf("\nFound element: %d Index: %d", node->data, node->index);
    }
}

NodeAVLTree *searchRecursiveIndex(NodeAVLTree *node, int n)
{
    if (node == NULL)
        return NULL;

    if (node->index == n)
    {
        return node;
    }
    else
    {
        NodeAVLTree *foundNode = searchRecursiveIndex(node->left, n);
        if (foundNode != NULL)
            return foundNode;

        return searchRecursiveIndex(node->right, n);
    }
}

void searchByIndex(AVLTree *avlTree, int n)
{
    NodeAVLTree *node = searchRecursiveIndex(avlTree->head, n);
    if (node == NULL)
    {
        printf("\nElement %d was not found", n);
    }
    else
    {
        printf("\nFound element: %d Index: %d", node->data, node->index);
    }
}

void putIndexesUtil(NodeAVLTree *node, int *index)
{
    if (node == NULL)
        return;

    putIndexesUtil(node->left, index);
    node->index = (*index)++;
    putIndexesUtil(node->right, index);
}

void putIndexes(AVLTree *avlTree)
{
    int index = 1;
    putIndexesUtil(avlTree->head, &index);
}

NodeAVLTree *minValueNode(NodeAVLTree *node)
{
    NodeAVLTree *current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

NodeAVLTree *deleteNode(NodeAVLTree *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else
    {
        if (root->left == NULL)
        {
            NodeAVLTree *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            NodeAVLTree *temp = root->left;
            free(root);
            return temp;
        }

        NodeAVLTree *temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

void deleteAVLTree(AVLTree *avlTree, int n)
{
    if (avlTree == NULL || avlTree->head == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    avlTree->head = deleteNode(avlTree->head, n);
}

void WriteDataInFileAVLTree(char *string, AVLTree *avlTree)
{
    FILE *ptr_w;

    ptr_w = fopen(string, "w");
    if (ptr_w == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    fprintf(ptr_w, "AVL Tree:\n");
    writeInOrderToFile(avlTree->head, 0, ptr_w);
    fprintf(ptr_w, "\nSize of AVLTree: %d", avlTree->size);

    fclose(ptr_w);
}

void writeInOrderToFile(NodeAVLTree *node, int depth, FILE *ptr_w)
{
    if (node != NULL)
    {
        writeInOrderToFile(node->right, depth + 1, ptr_w);

        for (int i = 0; i < depth; i++)
            fprintf(ptr_w, "     ");

        fprintf(ptr_w, "%d\n", node->data);

        writeInOrderToFile(node->left, depth + 1, ptr_w);
    }
}

void removeFileAVLTree(const char *string)
{
    if (remove(string) == 0)
    {
        printf("File '%s' removed successfully.\n", string);
    }
    else
    {
        printf("Unable to remove file '%s'.\n", string);
    }
}

void printMenuAVLTree()
{
    printf("\n");
    printf("=========================================\n");
    printf("|           AVL Tree Operations          |\n");
    printf("=========================================\n");
    printf("| 1. Insert Element                      |\n");
    printf("| 2. Delete Element                      |\n");
    printf("| 3. Search Element by Value             |\n");
    printf("| 4. Search Element by Index             |\n");
    printf("| 5. Display AVL Tree                    |\n");
    printf("| 6. Pop Left Element in AVL Tree        |\n");
    printf("| 7. Pop Right Element in AVL Tree       |\n");
    printf("| 8. Write Data in File                  |\n");
    printf("| 9. Delete File                         |\n");
    printf("| 10. Quit                                |\n");
    printf("=========================================\n");
}

void avlTreeFunction()
{
    AVLTree *avlTree = createAVLTree();
    int choice, element, index;
    char buffer[BUFFER_STR];

    do
    {
        printMenuAVLTree();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter element to insert: ");
            scanf("%d", &element);
            insertAVLTree(avlTree, element);
            break;
        case 2:
            printf("Enter element to delete: ");
            scanf("%d", &element);
            deleteAVLTree(avlTree, element);
            break;
        case 3:
            printf("Enter element to search by value: ");
            scanf("%d", &element);
            searchByValue(avlTree, element);
            break;
        case 4:
            printf("Enter index to search by: ");
            scanf("%d", &index);
            searchByIndex(avlTree, index);
            break;
        case 5:
            dispAVLTree(avlTree);
            break;
        case 6:
            popLeft(avlTree);
            break;
        case 7:
            popRight(avlTree);
            break;
        case 8:
            printf("Input path to file: ");
            getchar();
            fgets(buffer, BUFFER_STR, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            WriteDataInFileAVLTree(buffer, avlTree);
            break;
        case 9:
            printf("Input file which you want to delete: ");
            getchar();
            fgets(buffer, BUFFER_STR, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            removeFileAVLTree(buffer);
            break;
        case 10:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    } while (choice != 10);

    return;
}