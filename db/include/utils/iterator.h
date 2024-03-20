#ifndef LLP_ITERATOR_H
#define LLP_ITERATOR_H


#include "../file_manager/page.h"

typedef struct iterator{
    uint64_t* refs;
    uint64_t size;
    int64_t current;
} iterator_t;

iterator_t* createIterator();

bool addPage(iterator_t* iterator, uint64_t ref);

void destroyIterator(iterator_t* iterator);

bool hasNext(iterator_t* iterator);

page_t* next(struct file* file, iterator_t* iterator);

#endif //LLP_ITERATOR_H
