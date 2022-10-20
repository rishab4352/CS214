#ifndef _MYMALLOC_h
#define _MYMALLOC_h

#include <stdio.h>

#define malloc(s) mymalloc(s, __FILE__, __LINE__) 
#define free(p) myfree(p, __FILE__, __LINE__)     

typedef struct node
{                          
    struct node *nextNode; 
    short UsedSpace;       
    _Bool occu;           
} metadata;

void *mymalloc(int, const char *file, int line);  
void addNode(metadata *, int);                    
void defragment(void);                            
void *myfree(void *, const char *file, int line); 

static char myblock[4096] = {25}; 

#endif 