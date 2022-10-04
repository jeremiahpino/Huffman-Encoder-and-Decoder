#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include "htable.h"
#define SIZE 1000000
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

int main(int argc, char* argv[])
{
    /* file variables */
    int in, out;
    unsigned char bufChar;
    int uniqTotal;
    int i, k;
    unsigned char character;
    uint32_t charCount;
    Node* head = NULL;
    Node* root = NULL;
    unsigned char testBuf[SIZE];
    int num, number;
    int totalF = 0;

    /* decode variables */
    uint8_t* oneByte = calloc(SIZE, sizeof(oneByte));
    uint8_t* buf = malloc(SIZE * sizeof(buf));
    int j;
    int bitCount = 0;

    if(oneByte == NULL)
    {
	perror("malloc");
	exit(EXIT_FAILURE);
    }

    if(buf == NULL)
    {
	perror("malloc");
	exit(EXIT_FAILURE);
    }

    /* check if infile available */ 
    if(argv[1] != NULL)
    {
       /* if file is - take input from stdin */ 
        if(strcmp(argv[1], "-"))
        {
            in = STDIN_FILENO;
        }
     /* open file */       
        in = open(argv[1], O_RDONLY);
 
    }
    else
    {
	/* input from stdin */
        in = STDIN_FILENO;
    }

/* check if output file exists */
    if(argv[2] != NULL)
    {

	/* check if file is empty */
        if(!pread(in, testBuf, SIZE, 1))
        {
            out = creat(argv[2], 0644);
            close(out);
            exit(EXIT_FAILURE);
        }
	/* create outfile */
        out = creat(argv[2], 0644);
    }
    else
    {
	/* outfile to stdout */
        out = STDOUT_FILENO;
    }
    
    /* get unique count */
    read(in, &bufChar, 1);
    uniqTotal = 1 + (uint8_t)bufChar;

    /*
    printf("uniq Total:%d\n", uniqTotal);
    */    

    /* use unique count to get iterate through header */
    for(i = 0; i < uniqTotal; i++)
    {
	/* read header contents */
        read(in, &character, 1);
        read(in, &charCount, 4);
        charCount = ntohl(charCount);
	totalF += charCount; 
     
	/* create linked list */   
        head = makeList2(head, character, charCount);
    } 
    
    /* create huffman tree */
    head = root = createTree(head);

    /* check if root is child node */
    if(isLeaf(root))
    {
	/* iterate through root count and write characters */
	for(k = 0; k < root->count; k++)
	{
		write(out, &head->character, 1);
	}
	/* close file and return */	
	close(out);
	return 0;
    }

    /* iterate through rest of buffer and generate characters */
    while((num = read(in, oneByte, SIZE)) > 0)
    {
	/* iterate through byte returned from read */
        for(i = 0; i < num; i++)
        {
		/* increment through byte and while frequency is above 0 */
            for(j = 0; j < 8 && totalF > 0; j++, bitCount++)
            {
		/* check if bit is 1 */
             if(CHECK_BIT(oneByte[bitCount/8], (7 - j)))
             {
		/* if leaf write character */
                head = head->right;
                if(isLeaf(head))
                {
                    write(out, &head->character, 1);
                    head = root;
	 	    totalF--;
                }
             }
             else
             {
		/* if leaf write character */
                head = head->left;
                if(isLeaf(head))
                {
                    write(out, &head->character, 1);
                    head = root;  
		    totalF--;  
                }
              }
            }
         }
     }

     /* check if write failed */
    if(number == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    return 0;
}

