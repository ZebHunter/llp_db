#include "../../include/query_manager/condition.h"
#include "../../include/query_manager/query.h"

condition_t* createCondition(operation_t op, void* element1, void* element2){
    condition_t* cond = malloc(sizeof(condition_t));
    if(cond){
        cond->operation = op;
        cond->cond1 = element1;
        cond->cond2 = NULL;
        return cond;
    }
    return NULL;
}

condition_t* conditionEquals(element_t* element1, element_t* element2){
    return createCondition(EQUALS, element1, element2);
}

condition_t* conditionNeq(element_t* element1, element_t* element2){
    return createCondition(NEQ, element1, element2);
}

condition_t* conditionGt(element_t* element1, element_t* element2){
    return createCondition(GT, element1, element2);
}

condition_t* conditionGte(element_t* element1, element_t* element2){
    return createCondition(GTE, element1, element2);
}

condition_t* conditionLe(element_t* element1, element_t* element2){
    return createCondition(LE, element1, element2);
}

condition_t* conditionLee(element_t* element1, element_t* element2){
    return createCondition(LEE, element1, element2);
}

condition_t* conditionAnd(condition_t* cond1, condition_t* cond2){
    return createCondition(AND, cond1, cond2);
}

condition_t* conditionOr(condition_t* cond1, condition_t* cond2){
    return createCondition(OR, cond1, cond2);
}

condition_t* conditionLike(element_t* element1, element_t* element2){
    if (element1 && ((element1->type != TYPE_DEFAULT && !element2) ||
                (element1->type == TYPE_DEFAULT && element2 && element2->type == TYPE_DEFAULT))) {
        return createCondition(LIKE, element1, element2);
    }
    return NULL;
}

condition_t* conditionNot(condition_t* cond){
    return createCondition(NOT, cond, NULL);
}

void destroyCondition(condition_t* cond){
    if(cond){
        if(cond->cond1 || cond->cond2){
            destroyCondition(cond->cond1);
            destroyCondition(cond->cond2);
        }
        else{
            destroyElement(cond->element);
        }
        free(cond);
    }
}

int32_t compareElements(element_t* element1, element_t* element2){
    switch(element1->type){
        case TYPE_INTEGER:
            return (element1->intValue > element2->intValue) - (element1->intValue < element2->intValue);
        case TYPE_FLOAT:
            return (element1->floatValue > element2->floatValue) - (element1->floatValue < element2->floatValue);
        case TYPE_BOOLEAN:
            return element1->booleanValue - element2->booleanValue;
        case TYPE_STRING:
            return strcmp(element1->stringValue.data, element2->stringValue.data);
    }
    return 0;
}

bool checkCondition(condition_t* cond, element_t* element){
    if(cond->operation < AND && (cond->element->type != element->type || strcmp(element->key, cond->element->key) != 0)) return false;

    switch(cond->operation){
        case EQUALS:
            return compareElements(cond->element, element) == 0;
        case NEQ:
            return compareElements(cond->element, element) != 0;
        case GT:
            return compareElements(cond->element, element) > 0;
        case GTE:
            return compareElements(cond->element, element) >= 0;
        case LE:
            return compareElements(cond->element, element) < 0;
        case LEE:
            return compareElements(cond->element, element) <= 0;
        case LIKE:
            return cond->element ? strstr(cond->element->stringValue.data, element->stringValue.data) :
                     strstr(element->stringValue.data, element->stringValue.data);
        case AND:
            return checkCondition(cond->cond1, element) && checkCondition(cond->cond2, element);
        case OR:
            return checkCondition(cond->cond1, element) || checkCondition(cond->cond2, element);
        case NOT:
            return !checkCondition(cond->cond1, element);
    }
    return false;
}



