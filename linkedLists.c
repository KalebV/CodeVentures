#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int size;
    struct vector* next;
} vector;

vector* fLast(vector* head);
vector* fLast2(vector* head);
vector* newVector(int val);
void push(int val, vector* head, int index, int* totalIndex);
void pop(vector* head, int index, int* totalIndex);
void printList(vector* head);
void destruct(vector* head, int* totalIndex);

int main() {
    vector* head = newVector(0);
    int choice = 0, totalIndex = 0;
    while (choice != -1){
        int num, index;
        scanf("%d", &choice);
        if (choice == 0){
            destruct(head, &totalIndex);
        } else if (choice == 1){
            scanf("%d", &index);
            scanf("%d", &num);
            if(index <= totalIndex){
                push(num, head, index, &totalIndex);
            } else {
                printf("%d is out of bounds\n", index);
            }
        } else if (choice == 2){
            scanf("%d", &index);    
            if(index <= totalIndex){
                pop(head, index, &totalIndex);
            } else {
                printf("%d is out of bounds\n", index);
            }
        } else if (choice == 3){
            if (totalIndex != 0){
                printList(head);
            }else {
                printf("NULL\n");
            }
        } else if(choice == -1){
            break;
        } else {
            printf("Invalid option\n");
        }
    }
    free(head);
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
void push(int val, vector* head, int index, int* totalIndex){
    vector* put = newVector(val);
    if (index == -1){
        vector* last = fLast(head);
        last -> next = (struct vector*)put;
    } else { 
        vector* insert = head;
        for (int i = 0; i <= index; i++){
            insert = (vector*) insert -> next;
        }
        put -> next = (struct vector*)insert;
        insert = head;
        for (int i = 0; i <= index - 1; i++){
            insert = (vector*) insert -> next;
        }
        insert -> next  = (struct vector*)put;
    }
    *totalIndex += 1;
    return;
}
void pop(vector* head, int index, int* totalIndex){
    if (index == -1){
        vector* last = fLast(head);
        vector* secondLast = fLast2(head);
        secondLast -> next = NULL;
        free(last);
    } else {
        vector* traverse1 = head;
        vector* traverse2;
        for (int i = 0; i <= index; i++){
            traverse2 = traverse1;
            traverse1 = (vector*)traverse1 -> next;
        }
        traverse2 -> next = traverse1 -> next;
        free(traverse1);
    }
    *totalIndex -= 1;
    return;
}
void printList(vector* head){
    vector* last = (vector*) head -> next;
    while (last != NULL){
        printf("%d ", last->size);
        last = (vector*)last -> next;
    }
    printf("\n");
    return;
}
void destruct(vector* head, int* totalIndex){
    int index = *totalIndex;
    for(int i = 0; i < *totalIndex; i++){
        pop(head, -1, &index);
    }
    *totalIndex = 0;
    return;
}