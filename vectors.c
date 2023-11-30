#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int size;
    struct vector* next;
} vector;

vector* fLast(vector* head);
vector* fLast2(vector* head);
vector* newVector(int val);
void push(int val, vector* head);
void pop(vector* head);
void printList(vector* head);

int main () {
    vector* head = newVector(0);
    int choice = 0;
    while (choice != -1){
        scanf("%d", &choice);
        if (choice == 1){
            int num;
            scanf("%d", &num);
            push(num, head);
        }else if (choice == 0)
        {
            pop(head);
        } 
    }
    printList(head);
    return 0;
}

vector* fLast(vector* head){
    vector* last = (vector*) head;
    while (last -> next != NULL){
        last = (vector*) last -> next;
    }
    return last;
}
vector* fLast2(vector* head){
    vector* last = (vector*) head;
    vector* secondLast= NULL;
    while (last -> next != NULL){
        secondLast = last;
        last = (vector*) last -> next;
    }
    return secondLast;
}
vector* newVector(int val){
    vector* s = (vector*)malloc(sizeof(vector*));
    s->size = val;
    s->next = NULL;
    return s;
}
void push(int val, vector* head){
    vector* put = newVector(val);
    vector* last = fLast(head);
    last -> next = (struct vector*)put;
    return;
}
void pop(vector* head){;
    vector* last = fLast(head);
    vector* secondLast = fLast2(head);
    secondLast -> next = NULL;
    free(last);
    return;
}
void printList(vector* head){
    vector* last = (vector*) head -> next;
    while (last != NULL){
        printf("%d ", last->size);
        last = (vector*)last -> next;
    }
    return;
}