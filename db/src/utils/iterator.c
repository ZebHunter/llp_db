#include "../../include/utils/iterator.h"
#include "../../include/file_manager/document.h"

iterator_t* createIterator(){
    iterator_t* iterator = malloc(sizeof(iterator_t));
    if(iterator){
        iterator->refs = NULL;
        iterator->size = 0;
        iterator->current = -1;
        return iterator;
    }
    return NULL;
}

void destroyIterator(iterator_t* iterator){
    if(iterator){
        free(iterator->refs);
        free(iterator);
    }
}

bool addPage(iterator_t* iterator, uint64_t ref){
    if(iterator){
        uint64_t* tmp = realloc(iterator->refs, sizeof(uint64_t) * (iterator->size + 1));
        if(tmp){
            iterator->refs = tmp;
            iterator->refs[iterator->size++] = ref;
            return true;
        }
    }
    return false;
}

bool hasNext(iterator_t* iterator){
    return iterator && (iterator->current + 1 < iterator->size);
}

page_t* next(struct file* file, iterator_t* iterator){
    if(!hasNext(iterator)) return NULL;

    return readPage(file, iterator->refs[++iterator->current]);
}

