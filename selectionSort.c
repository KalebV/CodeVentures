#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b);

int main () {
    int rando, numberOfElements;
    scanf("%d %d", &rando, &numberOfElements);
    srand(rando);
    int* array = malloc(sizeof(int)* numberOfElements);
    // Initialize Array
    for(int i = 0; i < numberOfElements; i++){
        array[i] = rand() % 50;
    }
    for(int i = 0; i < numberOfElements - 1; i++){
        int min_index = i;
        for(int j = i + 1; j < numberOfElements; j++){
            if(array[min_index] > array[j]){
                min_index = j;
            }
        } if (min_index != i){
            swap(&array[min_index],&array[i]);
        }
    }
    for (int i = 0; i < numberOfElements; i++){
        printf("%d ", array[i]);
    }
    free(array);
    return 0;
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}