/*
    create a simple vector-like data structure and related routines that use realloc() to manage the vector. 
    Use an array to store the vectors elements; 
    when a user adds an entry to the vector, use realloc() to allocate more space for it. 
    How well does such a vector perform? How does it compare to a linked list? 



    Vector is great for fast random access and works well when you mostly add elements at the end.
    Linked List is better if you do a lot of inserts/deletes in the middle.
    Vector is more cache friendly.


*/


#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;     
    int size;      
    int capacity;   
} Vector;


void init(Vector *v) {
    v->size = 0;
    v->capacity = 2;  
    v->data = malloc(v->capacity * sizeof(int));
}


void push(Vector *v, int value) {
    if (v->size == v->capacity) {
       
        v->capacity *= 2; // full -> double
        v->data = realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size] = value;
    v->size++;
}


void print(Vector *v) {
    for (int i = 0; i < v->size; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");
}


void freeVector(Vector *v) {
    free(v->data);
}

int main() {
    Vector v;
    init(&v);

    for (int i = 0; i < 10; i++) {
        push(&v, i);
    }

    print(&v);

    freeVector(&v);
    return 0;
}