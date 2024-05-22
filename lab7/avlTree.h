#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_INT 200
#define BUFFER_STR 100

typedef struct NodeAVLTree
{
    int data;
    struct NodeAVLTree *left;
    struct NodeAVLTree *right;
    int height;
    int index;
} NodeAVLTree;

typedef struct AVLTree
{
    NodeAVLTree *head;
    int size;
} AVLTree;

AVLTree *createAVLTree();
NodeAVLTree *createNodeAVLTree(AVLTree *avlTree, int data);
void displayInOrder(NodeAVLTree *node, int depth);
void dispAVLTree(AVLTree *avlTree);
int height(NodeAVLTree *node);
void updateHeight(NodeAVLTree *node);
int balanceFactor(NodeAVLTree *node);
NodeAVLTree *rotateRight(NodeAVLTree *y);
NodeAVLTree *rotateLeft(NodeAVLTree *x);
NodeAVLTree *insertAVLTreeUtil(AVLTree *avlTree, NodeAVLTree *node, int data);
void insertAVLTree(AVLTree *avlTree, int element);
void popRight(AVLTree *avlTree);
void popLeft(AVLTree *avlTree);
NodeAVLTree *searchRecursiveValue(NodeAVLTree *node, int n);
void searchByValue(AVLTree *avlTree, int n);
NodeAVLTree *searchRecursiveIndex(NodeAVLTree *node, int n);
void searchByIndex(AVLTree *avlTree, int n);
void putIndexesUtil(NodeAVLTree *node, int *index);
void putIndexes(AVLTree *avlTree);
NodeAVLTree *minValueNode(NodeAVLTree *node);
NodeAVLTree *deleteNode(NodeAVLTree *root, int key);
void deleteAVLTree(AVLTree *avlTree, int n);
void WriteDataInFileAVLTree(char *string, AVLTree *avlTree);
void writeInOrderToFile(NodeAVLTree *node, int depth, FILE *ptr_w);
void removeFileAVLTree(const char *string);
void printMenuAVLTree();
void avlTreeFunction();

#endif