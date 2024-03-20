
#ifndef LLP_LIST_H
#define LLP_LIST_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct listNode{

    uint64_t fileSize;
    uint64_t index;
    struct listNode* prev;
    struct listNode* next;

} listNode;

typedef struct {
    listNode* front;
    listNode* botton;
} sortList;

listNode* createNode(uint64_t fileSize, uint64_t index);

void destroyList(sortList* node);

void insertNode(listNode* node, sortList* list);

listNode* popFront(sortList* list);

listNode* popBotton(sortList* list);

bool removeByIndex(sortList* list, uint64_t index);
#endif //LLP_LIST_H
