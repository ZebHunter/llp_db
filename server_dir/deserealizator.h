
#ifndef LLP_3_DESEREALIZATOR_H
#define LLP_3_DESEREALIZATOR_H

#include "../db/include/query_manager/query.h"
#include "../spec.pb-c.h"

void printStruct(AstNodeT* ast);

element_t* deserializeElementNode(AstNodeT* elementNode);

schema_t* deserializeValuesNode(AstNodeT* valuesNode, const char* schemaName);

condition_t* deserializeOperationNode(AstNodeT* operationNode);

query_t* deserializeQueryNode(AstNodeT* queryNode);

#endif //LLP_3_DESEREALIZATOR_H
