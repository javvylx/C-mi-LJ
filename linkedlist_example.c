#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX_CHAR 32
#define MAX_INPUT 256


struct linked_list
{
    char input[MAX_CHAR];
    struct linked_list *next;
};

typedef struct linked_list list1;
list1 *head = NULL, *last = NULL;

void insertinput(char *input);
void printll();

int main(void)
{
    char input[MAX_CHAR];
    char* special = "***";
	int temp;
	int	bool;

    while (bool != 0)
    {
        //printf("\nEnter a word (less than 32 characters long):");
        fgets(input, MAX_CHAR, stdin);
        input[strcspn(input, "\n")] = 0;
        int results = strcmp(input,special);
        
        if(results == 0){
          //call the function to print the linked list
          printll();
          //freeing the memory
          list1 *pp = head;
	        list1 *cc = head;
	        while (cc) {
		        pp = cc;
		        cc = pp->next;
		        free(pp);
	        }
            break;
        }
        insertinput(input);
    }

    return 0;
}

void insertinput(char *input)
{
    if (head == NULL)
    { 
        list1 *temp;
        temp = (list1 *)malloc(sizeof(list1));
        strcpy(temp->input, input);
        temp->next = NULL;

        head = temp;
        last = temp;
    }
    else
    {
        list1 *curr = (list1 *)malloc(sizeof(list1));
        strcpy(curr->input, input);

        list1 *checker=head;
        list1 *checker1 = checker->next;

        while (checker != NULL)
        {
            if ((strcmp(curr->input, checker->input) > 0) && (checker1==NULL)){
                 checker->next = curr;
                 curr->next = NULL;
                 break;
            }
            if ((strcmp(curr->input, checker->input) == 0) && (checker1==NULL)){
                 checker->next = curr;
                 curr->next = NULL;
                 break;
            }
            else if ((strcmp(curr->input, checker->input) > 0) && (strcmp(curr->input, checker1->input) < 0) ){
                curr->next = checker1;
                checker->next = curr;
                break;
            }
            else if ((strcmp(curr->input, checker->input) < 0)){
                curr->next = checker;
                head = curr;
                break;
            }
            
            checker = checker->next;
            checker1 = checker1->next;            
        }
        
    }
}

void printll()
{
    printf("All the entered words in order:");
    list1 *printlist;
    printlist = head;

    while (printlist != NULL)
    {
        printf("%s\n", printlist->input);
        printlist = printlist->next;
    }
}
