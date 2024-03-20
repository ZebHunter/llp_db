#include <stdlib.h>
#include "serealizator.h"

AstNodeT* serialize(nodeType* type, astNode* node){
    if(node){
        nodeType* nextParentType = type;
        FieldsT* field = malloc(sizeof(FieldsT));
        fields_t__init(field);
        switch(node->type){
            case SELECT_QUERY_NODE:
            case INSERT_QUERY_NODE:
            case UPDATE_QUERY_NODE:
            case DELETE_QUERY_NODE:
                nextParentType = &node->type;
                break;
            case NESTED_QUERY_NODE:
                node->type = *type;
                break;
            case INT_VAL_NODE:
                field->integervalue = node->intVal;
                break;
            case DOUBLE_VAL_NODE:
                field->doublevalue = node->doubleVal;
                break;
            case BOOL_VAL_NODE:
                field->booleanvalue = node->boolVal;
                break;
            case KEY_NODE:
            case OBJECT_NODE:
            case STR_VAL_NODE:
                field->stringvalue = node->strVal;
                break;
        }

        AstNodeT* ast = malloc(sizeof(AstNodeT));
        ast_node_t__init(ast);
        if (node->left) ast->left = serialize(nextParentType, node->left);
        if (node->right) ast->right = serialize(nextParentType, node->right);
        ast->type = (NodeTypeT) node->type;
        ast->field = field;
        return ast;
    }
    return NULL;
}

