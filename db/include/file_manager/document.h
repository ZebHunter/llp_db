#ifndef LLP_DOCUMENT_H
#define LLP_DOCUMENT_H

#define DEFAULT_VALUE 100000
#define BUFFER_SIZE 500000
#define DOCUMENT_NOT_EXISTS 0xFFFFFFFFFF

#include "../utils/list.h"
#include "op_status.h"

typedef struct document_header {
    uint64_t fileSize;
    int64_t firstPageOffset;
    uint64_t indexCount;
    uint64_t rootIndex;
} document_header;

struct file {
    FILE* f;
    document_header header;
    sortList indexes;
};

enum index_type {
    UNKNOWN_INDEX = 0,
    NEW_INDEX = 1,
    USING_INDEX = 2,
    DEAD_INDEX = 3
};

typedef struct __attribute__((packed)) block_index  {
    uint64_t block_offset;
    uint8_t type;
} block_index;

struct file* createFile(const char* name);

struct file* loadFile(const char* name);

void closeFile(struct file* file);

enum read_status loadIndexList(struct file* file);

enum write_status writeHeader(struct file* file);

block_index getIndex(struct file* file, uint64_t index_number);

enum write_status updateIndex(struct file* file, uint64_t number, uint64_t offset, uint8_t type);

enum write_status writeNewIndexes(struct file* file, uint64_t count, uint64_t last_index);

bool transferData(struct file* file, int64_t* old_position, int64_t* new_position, uint64_t size);

bool moveFirstBlock(struct file* file);

#endif //LLP_DOCUMENT_H
