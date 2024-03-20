
#ifndef LLP_QUERY_H
#define LLP_QUERY_H

#include "../schema_manager/schema.h"
#include "condition.h"
#include "../utils/iterator.h"

typedef enum{
    DEFAULT_QUERY = 0,
    SELECT_QUERY = 1,
    INSERT_QUERY = 2,
    UPDATE_QUERY = 3,
    REMOVE_QUERY = 4
} type_t;

typedef struct query{
    type_t queryType;
    const char* schemaName;
    schema_t* newValue;
    condition_t* condition;
    struct query** innerQuery;
    uint64_t size;
} query_t;

query_t* createQuery(type_t type, const char* schemaName, schema_t* newValue, condition_t* condition);

query_t* createSelectQuery(const char* schemaName, condition_t* condition);

query_t* createInsertQuery(const char* schemaName, schema_t* newValues, condition_t* condition);

query_t* createUpdateQuery(const char* schemaName, schema_t* newValues, condition_t* condition);

query_t* createDeleteQuery(const char* schemaName, condition_t* condition);

enum crud_status executeSelectQuery(struct file* file, iterator_t** iterator, query_t* query);

enum crud_status executeInsertQuery(struct file* file, query_t* query);

enum crud_status executeUpdateQuery(struct file* file, query_t* query);

enum crud_status executeDeleteQuery(struct file* file, query_t* query);

bool addInnerQuery(query_t* query, query_t* inner);

bool checkPage(struct file* file, uint64_t index, query_t* query);

enum crud_status insertPage(struct file* file, query_t* q, uint64_t* parentIndex);

enum crud_status updatePage(struct file* file, query_t* q, uint64_t* index);

enum crud_status removePage(struct file* file, query_t* q, uint64_t* index);

bool find(struct file* file, iterator_t* iterator, query_t* query, uint64_t* index, enum crud_status(*change)(struct file*, query_t*, uint64_t*));

void destroyQuery(query_t* query);

#endif //LLP_QUERY_H
