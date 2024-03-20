#ifndef LLP_SCHEMA_H
#define LLP_SCHEMA_H

#include "element.h"

typedef struct schema{
    element_t** elements;
    uint64_t size;
    char name[16];
} schema_t;

element_t* getElement(schema_t* schema, const char* key);

uint64_t calcSizeOfPage(schema_t* schema);

schema_t* createSchema(const char* name);

schema_t* createElements();

        bool addElement(schema_t* schema, element_t* el);

void destroySchema(schema_t* schema);

#endif //LLP_SCHEMA_H
