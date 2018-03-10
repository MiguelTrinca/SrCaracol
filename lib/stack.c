/* 
  This library implements an ordinary stack with linked lists with limited functionality and linear time for checking if an
  element is in the stack. It dows so by maintaining an additional array of whether or not the element is present.
*/

#include <stdio.h>
#include <stdlib.h>

struct node{
    int value;
    struct node *next;
};

static struct node *top;
int *stack_array;

void init(int V){  //Inicializa a pilha V-> numero de vertices
    top = NULL;
    stack_array = (int *)malloc(sizeof(int)*V);
}

int is_empty(){
    return (top == NULL);
}

void push(int value){
    struct node *new;
    new = (struct node *) malloc(sizeof(struct node));
    new->value = value;
    new->next = top;
    top = new;

    stack_array[value] = value; // Ir a posicao do array e meter la cenas
}

int pop(){
    int value;
    struct node *old;
    
    if(!is_empty()){
        value = top->value;
        old = top;
        top = top->next;
        free(old);

        stack_array[value] = 0;
        return value;
    }
    else
        return -1;
}

int belong(int value){
    return stack_array[value] == value;
}

int main(){
    return 0;
}
