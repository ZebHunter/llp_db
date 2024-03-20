#ifndef LLP_PAGE_H
#define LLP_PAGE_H
#include "../schema_manager/schema.h"
#include "document.h"
#include <string.h>

typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    uint64_t offset;
} page_id;


typedef struct __attribute__((packed)){
    uint64_t pageSize;
    uint64_t index;
    uint64_t brotherIndex;
    uint64_t childIndex;
    page_id id;
    char schemaName[16];
} page_header;

typedef struct page{
    page_header header;
    schema_t* schema;
} page_t;

page_t* createPage();

uint64_t readElement(struct file* file, element_t* el, bool skipStr);

uint64_t writeElement(struct file* file, element_t* el);

enum write_status updateElement(struct file* file, block_index* index, page_header* header, element_t* oldEl, element_t* newEl);

page_t* readPage(struct file* file, uint64_t index);

uint64_t writePage(struct file* file, schema_t* schema, uint64_t siblingIndex);

void printPage(page_t* page);

char* getPageData(page_t* page);

element_t* getElementFromPage(page_t* page, const char* key);

schema_t* getSchemaFromPage(page_t* page);

void destroyPage(page_t* page);

#endif //LLP_PAGE_H
