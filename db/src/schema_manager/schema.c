#include "../../include/schema_manager/schema.h"
#include "../../include/file_manager/page.h"
#include <string.h>

schema_t* createSchema(const char* name) {
    if (name && strlen(name) <= 15) {

        schema_t* schema = malloc(sizeof(schema_t));

        if (schema) {
            schema->elements = NULL;
            schema->size = 0;
            strcpy(schema->name, name);

            return schema;
        }
    }

    return NULL;
}

schema_t* createElements() {
    return createSchema("0");
}

element_t* getElement(schema_t* schema, const char* key) {
    if (schema) {
        for (uint64_t i = 0; i < schema->size; i++) {
            if (!strcmp(key, schema->elements[i]->key)) {
                return schema->elements[i];
            }
        }
    }
    return NULL;
}

bool addElement(schema_t* schema, element_t* el) {
    element_t** e = realloc(schema->elements, sizeof(element_t*) * (schema->size + 1));
    if (e) {
        schema->elements = e;
        schema->elements[schema->size++] = el;
        return true;
    }

    return false;
}

uint64_t calcSizeOfPage(schema_t* schema) {
    uint64_t sz = sizeof(page_header);
    for (uint64_t i = 0; i < schema->size; i++) {
        sz += sizeof(uint8_t) + 16 * sizeof(char);
        element_t* element = schema->elements[i];
        switch (element->type) {
            case TYPE_INTEGER:
                sz += sizeof(int32_t);
                break;
            case TYPE_FLOAT:
                sz += sizeof(double);
                break;
            case TYPE_BOOLEAN:
                sz += sizeof(bool);
                break;
            case TYPE_STRING:
                sz += sizeof(uint32_t) + sizeof(char) * element->stringValue.size;
                break;
        }
    }
    return sz;
}

void destroySchema(schema_t* schema) {
    if (schema) {
        if (schema->elements) {
            for (uint64_t i = 0; i < schema->size; i++) {
                destroyElement(schema->elements[i]);
            }
            free(schema->elements);
        }
        free(schema);
    }
}
