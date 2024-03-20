#include "../../include/schema_manager/element.h"


element_t* createElement(const char* key, element_t el){
    if (key && strlen(key) <= 15) {
        element_t* elem = malloc(sizeof(element_t));
        *elem = el;
        memset(elem->key, 0, 16);
        strncpy(elem->key, key, 16);
        return elem;
    }
    return NULL;
}

element_t* createIntegerElement(const char* key, int32_t value) {
    return createElement(key, (element_t) { .type = TYPE_INTEGER, .intValue = value });
}

element_t* createDoubleElement(const char* key, double value) {
    return createElement(key, (element_t) { .type = TYPE_FLOAT, .floatValue = value });
}

element_t* createBooleanElement(const char* key, bool value) {
    return createElement(key, (element_t) { .type = TYPE_BOOLEAN, .booleanValue = value });
}

element_t* createStringElement(const char* key, char* value) {
    return createElement(key, (element_t) { .type = TYPE_STRING, .stringValue = (string_t){ strlen(value) + 1, value } });
}

element_t* noValueElement(const char* key) {
    if (key && strlen(key) <= 12) {
        element_t* ptr = malloc(sizeof(element_t));
        if (ptr) {
            ptr->type = TYPE_DEFAULT;
            memset(ptr->key, 0, 13);
            strncpy(ptr->key, key, 13);
            return ptr;
        }
    }
    return NULL;
}

void printElement(element_t* el){
    if(el){
        printf("%s = ", el->key);
        switch (el->type) {
            case TYPE_INTEGER:
                printf("%d\n", el->intValue);
                break;
            case TYPE_FLOAT:
                printf("%f\n", el->floatValue);
                break;
            case TYPE_BOOLEAN:
                printf("%s\n", el->booleanValue ? "true" : "false");
                break;
            case TYPE_STRING:
                printf("\"%s\"\n", el->stringValue.data);
                break;
        }
    } else {
        printf("Element doesn't exist!\n");
    }
}

void destroyElement(element_t* el) {
    if (el) {
        if (el->type == TYPE_STRING && el->stringValue.data) {
            free(el->stringValue.data);
        }
        free(el);
    }
}

