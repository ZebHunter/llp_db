
#ifndef LLP_3_SEREALIZATOR_H
#define LLP_3_SEREALIZATOR_H

#include "structures/structures.h"
#include "../spec.pb-c.h"

AstNodeT* serialize(nodeType* parentType, astNode* node);

#endif //LLP_3_SEREALIZATOR_H