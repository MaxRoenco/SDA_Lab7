#include "bTree.h"

void displayNodeIndented(NodeBTree *node, int depth)
{
    for (int i = 0; i < depth; i++)
    {
        printf("     ");
    }

    for (int i = 0; i < node->numKeys; i++)
    {
        printf("%d ", node->keys[i]);
    }
    printf("\n");
}

void displayBTreeIndented(NodeBTree *node, int depth)
{
    if (node != NULL)
    {
        if (!node->leaf)
        {
            displayBTreeIndented(node->children[node->numKeys], depth + 1);
        }

        displayNodeIndented(node, depth);

        for (int i = node->numKeys - 1; i >= 0; i--)
        {
            if (!node->leaf)
            {
                displayBTreeIndented(node->children[i], depth + 1);
            }
        }
    }
}

void printBTreeIndented(bTree *tree)
{
    printf("B-Tree:\n");
    displayBTreeIndented(tree->root, 0);
    printf("\n");
}

bTree *createBTree(int t)
{
    bTree *newTree = (bTree *)malloc(sizeof(bTree));
    newTree->t = t;
    newTree->root = NULL;
    return newTree;
}

NodeBTree *createNode(int t, int leaf)
{
    NodeBTree *newNode = (NodeBTree *)malloc(sizeof(NodeBTree));
    newNode->keys = (KeyValue *)malloc((2 * t - 1) * sizeof(KeyValue));
    newNode->children = (NodeBTree **)malloc((2 * t) * sizeof(NodeBTree *));
    newNode->numKeys = 0;
    newNode->leaf = leaf;
    return newNode;
}

void splitChild(NodeBTree *parent, int i, NodeBTree *fullChild, int t)
{
    NodeBTree *newNode = createNode(t, fullChild->leaf);
    newNode->numKeys = t - 1;

    for (int j = 0; j < t - 1; j++)
    {
        newNode->keys[j] = fullChild->keys[j + t];
    }

    if (!fullChild->leaf)
    {
        for (int j = 0; j < t; j++)
        {
            newNode->children[j] = fullChild->children[j + t];
        }
    }

    fullChild->numKeys = t - 1;

    for (int j = parent->numKeys; j >= i + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[i + 1] = newNode;

    for (int j = parent->numKeys - 1; j >= i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = fullChild->keys[t - 1];
    parent->numKeys++;
}

void insertNonFull(NodeBTree *node, int k, int idx, int t)
{
    int i = node->numKeys - 1;

    if (node->leaf)
    {
        while (i >= 0 && node->keys[i].key > k)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1].key = k;
        node->keys[i + 1].index = idx;
        node->numKeys++;
    }
    else
    {
        while (i >= 0 && node->keys[i].key > k)
        {
            i--;
        }
        i++;
        if (node->children[i]->numKeys == 2 * t - 1)
        {
            splitChild(node, i, node->children[i], t);
            if (node->keys[i].key < k)
            {
                i++;
            }
        }
        insertNonFull(node->children[i], k, idx, t);
    }
}

void insertBTree(bTree *tree, int k, int idx)
{
    NodeBTree *root = tree->root;

    if (root == NULL)
    {
        root = createNode(tree->t, 1);
        root->keys[0].key = k;
        root->keys[0].index = idx;
        root->numKeys = 1;
        tree->root = root;
    }
    else
    {
        if (root->numKeys == 2 * tree->t - 1)
        {
            NodeBTree *newRoot = createNode(tree->t, 0);
            newRoot->children[0] = root;
            splitChild(newRoot, 0, root, tree->t);
            int i = 0;
            while (i < newRoot->numKeys && newRoot->keys[i].key < k)
            {
                i++;
            }
            insertNonFull(newRoot->children[i], k, idx, tree->t);
            tree->root = newRoot;
        }
        else
        {
            int i = 0;
            while (i < root->numKeys && root->keys[i].key < k)
            {
                i++;
            }
            insertNonFull(root, k, idx, tree->t);
        }
    }
}

int getPred(NodeBTree *node, int idx)
{
    NodeBTree *cur = node->children[idx];
    while (!cur->leaf)
    {
        cur = cur->children[cur->numKeys];
    }
    return cur->keys[cur->numKeys - 1].key;
}

int getSucc(NodeBTree *node, int idx)
{
    NodeBTree *cur = node->children[idx + 1];
    while (!cur->leaf)
    {
        cur = cur->children[0];
    }
    return cur->keys[0].key;
}

void merge(NodeBTree *node, int idx, int t)
{
    NodeBTree *child = node->children[idx];
    NodeBTree *sibling = node->children[idx + 1];

    child->keys[t - 1] = node->keys[idx];

    for (int i = 0; i < sibling->numKeys; ++i)
    {
        child->keys[i + t] = sibling->keys[i];
    }

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->numKeys; ++i)
        {
            child->children[i + t] = sibling->children[i];
        }
    }

    for (int i = idx + 1; i < node->numKeys; ++i)
    {
        node->keys[i - 1] = node->keys[i];
    }

    for (int i = idx + 2; i <= node->numKeys; ++i)
    {
        node->children[i - 1] = node->children[i];
    }

    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

void fill(NodeBTree *node, int idx, int t)
{
    if (idx != 0 && node->children[idx - 1]->numKeys >= t)
    {
        borrowFromPrev(node, idx, t);
    }
    else if (idx != node->numKeys && node->children[idx + 1]->numKeys >= t)
    {
        borrowFromNext(node, idx, t);
    }
    else
    {
        if (idx != node->numKeys)
        {
            merge(node, idx, t);
        }
        else
        {
            merge(node, idx - 1, t);
        }
    }
}

void borrowFromNext(NodeBTree *node, int idx, int t)
{
    NodeBTree *child = node->children[idx];
    NodeBTree *sibling = node->children[idx + 1];

    child->keys[child->numKeys] = node->keys[idx];

    if (!child->leaf)
    {
        child->children[child->numKeys + 1] = sibling->children[0];
    }

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->numKeys; ++i)
    {
        sibling->keys[i - 1] = sibling->keys[i];
    }
    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->numKeys; ++i)
        {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->numKeys++;
    sibling->numKeys--;
}

void borrowFromPrev(NodeBTree *node, int idx, int t)
{
    NodeBTree *child = node->children[idx];
    NodeBTree *sibling = node->children[idx - 1];

    for (int i = child->numKeys - 1; i >= 0; --i)
    {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->leaf)
    {
        for (int i = child->numKeys; i >= 0; --i)
        {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = node->keys[idx - 1];

    if (!child->leaf)
    {
        child->children[0] = sibling->children[sibling->numKeys];
    }
    node->keys[idx - 1] = sibling->keys[sibling->numKeys - 1];

    child->numKeys += 1;
    sibling->numKeys -= 1;
}

void deleteBTree(bTree *tree, int k)
{
    if (!tree || !tree->root)
        return;

    deleteFromNode(tree->root, k, tree->t);

    if (tree->root->numKeys == 0 && !tree->root->leaf)
    {
        NodeBTree *tmp = tree->root;
        tree->root = tree->root->children[0];
        free(tmp->keys);
        free(tmp->children);
        free(tmp);
    }
}

int findKey(NodeBTree *node, int k)
{
    int idx = 0;
    while (idx < node->numKeys && k > node->keys[idx].key)
    {
        idx++;
    }
    return idx;
}

void deleteFromNode(NodeBTree *node, int k, int t)
{
    int idx = findKey(node, k);

    if (idx < node->numKeys && node->keys[idx].key == k)
    {
        if (node->leaf)
        {
            removeFromLeaf(node, idx);
        }
        else
        {
            removeFromNonLeaf(node, idx, t);
        }
    }
    else
    {
        if (node->leaf)
        {
            printf("Key %d does not exist in the B-tree\n", k);
            return;
        }
        bool lastChild = (idx == node->numKeys);
        if (node->children[idx]->numKeys < t)
            fill(node, idx, t);

        if (lastChild && idx > node->numKeys)
            deleteFromNode(node->children[idx - 1], k, t);
        else
            deleteFromNode(node->children[idx], k, t);
    }
}

void removeFromLeaf(NodeBTree *node, int idx)
{
    for (int i = idx + 1; i < node->numKeys; ++i)
        node->keys[i - 1] = node->keys[i];

    node->numKeys--;
}

void removeFromNonLeaf(NodeBTree *node, int idx, int t)
{
    int k = node->keys[idx].key;

    if (node->children[idx]->numKeys >= t)
    {
        int pred = getPred(node, idx);
        node->keys[idx].key = pred;
        deleteFromNode(node->children[idx], pred, t);
    }

    else if (node->children[idx + 1]->numKeys >= t)
    {
        int succ = getSucc(node, idx);
        node->keys[idx].key = succ;
        deleteFromNode(node->children[idx + 1], succ, t);
    }

    else
    {
        mergeNodes(node, idx, t);
        deleteFromNode(node->children[idx], k, t);
    }
}

int searchByKey(NodeBTree *node, int key, int *result)
{
    if (node == NULL)
        return 0;

    int i = 0;
    while (i < node->numKeys && key > node->keys[i].key)
        i++;

    if (i < node->numKeys && key == node->keys[i].key)
    {
        *result = i;
        return 1;
    }

    if (node->leaf)
    {
        *result = -1;
        return 0;
    }
    return searchByKey(node->children[i], key, result);
}

int searchIndexByValue(NodeBTree *node, int value)
{
    if (node == NULL)
        return -1;

    int i = 0;
    while (i < node->numKeys && value > node->keys[i].key)
        i++;

    if (i < node->numKeys && value == node->keys[i].key)
        return node->keys[i].index;

    if (node->leaf)
        return -1;

    return searchIndexByValue(node->children[i], value);
}

void mergeNodes(NodeBTree *node, int idx, int t)
{
    NodeBTree *child = node->children[idx];
    NodeBTree *sibling = node->children[idx + 1];

    child->keys[t - 1] = node->keys[idx];

    for (int i = 0; i < sibling->numKeys; ++i)
        child->keys[i + t] = sibling->keys[i];

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->numKeys; ++i)
            child->children[i + t] = sibling->children[i];
    }

    for (int i = idx + 1; i < node->numKeys; ++i)
        node->keys[i - 1] = node->keys[i];

    for (int i = idx + 2; i <= node->numKeys; ++i)
        node->children[i - 1] = node->children[i];

    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

void WriteDataInFileBTree(const char *filename, bTree *tree)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    WriteBTreeToFile(tree->root, file);
    fclose(file);
}

void WriteBTreeToFile(NodeBTree *node, FILE *file)
{
    if (node != NULL)
    {
        for (int i = 0; i < node->numKeys; i++)
        {
            fprintf(file, "%d ", node->keys[i].key);
        }
        for (int i = 0; i <= node->numKeys; i++)
        {
            WriteBTreeToFile(node->children[i], file);
        }
    }
}

void removeFileBTree(const char *filename)
{
    if (remove(filename) == 0)
    {
        printf("File deleted successfully.\n");
    }
    else
    {
        printf("Error deleting file.\n");
    }
}

void printMenuBTree()
{
    printf("\n");
    printf("=========================================\n");
    printf("|            B Tree Operations           |\n");
    printf("=========================================\n");
    printf("| 1. Insert Element                      |\n");
    printf("| 2. Delete Element                      |\n");
    printf("| 3. Search Element by Value             |\n");
    printf("| 4. Search Element by Index             |\n");
    printf("| 5. Display B Tree                      |\n");
    printf("| 6. Write Data in File                  |\n");
    printf("| 7. Delete File                         |\n");
    printf("| 8. Quit                                |\n");
    printf("=========================================\n");
}

void bTreeFunction()
{
    int index = 1;
    int t = 3;
    int choice, element;
    char buffer[BUFFER_STR];
    bTree *tree = createBTree(t);
    do
    {
        printMenuBTree();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter element to insert: ");
            scanf("%d", &element);
            insertBTree(tree, element, index);
            index++;
            break;
        case 2:
            printf("Enter element to delete: ");
            scanf("%d", &element);
            deleteBTree(tree, element);
            index--;
            break;
        case 3:
            printf("Enter element to search by value: ");
            scanf("%d", &element);
            int indexByValue = searchIndexByValue(tree->root, element);
            if (indexByValue != -1)
                printf("Index of element %d is: %d\n", element, indexByValue);
            else
                printf("Element %d not found in the B-tree\n", element);
            break;
        case 4:
            printf("Enter index to search by: ");
            scanf("%d", &index);
            int keyByIndex;
            if (searchByKey(tree->root, index, &keyByIndex))
                printf("Key at index %d is: %d\n", index, keyByIndex);
            else
                printf("Index %d out of bounds or B-tree is empty\n", index);
            break;
        case 5:
            printBTreeIndented(tree);
            break;
        case 6:
            printf("Input path to file: ");
            getchar();
            fgets(buffer, BUFFER_STR, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            WriteDataInFileBTree(buffer, tree);
            break;
        case 7:
            printf("Input file which you want to delete: ");
            getchar();
            fgets(buffer, BUFFER_STR, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            removeFileBTree(buffer);
            break;
        case 8:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice.\n");
            break;
        }
    } while (choice != 8);

    return;
}