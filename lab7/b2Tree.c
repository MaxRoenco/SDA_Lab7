#include "b2Tree.h"

NodeBPTree *createNodeBPTree(int t, int leaf)
{
    NodeBPTree *newNode = (NodeBPTree *)malloc(sizeof(NodeBPTree));
    newNode->keys = (int *)malloc((2 * t - 1) * sizeof(int));
    newNode->children = (NodeBPTree **)malloc((2 * t) * sizeof(NodeBPTree *));
    newNode->numKeys = 0;
    newNode->leaf = leaf;
    newNode->next = NULL;
    return newNode;
}

BPTree *createBPTree(int t)
{
    BPTree *tree = (BPTree *)malloc(sizeof(BPTree));
    tree->root = NULL;
    tree->t = t;
    return tree;
}

void splitChildBPTree(NodeBPTree *parent, int i, NodeBPTree *fullChild, int t)
{
    NodeBPTree *newNode = createNodeBPTree(t, fullChild->leaf);
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
    else
    {
        newNode->next = fullChild->next;
        fullChild->next = newNode;
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

void insertNonFullBPTree(NodeBPTree *node, int k, int idx, int t)
{
    int i = node->numKeys - 1;

    if (node->leaf)
    {
        while (i >= 0 && node->keys[i] > k)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = k;
        node->numKeys++;
    }
    else
    {
        while (i >= 0 && node->keys[i] > k)
        {
            i--;
        }
        i++;
        if (node->children[i]->numKeys == 2 * t - 1)
        {
            splitChildBPTree(node, i, node->children[i], t);
            if (node->keys[i] < k)
            {
                i++;
            }
        }
        insertNonFullBPTree(node->children[i], k, idx, t);
    }
}

void insertBPTree(BPTree *tree, int k, int idx)
{
    NodeBPTree *root = tree->root;

    if (root == NULL)
    {
        root = createNodeBPTree(tree->t, 1);
        root->keys[0] = k;
        root->numKeys = 1;
        tree->root = root;
    }
    else
    {
        if (root->numKeys == 2 * tree->t - 1)
        {
            NodeBPTree *newRoot = createNodeBPTree(tree->t, 0);
            newRoot->children[0] = root;
            splitChildBPTree(newRoot, 0, root, tree->t);
            int i = 0;
            if (newRoot->keys[0] < k)
            {
                i++;
            }
            insertNonFullBPTree(newRoot->children[i], k, idx, tree->t);
            tree->root = newRoot;
        }
        else
        {
            insertNonFullBPTree(root, k, idx, tree->t);
        }
    }
}

void displayNodeIndentedBPTree(NodeBPTree *node, int depth)
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

void displayBPTreeIndented(NodeBPTree *node, int depth)
{
    if (node != NULL)
    {
        if (!node->leaf)
        {
            for (int i = 0; i <= node->numKeys; i++)
            {
                displayBPTreeIndented(node->children[i], depth + 1);
            }
        }
        displayNodeIndentedBPTree(node, depth);
        if (node->leaf)
        {
            if (node->next != NULL)
            {
                displayNodeIndentedBPTree(node->next, depth);
            }
        }
    }
}

void printBPTreeIndented(BPTree *tree)
{
    printf("B+ Tree:\n");
    displayBPTreeIndented(tree->root, 0);
    printf("\n");
}

void printMenuBPTree()
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

void deleteBPTree(BPTree *tree, int k)
{
    if (!tree->root)
    {
        printf("The tree is empty\n");
        return;
    }

    deleteKeyFromNodeBPTree(tree->root, k, tree->t);

    if (tree->root->numKeys == 0)
    {
        NodeBPTree *tmp = tree->root;
        if (tree->root->leaf)
        {
            tree->root = NULL;
        }
        else
        {
            tree->root = tree->root->children[0];
        }
        free(tmp->keys);
        free(tmp->children);
        free(tmp);
    }
}

void deleteKeyFromNodeBPTree(NodeBPTree *node, int k, int t)
{
    int idx = 0;
    while (idx < node->numKeys && node->keys[idx] < k)
    {
        idx++;
    }

    if (idx < node->numKeys && node->keys[idx] == k)
    {
        if (node->leaf)
        {
            removeFromLeafBPTree(node, idx);
        }
        else
        {
            removeFromNonLeafBPTree(node, idx, t);
        }
    }
    else
    {
        if (node->leaf)
        {
            printf("The key %d is not in the tree\n", k);
            return;
        }

        bool flag = (idx == node->numKeys);
        if (node->children[idx]->numKeys < t)
        {
            fillBPTree(node, idx, t);
        }

        if (flag && idx > node->numKeys)
        {
            deleteKeyFromNodeBPTree(node->children[idx - 1], k, t);
        }
        else
        {
            deleteKeyFromNodeBPTree(node->children[idx], k, t);
        }
    }
}

void removeFromLeafBPTree(NodeBPTree *node, int idx)
{
    for (int i = idx + 1; i < node->numKeys; i++)
    {
        node->keys[i - 1] = node->keys[i];
    }
    node->numKeys--;
}

void removeFromNonLeafBPTree(NodeBPTree *node, int idx, int t)
{
    int k = node->keys[idx];

    if (node->children[idx]->numKeys >= t)
    {
        int pred = getPredecessorBPTree(node, idx);
        node->keys[idx] = pred;
        deleteKeyFromNodeBPTree(node->children[idx], pred, t);
    }
    else if (node->children[idx + 1]->numKeys >= t)
    {
        int succ = getSuccessorBPTree(node, idx);
        node->keys[idx] = succ;
        deleteKeyFromNodeBPTree(node->children[idx + 1], succ, t);
    }
    else
    {
        mergeBPTree(node, idx, t);
        deleteKeyFromNodeBPTree(node->children[idx], k, t);
    }
}

int getPredecessorBPTree(NodeBPTree *node, int idx)
{
    NodeBPTree *cur = node->children[idx];
    while (!cur->leaf)
    {
        cur = cur->children[cur->numKeys];
    }
    return cur->keys[cur->numKeys - 1];
}

int getSuccessorBPTree(NodeBPTree *node, int idx)
{
    NodeBPTree *cur = node->children[idx + 1];
    while (!cur->leaf)
    {
        cur = cur->children[0];
    }
    return cur->keys[0];
}

void mergeBPTree(NodeBPTree *node, int idx, int t)
{
    NodeBPTree *child = node->children[idx];
    NodeBPTree *sibling = node->children[idx + 1];

    child->keys[t - 1] = node->keys[idx];

    for (int i = 0; i < sibling->numKeys; i++)
    {
        child->keys[i + t] = sibling->keys[i];
    }

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->numKeys; i++)
        {
            child->children[i + t] = sibling->children[i];
        }
    }

    for (int i = idx + 1; i < node->numKeys; i++)
    {
        node->keys[i - 1] = node->keys[i];
    }

    for (int i = idx + 2; i <= node->numKeys; i++)
    {
        node->children[i - 1] = node->children[i];
    }

    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

void fillBPTree(NodeBPTree *node, int idx, int t)
{
    if (idx != 0 && node->children[idx - 1]->numKeys >= t)
    {
        borrowFromPrevBPTree(node, idx, t);
    }
    else if (idx != node->numKeys && node->children[idx + 1]->numKeys >= t)
    {
        borrowFromNextBPTree(node, idx, t);
    }
    else
    {
        if (idx != node->numKeys)
        {
            mergeBPTree(node, idx, t);
        }
        else
        {
            mergeBPTree(node, idx - 1, t);
        }
    }
}

void borrowFromPrevBPTree(NodeBPTree *node, int idx, int t)
{
    NodeBPTree *child = node->children[idx];
    NodeBPTree *sibling = node->children[idx - 1];

    for (int i = child->numKeys - 1; i >= 0; i--)
    {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->leaf)
    {
        for (int i = child->numKeys; i >= 0; i--)
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
    child->numKeys++;
    sibling->numKeys--;
}

void borrowFromNextBPTree(NodeBPTree *node, int idx, int t)
{
    NodeBPTree *child = node->children[idx];
    NodeBPTree *sibling = node->children[idx + 1];

    child->keys[child->numKeys] = node->keys[idx];

    if (!child->leaf)
    {
        child->children[child->numKeys + 1] = sibling->children[0];
    }

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->numKeys; i++)
    {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->numKeys; i++)
        {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->numKeys++;
    sibling->numKeys--;
}

int searchIndexByValueBPTree(NodeBPTree *node, int value)
{
    int index = 0;
    while (index < node->numKeys && node->keys[index] < value)
    {
        index++;
    }
    if (index < node->numKeys && node->keys[index] == value)
    {
        return index;
    }
    else
    {
        if (node->leaf)
        {
            return -1;
        }
        else
        {
            return searchIndexByValueBPTree(node->children[index], value);
        }
    }
}

bool searchByKeyBPTree(NodeBPTree *node, int index, int *key)
{
    if (index >= 0 && index < node->numKeys)
    {
        *key = node->keys[index];
        return true;
    }
    else
    {
        return false;
    }
}

void WriteDataInFileBPTree(const char *filename, BPTree *tree)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file %s\n", filename);
        return;
    }

    writeNodeToFileBPTree(tree->root, file);

    fclose(file);
}

void writeNodeToFileBPTree(NodeBPTree *node, FILE *file)
{
    if (node == NULL)
    {
        return;
    }

    for (int i = 0; i < node->numKeys; i++)
    {
        fprintf(file, "%d ", node->keys[i]);
    }
    fprintf(file, "\n");

    if (!node->leaf)
    {
        for (int i = 0; i <= node->numKeys; i++)
        {
            writeNodeToFileBPTree(node->children[i], file);
        }
    }
}

void removeFileBPTree(const char *filename)
{
    int status = remove(filename);
    if (status == 0)
    {
        printf("File %s removed successfully.\n", filename);
    }
    else
    {
        printf("Error removing file %s\n", filename);
    }
}

void BPTreeFunction()
{
    int index = 1;
    int t = 3;
    int choice, element;
    char buffer[BUFFER_STR];
    BPTree *tree = createBPTree(t);
    do
    {
        printMenuBPTree();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter element to insert: ");
            scanf("%d", &element);
            insertBPTree(tree, element, index);
            index++;
            break;
        case 2:
            printf("Enter element to delete: ");
            scanf("%d", &element);
            deleteBPTree(tree, element);
            index--;
            break;
        case 3:
            printf("Enter element to search by value: ");
            scanf("%d", &element);
            int indexByValue = searchIndexByValueBPTree(tree->root, element);
            if (indexByValue != -1)
                printf("Index of element %d is: %d\n", element, indexByValue);
            else
                printf("Element %d not found in the B+ tree\n", element);
            break;
        case 4:
            printf("Enter index to search by: ");
            scanf("%d", &index);
            int keyByIndex;
            if (searchByKeyBPTree(tree->root, index, &keyByIndex))
                printf("Key at index %d is: %d\n", index, keyByIndex);
            else
                printf("Index %d out of bounds or B+ tree is empty\n", index);
            break;
        case 5:
            printBPTreeIndented(tree);
            break;
        case 6:
            printf("Input path to file: ");
            getchar();
            fgets(buffer, BUFFER_STR, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            WriteDataInFileBPTree(buffer, tree);
            break;
        case 7:
            printf("Input file which you want to delete: ");
            getchar();
            fgets(buffer, BUFFER_STR, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            removeFileBPTree(buffer);
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
