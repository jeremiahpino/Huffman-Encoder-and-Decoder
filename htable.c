#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"


/*get total number of unique characters */
unsigned int totalChars(Node* head)
{
    int total = 0;
    Node* start = head;
   /* iterate through list and count uniq chars */
    while(start != NULL)
    {
        total++;
        start = start->next;
        if(start == NULL)
        {
            break;
        }
    }
    return total;
}

/* compare function to sort in alphabetic order */
int compare(const void* a, const void* b)
{
    const Node* first = *(Node**)a;
    const Node* second = *(Node**)b;
    
    if(!first) return 1;
    if(!second) return 1;
    return (first->character) - (second->character);
}

/* determine if node is a leaf */
int isLeaf(Node* root)
{
    return !(root->left) && !(root->right);
}

/* get the Huffman codes and store in each node */
Node** getResults(Node* root, char string[], Node** nodes, int top)
{
    static int level = 0;
    /* iterate through root left and right nodes until leaf */
    if(root->left)
    {
        string[top] = '0';
        getResults(root->left, string, nodes, top + 1);
    }
    if(root->right)
    {
        string[top] = '1';
        getResults(root->right, string, nodes, top + 1);
    }
    if(isLeaf(root))
    {
        string[top] = '\0';
        strcpy(root->codes, string);
        /* assign root to nodes */
        nodes[level++] = root;
    }
    return nodes;
} 

/* print Linked List */
void printList(Node* head)
{
    Node* start = head;
    while(start != NULL)
    {
        printf("Character: %d and Count: %d\n", start->character, start->count);
        start = start->next;
    }
}

/* create a new Node */
Node* newNode(int count, int character)
{
    /* assign values to new Node */
    Node* newNode = (Node*)malloc(sizeof(newNode));
    newNode->count = count;
    newNode->character = character;
    newNode->next = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* Build Huffman Tree Function */
Node* treeInsert(Node* head, Node* newNode)
{
    Node* current = NULL;
    
    /* replace head if NULL or newNode count is less than */    
    if(head == NULL || newNode->count < head->count)
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        current = head;
        /* place new node in order of count */
        while(current->next != NULL && current->next->count < newNode->count)
        {
            current = current->next;
        }
        /* assign newNode next pointer with current next pointer address */
        newNode->next = current->next;

        /* assign current next pointer with newNode address */
        current->next = newNode;
    }
    return head;
}

/* Sorted insert into Linked List */
Node* sortedInsert(Node* head, int count, int character)
{
    Node* current = NULL;
    Node* newP = NULL;

    newP = newNode(count, character);
     
    if(head == NULL || count < head->count)
    {
        newP->next = head;
        head = newP;
    }
    else
    {
        current = head;
        /* place new node in order of count */
        while(current->next != NULL && current->next->count <= count)
        {
            current = current->next;
        }
        /* newNode next pointer holds current next address */
        newP->next = current->next;

        /* current next pointer holds newP address */
        current->next = newP;
    }
    return head;
}
       
/* merge two nodes into one node */
Node* merge(Node* first, Node* second)
{
    Node* newP = NULL;

    if(first == NULL || second == NULL) return NULL;
    first->next = NULL;
    second->next = NULL;
    
    /* create new Node to return */
    newP = (Node*)malloc(sizeof(Node));
    newP->left = first;
    newP->right = second;

    /* new node count is first + second counts */
    newP->count = first->count + second->count;
    return newP;
}
 
/* WRITE HEADERS IN H FILE */

/* fill values */
void fillValues(int array[], unsigned char chars[], int num)
{
    int b;
    int c = 0;

    for(b = 0; b < num; b++)
    {
        c = (int)chars[b];
        array[c]++;
    }
}

/* create new nodes */
Node* makeList(Node* head, int array[])
{
    int j = 0;
    for(; j < 256; j++)
    {
        if(array[j] > 0)
        {
	    /* create list by count and character */
            if(head == NULL)
            {
                head = newNode(array[j], j);
            }
            else
            {
                head = sortedInsert(head, array[j], j);
            }
        }
    }
    return head;
}

/* create linked list */
Node* makeList2(Node* head, unsigned char character, int count)
{
    /* create list by count and character */
    if(head == NULL)
    {
        head = newNode(count, character);
    }
    else
    {
        head = sortedInsert(head, count, character);
    }
    
    return head;
} 

/* create huffman tree */
Node* createTree(Node* head)
{
    Node* first = NULL;
    Node* second = NULL;
    Node* addBack = NULL;

   /* take first two nodes of linked list and combine */
    while(head != NULL && head->next != NULL)
    {
        first = head;
        second = head->next;

        head = head->next->next;
    
        addBack = merge(first, second);
        if(head == NULL)
        {
            head = addBack;
            break;
        }   
        head = treeInsert(head, addBack);
    }
    return head;
}
        
