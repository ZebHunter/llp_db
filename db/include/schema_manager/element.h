

#ifndef LLP_ELEMENT_H
#define LLP_ELEMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef enum fields{
    TYPE_DEFAULT = 0,
    TYPE_FLOAT = 1,
    TYPE_STRING = 2,
    TYPE_BOOLEAN = 3,
    TYPE_INTEGER = 4
} fields_t;
//возвращаемое значение size - int -> int32_t
typedef struct string{
    int32_t size;
    char* data;
} string_t;

typedef struct element{
    uint8_t type;
    char key[16];
    union {
        int32_t intValue;
        double floatValue;
        string_t stringValue;
        bool booleanValue;
    };
} element_t;


element_t* createElement(const char* key, element_t el);

element_t* createIntegerElement(const char* key, int32_t value);

element_t* createDoubleElement(const char* key, double value);

element_t* createBooleanElement(const char* key, bool value);

element_t* createStringElement(const char* key, char* value);

element_t* noValueElement(const char* key);

void printElement(element_t* el);

void destroyElement(element_t* el);

#endif //LLP_ELEMENT_H
