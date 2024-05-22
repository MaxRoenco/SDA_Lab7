#ifndef B2TREE_H
#define B2TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_STR 100

typedef struct NodeBPTree
{
    int *keys;
    struct NodeBPTree **children;
    int numKeys;
    bool leaf;
    struct NodeBPTree *next;
} NodeBPTree;

typedef struct BPTree
{
    NodeBPTree *root;
    int t;
} BPTree;

NodeBPTree *createNodeBPTree(int t, int leaf);
BPTree *createBPTree(int t);
void splitChildBPTree(NodeBPTree *parent, int i, NodeBPTree *fullChild, int t);
void insertNonFullBPTree(NodeBPTree *node, int k, int idx, int t);
void insertBPTree(BPTree *tree, int k, int idx);
void displayNodeIndentedBPTree(NodeBPTree *node, int depth);
void displayBPTreeIndented(NodeBPTree *node, int depth);
void printBPTreeIndented(BPTree *tree);
void printMenuBPTree();
void deleteBPTree(BPTree *tree, int k);
void deleteKeyFromNodeBPTree(NodeBPTree *node, int k, int t);
void removeFromLeafBPTree(NodeBPTree *node, int idx);
void removeFromNonLeafBPTree(NodeBPTree *node, int idx, int t);
int getPredecessorBPTree(NodeBPTree *node, int idx);
int getSuccessorBPTree(NodeBPTree *node, int idx);
void mergeBPTree(NodeBPTree *node, int idx, int t);
void fillBPTree(NodeBPTree *node, int idx, int t);
void borrowFromPrevBPTree(NodeBPTree *node, int idx, int t);
void borrowFromNextBPTree(NodeBPTree *node, int idx, int t);
int searchIndexByValueBPTree(NodeBPTree *node, int value);
bool searchByKeyBPTree(NodeBPTree *node, int index, int *key);
void WriteDataInFileBPTree(const char *filename, BPTree *tree);
void writeNodeToFileBPTree(NodeBPTree *node, FILE *file);
void removeFileBPTree(const char *filename);
void BPTreeFunction();

#endif // BTREE_H
