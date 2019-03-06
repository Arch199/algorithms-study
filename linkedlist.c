/* Linked List Implementation
 * Originally written for finding primes in primelink.c (Joel's idea).
 * Some code copied from textbook.
 */

#include <stdlib.h>
#include <assert.h>

// Note: Need to typedef data_t in another file when using.

typedef struct node_t {
    data_t data;
    struct node_t* next;
} node_t;

typedef struct linked_t {
    node_t* head;
    node_t* foot;
} linked_t;

linked_t* new_linked();
void append_linked(linked_t* list, int new_data);
void prepend_linked(linked_t* list, int new_data);
void free_linked(linked_t* list);

linked_t* new_linked() {
    linked_t* new = malloc(sizeof(linked_t));
    assert(new);
    new->head = new->foot = NULL;
    return new;
}

void append_linked(linked_t* list, int new_data) {
    node_t* new = malloc(sizeof(node_t));
    assert(list && new);
    new->data = new_data;
    new->next = NULL;
    if (!(list->foot)) list->head = list->foot = new;
    else {
        list->foot->next = new;
        list->foot = new;
    }
}

// Extra function I made during the lecture 17/9/18, W8 Monday
void prepend_linked(linked_t* list, int new_data) {
    node_t* new = malloc(sizeof(node_t));
    assert(list && new);
    new->data = new_data;
    new->next = list->head;
    list->head = new;
    if (!(list->foot)) list->foot = list->head;
}

void free_linked(linked_t* list) {
    assert(list);
    node_t *ptr = list->head, *next;
    while (ptr) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    free(list);
}

data_t pop_linked(linked_t* list, node_t* node) {
    assert(list && list->head && node);

    // Find the previous node
    for (node_t* p = list->head; p->next && p->next != node; p = p->next);
    assert(p->next);
    p->next = node->next;
    data_t data = node->data;
    free(node);
    return data;
}
