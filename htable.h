#ifndef HTABLE
#define HTABLE
#include <stdio.h>
#include <stdlib.h>

/* One node */
typedef struct Node
{
    int count;
    int character;
    char codes[10000];
    struct Node* next;
    struct Node* left;
    struct Node* right;
}Node;

/* Function Prototypes */
unsigned int totalChars(Node* head);
int compare(const void* a, const void* b);
int isLeaf(Node* root);
Node** getResults(Node* root, char string[], Node** nodes, int top);
void printList(Node* head);
Node* newNode(int count, int character);
Node* sortedInsert(Node* head, int count, int character);
Node* treeInsert(Node* head, Node* newNode);
Node* merge(Node* first, Node* second);
void fillValues(int array[], unsigned char chars[], int num);
Node* makeList(Node* head, int arr[]);
Node* makeList2(Node* head, unsigned char character, int count);
Node* createTree(Node* head);

#endif
