#include "deserealizator.h"

void printStruct(AstNodeT* ast){
    printf("%s\n",(char*)ast->type);
    printf("%s\n",(char*)ast->field);
    if(ast->left) printStruct(ast->left);
    if(ast->right) printStruct(ast->right);
}

element_t* deserializeElementNode(AstNodeT* elementNode){
    if(elementNode){
        AstNodeT* node1 = elementNode->left;
        AstNodeT* node2 = elementNode->right;
        switch (node2->type) {
            case NODE_TYPE_T__INT_VAL_NODE:
                return createIntegerElement(node1->field->stringvalue, node2->field->integervalue);
            case NODE_TYPE_T__DOUBLE_VAL_NODE:
                return createDoubleElement(node1->field->stringvalue, node2->field->doublevalue);
            case NODE_TYPE_T__BOOL_VAL_NODE:
                return createBooleanElement(node1->field->stringvalue, node2->field->booleanvalue);
            case NODE_TYPE_T__STR_VAL_NODE:
                return createStringElement(node1->field->stringvalue, node2->field->stringvalue);
        }
    }
    return NULL;
}

schema_t* deserializeValuesNode(AstNodeT* valuesNode, const char* schemaName){
    if (valuesNode) {
        schema_t* newElements = schemaName ? createSchema(schemaName) : createElements();
        AstNodeT* elementSetNode = valuesNode->left;
        while (elementSetNode) {
            AstNodeT * elementNode = elementSetNode->left;
            addElement(newElements, deserializeElementNode(elementNode));
            elementSetNode = elementSetNode->right ? elementSetNode->right : NULL;
        }
        return newElements;
    }
    return NULL;
}

condition_t* deserializeOperationNode(AstNodeT* operationNode){
    if (operationNode) {
        AstNodeT* left = operationNode->left;
        AstNodeT* right = operationNode->right;
        switch (operationNode->type) {
            case NODE_TYPE_T__OP_EQ_NODE:
                if (right->type == NODE_TYPE_T__KEY_NODE) {
                    return conditionEquals(noValueElement(left->field->stringvalue), noValueElement(right->field->stringvalue));
                } else {
                    return conditionEquals(deserializeElementNode(operationNode), NULL);
                }
            case NODE_TYPE_T__OP_NEQ_NODE:
                if (right->type == NODE_TYPE_T__KEY_NODE) {
                    return conditionNeq(noValueElement(left->field->stringvalue), noValueElement(right->field->stringvalue));
                } else {
                    return conditionNeq(deserializeElementNode(operationNode), NULL);
                }
            case NODE_TYPE_T__OP_GT_NODE:
                if (right->type == NODE_TYPE_T__KEY_NODE) {
                    return conditionGt(noValueElement(left->field->stringvalue), noValueElement(right->field->stringvalue));
                } else {
                    return conditionGt(deserializeElementNode(operationNode), NULL);
                }
            case NODE_TYPE_T__OP_GTE_NODE:
                if (right->type == NODE_TYPE_T__KEY_NODE) {
                    return conditionGte(noValueElement(left->field->stringvalue), noValueElement(right->field->stringvalue));
                } else {
                    return conditionGte(deserializeElementNode(operationNode), NULL);
                }
            case NODE_TYPE_T__OP_LE_NODE:
                if (right->type == NODE_TYPE_T__KEY_NODE) {
                    return conditionLe(noValueElement(left->field->stringvalue), noValueElement(right->field->stringvalue));
                } else {
                    return conditionLe(deserializeElementNode(operationNode), NULL);
                }
            case NODE_TYPE_T__OP_LEE_NODE:
                if (right->type == NODE_TYPE_T__KEY_NODE) {
                    return conditionLee(noValueElement(left->field->stringvalue), noValueElement(right->field->stringvalue));
                } else {
                    return conditionLee(deserializeElementNode(operationNode), NULL);
                }
            case NODE_TYPE_T__OP_AND_NODE:
                return conditionAnd(deserializeOperationNode(left), deserializeOperationNode(right));
            case NODE_TYPE_T__OP_OR_NODE:
                return conditionOr(deserializeOperationNode(left), deserializeOperationNode(right));
            case NODE_TYPE_T__OP_NOT_NODE:
                return conditionNot(deserializeOperationNode(left));
        }
    }
    return NULL;
}

query_t* deserializeQueryNode(AstNodeT* queryNode){
    if (queryNode) {
        AstNodeT* objectNode = queryNode->left;
        AstNodeT* valuesNode = objectNode->left ? objectNode->left : NULL;
        AstNodeT* filterNode = objectNode->right ? objectNode->right : NULL;
        AstNodeT* operationNode = filterNode && filterNode->left ? filterNode->left : NULL;

        // Создаём запрос:
        query_t* q = NULL;
        switch (queryNode->type) {
            case NODE_TYPE_T__SELECT_QUERY_NODE:
                q = createSelectQuery(objectNode->field->stringvalue, deserializeOperationNode(operationNode));
                break;
            case NODE_TYPE_T__INSERT_QUERY_NODE: {
                schema_t* schema = deserializeValuesNode(valuesNode, objectNode->field->stringvalue);
                q = createInsertQuery(!schema ? objectNode->field->stringvalue : NULL, schema,
                                      deserializeOperationNode(operationNode));
                break;
            }
            case NODE_TYPE_T__UPDATE_QUERY_NODE:
                q = createUpdateQuery(objectNode->field->stringvalue, deserializeValuesNode(valuesNode, NULL),
                                      deserializeOperationNode(operationNode));
                break;
            case NODE_TYPE_T__DELETE_QUERY_NODE:
                q = createDeleteQuery(objectNode->field->stringvalue, deserializeOperationNode(operationNode));
                break;
        }

        if (q) {
            AstNodeT* querySetNode = queryNode->right ? queryNode->right : NULL;
            while (querySetNode) {
                queryNode = querySetNode->left;
                querySetNode = querySetNode->right ? querySetNode->right : NULL;
                addInnerQuery(q, deserializeQueryNode(queryNode));
            }
            return q;
        }
    }
    return NULL;
}