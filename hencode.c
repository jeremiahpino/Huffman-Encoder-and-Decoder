#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include "htable.h"
#define SIZE 4096

/* Prototypes */
void createHash(Node** nodes, Node** ht);

int main (int argc, char* argv[])
{
    /* file variables */
    int in, out;
    char* inName = NULL;
    char* outName = NULL;
    int num;
    unsigned char buf[SIZE];
    int charArray[256] = {0};
    Node* head = NULL;
    Node** nodes = malloc(256 * sizeof(Node*));
    Node** ht = malloc(256 * sizeof(Node*));
    char string[100];
    uint8_t total = 0;
    uint8_t c = 0;
    uint32_t count = 0;
    int i = 0;
    int k = 0;

/* encode variables */
    int bitSize = 0;
    uint8_t byte = 0;
    uint8_t bit = 0;
    char* newCode;
    uint8_t buff[SIZE];
    uint8_t body;
    int j = 0;
    int number = 0;
    int index;

/* check if file is here */
    if(argc > 1)
    {
        inName = argv[1];
        if((in = open(inName, O_RDONLY)) == -1)
        {
            perror(inName);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* no file */
        printf("Format: hencode infile [outfile]\n");
        exit(EXIT_FAILURE);
    }

    /* open outfile */
    if(argv[2] != NULL)
    {
	outName = argv[2];
    }

   /* check if file is empty */		
    if(!pread(in, buf, SIZE, 1))
    {
	/* open empty file then close */
        out = open(outName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        close(out); 
        exit(EXIT_FAILURE);
    }
   
    /* read from buf and fill values into charArray */ 
    while((num = read(in, buf, SIZE)) > 0)
    {
         fillValues(charArray, buf, num);
         number = num;
    }

/* close input fd */
    close(in);
     
    /* create linked list */
    head = makeList(head, charArray);

    /* get total number of unique characters */
    total = totalChars(head);
    total = total - 1;

    /* create Huffman Tree */
    head = createTree(head);

    /* get codes */
    nodes = getResults(head, string, nodes, 0);

    /* sort codes in alphabetic order */
    qsort(nodes, 256, sizeof(nodes[0]), compare);
   
    /* check if outfile is there */
    if(argv[2] == NULL)
    {
        /* write to stdout */
        if(write(STDOUT_FILENO, &total, sizeof(total)) == -1)
        {
            perror("write stdout");
            exit(EXIT_FAILURE);
        }

        /* iterate through nodes and print characters and count */
        write(out, &total, sizeof(total));
        for(; i < 256; i++)
        {
            if(nodes[i] != NULL)
            {
                
                c = (uint8_t)nodes[i]->character;
                write(out, &c, sizeof(c));
               
                count = (uint32_t)nodes[i]->count;
                count = htonl(count);
                write(out, &count, sizeof(count));
            }
        }

	
	/* reopen infile */
	inName = argv[1];
        if((in = open(inName, O_RDONLY)) == -1)
        {
            perror(inName);
            exit(EXIT_FAILURE);
        }

        /* encode codes */
       
	/* create hash table */
        createHash(nodes, ht);

	/* iterate through buffer and get codes */
        for(k = 0; k < number; k++)
        {
            index = (int)buf[k];
            newCode = ht[index]->codes;

	/* iterate through codes and set bits of byte */
         while(*newCode)
         {
	    /* set bit to 1 */
            if(*newCode == '1')
            {
                bit |= (1 << (7 - bitSize));
                bitSize++;  
                newCode++;
            }
            else
            {
                newCode++;
                bitSize++;
            }
	    /* check if bitSize equals 8 */
             if(bitSize == 8)
             {
             bitSize = 0;
             buff[byte] = bit;
             bit = 0;
             byte++;
             }
         }
        }
	
	/* pad bits if bitSize is NOT 0 */
        if(bitSize != 0)
        {
            buff[byte] = bit;
            byte++;
        }
  
	/* loop through encodes */
        for(j = 0; j < byte; j++)
        {
            body = buff[j];
            write(out, &body, sizeof(body));
        }
 
    }
    else
    {
       /* write to file */
        outName = argv[2];
        
        if((out = open(outName, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
        {
            perror(outName);
            exit(EXIT_FAILURE);
        }
        
        /* iterate through nodes and print characters and count */
        
        write(out, &total, sizeof(total));
        for(i = 0; i < 256; i++)
        {
            if(nodes[i] != NULL)
            {
               /* write out characters and counts */ 
                c = (uint8_t)nodes[i]->character;
                write(out, &c, sizeof(c));
               
                count = (uint32_t)nodes[i]->count;
                count = htonl(count);
                write(out, &count, sizeof(count));
            }
        }

	/* create hash table */
        createHash(nodes, ht);

	/* open inputfile */
	inName = argv[1];
        if((in = open(inName, O_RDONLY)) == -1)
        {
            perror(inName);
            exit(EXIT_FAILURE);
        }

	/* iterate through buffer to get codes */
        for(k = 0; k < number; k++)
        {
            index = (int)buf[k];
            newCode = ht[index]->codes;

         while(*newCode)
         {
	   /* set 1 bit in byte */
            if(*newCode == '1')
            {
                bit |= (1 << (7 - bitSize));
                bitSize++;  
                newCode++;
            }
            else
            {
                newCode++;
                bitSize++;
            }
	   /* check if bitSize equals 8 */
             if(bitSize == 8)
             {
             bitSize = 0;
             buff[byte] = bit;
             bit = 0;
             byte++;
             }
         }
        }
	/* pad bits if bitSize is NOT 0 */
        if(bitSize != 0)
        {
            buff[byte] = bit;
            byte++;
        }
 
	/* iterate through byte array */  
        for(j = 0; j < byte; j++)
        {
            body = buff[j];
            write(out, &body, sizeof(body));
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

/* hashtable to create index of character */
void createHash(Node** nodes, Node** hashtable)
{
    int hash = 0;
    int i = 0;
    for(; i < 256; i++)
    {
        if(nodes[i] != NULL)
        {
            /* character is hash index */
            hash = nodes[i]->character;
            
            /* node is at hash according to character */
            hashtable[hash] = nodes[i];
        }
    }
}

