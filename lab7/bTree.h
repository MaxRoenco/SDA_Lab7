#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_STR 100

typedef struct KeyValue
{
    int key;
    int index;
} KeyValue;

typedef struct NodeBTree
{
    struct KeyValue *keys;
    struct NodeBTree **children;
    int numKeys;
    int leaf;
} NodeBTree;

typedef struct bTree
{
    NodeBTree *root;
    int t;
} bTree;

void displayNodeIndented(NodeBTree *node, int depth);
void displayBTreeIndented(NodeBTree *node, int depth);
void printBTreeIndented(bTree *tree);
bTree *createBTree(int t);
NodeBTree *createNode(int t, int leaf);
void splitChild(NodeBTree *parent, int i, NodeBTree *fullChild, int t);
void insertNonFull(NodeBTree *node, int k, int idx, int t);
void insertBTree(bTree *tree, int k, int idx);
int getPred(NodeBTree *node, int idx);
int getSucc(NodeBTree *node, int idx);
void merge(NodeBTree *node, int idx, int t);
void fill(NodeBTree *node, int idx, int t);
void borrowFromNext(NodeBTree *node, int idx, int t);
void borrowFromPrev(NodeBTree *node, int idx, int t);
void deleteBTree(bTree *tree, int k);
int findKey(NodeBTree *node, int k);
void deleteFromNode(NodeBTree *node, int k, int t);
void removeFromLeaf(NodeBTree *node, int idx);
void removeFromNonLeaf(NodeBTree *node, int idx, int t);
void mergeNodes(NodeBTree *node, int idx, int t);
void printMenuBTree();
int searchByKey(NodeBTree *node, int key, int *result);
int searchIndexByValue(NodeBTree *node, int value);
void WriteDataInFileBTree(const char *filename, bTree *tree);
void WriteBTreeToFile(NodeBTree *node, FILE *file);
void removeFileBTree(const char *filename);
void bTreeFunction();

#endif
