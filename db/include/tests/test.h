#ifndef LLP_TEST_H
#define LLP_TEST_H

#include "../file_manager/document.h"
#include "../query_manager/query.h"

uint32_t true_rand(uint32_t start, uint32_t end);

void testInsert(const char* fileName, query_t* insertRoot);

void testDelete(const char* fileName, const char* schemaName);

void testAddDelete(const char* fileName, query_t* insertRoot);

#endif //LLP_TEST_H
