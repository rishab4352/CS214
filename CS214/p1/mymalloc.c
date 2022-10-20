#include "mymalloc.h"
#define magicNum 25 


void *mymalloc(int reqBytes, const char *file, int line)
{
    metadata *cursor = (void *)myblock; 
    metadata *currentNode = NULL;       
    void *userMemory = NULL;            
    int *testInit = (int *)myblock;     
    
  
    if (reqBytes <= 0)
    {
        fprintf(stderr, "\nERROR: Please request a valid size of memory.\n"
                "File: %s\n"
                "Line: %d\n",
                file, line);
        return NULL;
    }
    
  
    if (*testInit == magicNum)
    {
        cursor->occu = 0;
        cursor->UsedSpace = 4096 - sizeof(metadata);
        cursor->nextNode = NULL;
    }
    
    currentNode = cursor; 
    
   
    while ((currentNode->occu == 1 || currentNode->UsedSpace < reqBytes) && currentNode->nextNode != NULL)
    {
        currentNode = currentNode->nextNode;
    }
    
   
    if (currentNode->UsedSpace == reqBytes)
    {
        currentNode->occu = 1;               
        userMemory = (void *)(++currentNode); 
        
        return userMemory; 
    }
    
   
    else if (currentNode->UsedSpace > (reqBytes + sizeof(metadata)))
    {
        addNode(currentNode, reqBytes); 
        userMemory = (void *)(++currentNode); 
        
        return userMemory; 
    }
    
  
    else if ((currentNode->UsedSpace > (reqBytes)) && (currentNode->UsedSpace < (reqBytes + sizeof(metadata))))
    {
        currentNode->occu = 1;               
        userMemory = (void *)(++currentNode); 
        
        return userMemory; 
    }
    
 
    else
    {
        userMemory = NULL;
        fprintf(stderr, "\nERROR: Unable to allocate %d byte(s) of memory.\n"
                "File: %s\n"
                "Line: %d\n",
                reqBytes, file, line);
        return userMemory; 
    }
}


void addNode(metadata *oldNode, int reqBytes)
{
    
    metadata *newNode = (void *)((void *)oldNode + reqBytes + sizeof(metadata));
    newNode->occu = 0; 
    newNode->UsedSpace = oldNode->UsedSpace - reqBytes - sizeof(metadata);
    newNode->nextNode = NULL; 
    
    oldNode->occu = 1;                  
    oldNode->UsedSpace = reqBytes; 
    oldNode->nextNode = newNode;         
}


void defragment()
{
    metadata *head = (void *)myblock; 
    metadata *current = head;        
    
  
    while ((current != NULL) && (current->nextNode != NULL))
    {
        
        if ((current->occu == 0) && (current->nextNode->occu == 0))
        {
            current->UsedSpace += current->nextNode->UsedSpace + sizeof(metadata); 
           
            current->nextNode = current->nextNode->nextNode;
            defragment();
        }
        current = current->nextNode; 
    }
}


void *myfree(void *memory, const char *file, int line)
{
    _Bool foundFlag = 0; 
    
   
    if (memory == NULL)
    {
        fprintf(stderr, "\nERROR: Given pointer is NULL.\n"
                "File: %s\n"
                "Line: %d\n",
                file, line);
        return NULL;
    }
    
  
    
    metadata *cursor = (void *)myblock;
    
    while (cursor != NULL)
    {
        if (memory - sizeof(metadata) == cursor)
        {
            foundFlag = 1; 
            break;
        }
        else
            cursor = cursor->nextNode;
    }
    
    if (foundFlag == 0) 
    {
        fprintf(stderr, "\nERROR: Pointer is not associated with malloc().\n"
                "File: %s\n"
                "Line: %d\n",
                file, line);
        return NULL;
    }
    
    metadata *currentNode = memory - (sizeof(metadata));
    
   
    if (currentNode->occu == 0) 
    {
        fprintf(stderr, "\nERROR: Redundant free()ing of the same pointer.\n"
                "File: %s\n"
                "Line: %d\n",
                file, line);
        return NULL;
    }
    
    currentNode->occu = 0;
    
    defragment(); 
    
    return NULL;
}