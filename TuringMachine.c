/*
Project by Andrew Boyce
CSC 344

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLENGTH 40

struct Node * head;

typedef struct Node
{
    struct Node *next;
    struct Node *prev;
    char data;
} Node;

typedef struct Instruction
{
    char write;
    char direction;
    int newState;
} Instruction;

typedef struct Table
{
    struct Instruction inst;
} Table;

void prepend(char c,int j)
{
    //creates a node to add at the beginning of the list
    struct Node* insert = (struct Node*) malloc(sizeof(Node));
    insert->data = c;
    insert->next = NULL;
    insert->prev = NULL;
    //if the list is empty then create the list and return
    if(head == NULL)
    {
        head = insert;
        return;
    }
    //make the node being inserted the head of the list, and then update the head
    head->prev = insert;
    insert->next = head;
    head = insert;
}

void append(char c,int j)
{
    //creates a node to add at the end of the list
    struct Node* temp = head;
    struct Node* insert = (struct Node*) malloc(sizeof(Node));
    insert->data = c;
    insert->next = NULL;
    insert->prev = NULL;

    //if the list is empty then create the list and return
    if(head == NULL)
    {
        head = insert;
        return;
    }

    //make the node being inserted the last node in the list, and then update the references
    while(temp->next != NULL)
        temp = temp->next;
    temp->next = insert;
    insert->prev = temp;
}

void printList(struct Node * list,int listLength)
{
    struct Node *temp = (struct Node*) malloc(sizeof(Node));
    temp = list;
    //sets temp equal to our list at whatever node that its pointing to and then iterates back to its head
    while(temp->prev != NULL)
        temp = temp->prev;
    printf("Final tape contents: ");
    //prints the list in order
    for(int i =0; i < listLength; i++)
    {
        printf("%c",temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main()
{

    FILE *fp;
    char filename[20];
    char *initialTape = (char *)malloc(sizeof(char)*MAXLENGTH);
    char *tape = (char *)malloc(sizeof(char)*MAXLENGTH);
    char * str = (char *)malloc(sizeof(char)*MAXLENGTH);
    int max = -1;
    int start = -1;
    int end = -1;
    int stateCurr = 0;
    int stateNext = 0;
    int listSize = 0;
    head = NULL;

    //ask for user input
    printf("Input File: ");
    scanf("%s",&filename);
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Can't open %s",filename);
        return 1;
    }

    printf("Writing tape...\n");
    fscanf(fp,"%s",initialTape);

    //creates the list
    for(int i = 0; i < strlen(initialTape); i++)
    {
        append(*(initialTape+i),listSize++);
    }
    //prints the initial tape
    printf("Initial Tape Contents: %s", initialTape);
    //updates our amount of states as well as the start and end state
    fscanf(fp,"%s", str);
    max = str[0] - '0';
    fscanf(fp,"%s", str);
    start = str[0] - '0';
    fscanf(fp,"%s", str);
    end = str[0] - '0';

    //creates our table
    struct Table table[max][255];
    char read = 'a';
    char write = 'a';
    char direction='a';

    //reads the instructions and makes our table
    while(!feof(fp))
    {
        fscanf(fp,"%d %c %c %c %d",&stateCurr,&read,&write,&direction,&stateNext);
        table[stateCurr][read].inst.direction = direction;
        table[stateCurr][read].inst.write = write;
        table[stateCurr][read].inst.newState = stateNext;
    }

    int x = start;
    struct Node * iterator = head;
    stateCurr = start;
    char r = 'a';

    /*
    This is where the Turing Machine does its work.
    1.)The variable r keeps track of what we're reading.
    2.)Then we overwrite the data at that node that we're pointing to.
    3.)Move left or right depending on instruction.
    4.)If the node that we are moving to is NULL, then create one on the fly and update our iterators pointer to it.
    5.)Then we update our current state depending on the end state of the instruction.
    */
    while(stateCurr != end)
    {
        r = iterator->data;
        iterator->data = table[stateCurr][r].inst.write;
        if(table[stateCurr][r].inst.direction == 'R')
        {
            if(iterator->next!=NULL)
            {
                iterator = iterator->next;
            }
            else
            {
                append('B',listSize++);
                iterator = iterator->next;
            }
        }
        else
        {
            if(iterator->prev!=NULL)
            {
                iterator = iterator->prev;
            }
            else
            {
                prepend('B',listSize++);
                iterator = iterator->prev;
            }

        }
        stateCurr = table[stateCurr][r].inst.newState;
    }
    printf("\n");
    //Then print our doubly linked list in order.
    printList(head,listSize);
    return 1;
}
