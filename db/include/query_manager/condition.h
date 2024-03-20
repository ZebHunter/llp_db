
#ifndef LLP_CONDITION_H
#define LLP_CONDITION_H

#include "../schema_manager/element.h"
#include <string.h>

typedef enum{
    EQUALS = 1, // ==
    NEQ = 2,    // !=
    GT = 3,     // >
    GTE = 4,    // >=
    LE = 5,     // <
    LEE = 6,
    LIKE = 7,    // <=
    AND = 8,    // &
    OR = 9,     // |
    NOT = 10     // !
} operation_t;

typedef struct condition{
    operation_t operation;
    union{
        element_t* element;
        struct condition* cond1;
    };
    struct condition* cond2;

} condition_t;

condition_t* createCondition(operation_t op, void* element1, void* element2);

int32_t compareElements(element_t* element1, element_t* element2);

bool checkCondition(condition_t* cond, element_t* element);

condition_t* conditionEquals(element_t* element1, element_t* element2);

condition_t* conditionNeq(element_t* element1, element_t* element2);

condition_t* conditionGt(element_t* element1, element_t* element2);

condition_t* conditionGte(element_t* element1, element_t* element2);

condition_t* conditionLe(element_t* element1, element_t* element2);

condition_t* conditionLee(element_t* element1, element_t* element2);

condition_t* conditionAnd(condition_t* cond1, condition_t* cond2);

condition_t* conditionOr(condition_t* cond1, condition_t* cond2);

condition_t* conditionNot(condition_t* cond);

void destroyCondition(condition_t* cond);

#endif //LLP_CONDITION_H
