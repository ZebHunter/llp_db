#include "../../include/utils/list.h"

listNode* createNode(uint64_t fileSize, uint64_t index)
{
    listNode* node = malloc(sizeof(listNode));
    if (node) {
        node->fileSize = fileSize;
        node->index = index;
        node->prev = NULL;
        node->next = NULL;
    }
    return node;

}

void destroyList(sortList* list)
{
    listNode* node = list->front;
    while (node) {
        listNode* next = node->next;
        free(node);
        node = next;
    }
}

void insertNode(listNode* node, sortList* list) {
    if (list->front) {
        listNode* cur = list->front;

        while (cur) {

            if ((cur->fileSize < node->fileSize) || (node->fileSize == 0 && cur->fileSize == 0)) {
                if (cur == list->front) {
                    list->front = node;
                }
                else {
                    cur->prev->next = node;
                }
                node->prev = cur->prev;
                node->next = cur;
                cur->prev = node;

                break;
            }
            else if (cur == list->botton) {
                list->botton = node;
                node->next = cur->next;
                node->prev = cur;
                cur->next = node;
                break;
            }
            cur = cur->next;
        }
    }
    else {
        list->front = node;
        list->botton = node;
    }

}

listNode* popFront(sortList* list) {
    if (list) {
        if (list->front) {
            listNode* front = list->front;
            list->front = list->front->next;
            if (list->front) {
                list->front->prev = NULL;
            }
            else {
                list->botton = NULL;
            }
            return front;
        }
    }
    return NULL;
}

listNode* popBotton(sortList* list) {
    if (list) {
        if (list->botton) {
            listNode* back = list->botton;
            list->botton = list->botton->prev;
            if (list->botton) {
                list->botton->next = NULL;
            }
            else {
                list->front = NULL;
            }
            return back;
        }
    }
    return NULL;
}

bool removeByIndex(sortList* list, uint64_t index) {
    listNode* curr = list->front;
    while (curr) {
        listNode* prev = curr->prev;
        listNode* next = curr->next;
        if (curr->index == index) {
            if (prev) {
                prev->next = next;
            }
            if (next) {
                next->prev = prev;
            }
            if (curr == list->front) {
                list->front = next;
            }
            if (curr == list->botton) {
                list->botton = prev;
            }
            free(curr);
            return true;
        }
        curr = next;
    }
    return false;
}
