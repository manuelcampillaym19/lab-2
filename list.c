#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node Node;

struct Node {
    void * data;
    Node * next;
    Node * prev;
};

struct List {
    Node * head;
    Node * tail;
    Node * current;
};

Node * createNode(void * data) {
    Node * newNode = (Node *)malloc(sizeof(Node));
    assert(newNode != NULL);
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

List * createList() {
    List * list = (List *) malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    return list;
}

void* firstList(List* list) {
    if (!list || !list->head) return NULL;
    list->current = list->head;
    return list->current->data;
}

void* nextList(List* list) {
    if (!list || !list->current || !list->current->next) return NULL;
    list->current = list->current->next;
    return list->current->data;
}

void* lastList(List* list) {
    if (!list || !list->tail) return NULL;
    list->current = list->tail;
    return list->current->data;
}

void* prevList(List* list) {
    if (!list || !list->current || !list->current->prev) return NULL;
    list->current = list->current->prev;
    return list->current->data;
}

void pushFront(List* list, void* data) {
    if (!list) return;
    Node* n = createNode(data);
    if (!list->head) {
        list->head = list->tail = n;
    } else {
        n->next = list->head;
        list->head->prev = n;
        list->head = n;
    }
}

void pushBack(List* list, void* data) {
    if (!list) return;
    if (!list->tail) {
        pushFront(list, data);
    } else {
        list->current = list->tail;
        pushCurrent(list, data);
    }
}

void pushCurrent(List* list, void* data) {
    if (!list || !list->current) return;
    Node* n = createNode(data);
    n->prev = list->current;
    n->next = list->current->next;
    if (list->current->next) {
        list->current->next->prev = n;
    } else {
        list->tail = n;
    }
    list->current->next = n;
}

void* popCurrent(List* list) {
    if (!list || !list->current) return NULL;
    Node* node = list->current;
    void* data = node->data;

    if (node->prev) node->prev->next = node->next;
    else list->head = node->next;

    if (node->next) {
        node->next->prev = node->prev;
        list->current = node->next;
    } else {
        list->tail = node->prev;
        list->current = NULL;
    }

    free(node);
    return data;
}

void* popFront(List* list) {
    if (!list) return NULL;
    list->current = list->head;
    return popCurrent(list);
}

void* popBack(List* list) {
    if (!list) return NULL;
    list->current = list->tail;
    return popCurrent(list);
}

void cleanList(List* list) {
    if (!list) return;
    list->current = list->head;
    while(list->head) {
        popFront(list);
    }
}
