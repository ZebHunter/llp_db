#include <time.h>
#include "../../include/file_manager/page.h"



page_t* createPage()
{
    schema_t* schema = malloc(sizeof(schema_t));
    if (schema) {
        *schema = (schema_t) {0};
        page_t* page = malloc(sizeof(page_t));
        if (page) {
            page->schema = schema;
            return page;
        }
        free(schema);
    }
    return NULL;
}

uint64_t readElement(struct file* file, element_t* el, bool skipStr) {
    if (!fread(&el->type, sizeof(uint8_t), 1, file->f)) return 0;
    if (el->type == TYPE_DEFAULT) return sizeof(uint8_t);
    else if (fread(&el->key, sizeof(char), 16, file->f) == 16) {
        uint64_t rbytes = sizeof(uint8_t) + 16 * sizeof(char);
        switch (el->type) {
            case TYPE_DEFAULT:
                return rbytes;
            case TYPE_INTEGER:
                return fread(&el->intValue, sizeof(int32_t), 1, file->f) ? rbytes + sizeof(int32_t) : 0;
            case TYPE_FLOAT:
                return fread(&el->floatValue, sizeof(double), 1, file->f) ? rbytes + sizeof(double) : 0;
            case TYPE_BOOLEAN:
                return fread(&el->booleanValue, sizeof(bool), 1, file->f) ? rbytes + sizeof(bool) : 0;
            case TYPE_STRING:
                if (fread(&el->stringValue.size, sizeof(uint32_t), 1, file->f)) {
                    if (skipStr) {
                        fseek(file->f, el->stringValue.size, SEEK_CUR);
                        return rbytes + sizeof(uint32_t) + el->stringValue.size;
                    }
                    el->stringValue.data = malloc(sizeof(char) * el->stringValue.size);
                    if (el->stringValue.data) {
                        if (fread(el->stringValue.data, sizeof(char), el->stringValue.size, file->f) == el->stringValue.size) {
                            return rbytes + sizeof(uint32_t) + el->stringValue.size;
                        }
                        free(el->stringValue.data);
                    }
                }
                return 0;
        }
    }
}

uint64_t writeElement(struct file* file, element_t* el) {
    uint64_t bytesWritten = 0;
    bytesWritten += fwrite(&el->type, sizeof(uint8_t), 1, file->f);
    bytesWritten += fwrite(el->key, sizeof(char), 16, file->f) * sizeof(char);
    switch (el->type) {
        case TYPE_INTEGER:
            bytesWritten += fwrite(&el->intValue, sizeof(int32_t), 1, file->f) * sizeof(int32_t);
            break;
        case TYPE_FLOAT:
            bytesWritten += fwrite(&el->floatValue, sizeof(double), 1, file->f) * sizeof(double);
            break;
        case TYPE_BOOLEAN:
            bytesWritten += fwrite(&el->booleanValue, sizeof(uint8_t), 1, file->f);
            break;
        case TYPE_STRING:
            bytesWritten += fwrite(&el->stringValue.size, sizeof(uint32_t), 1, file->f) * sizeof(uint32_t);
            bytesWritten += fwrite(el->stringValue.data, sizeof(char), el->stringValue.size, file->f);
            break;
    }
    return bytesWritten;
}

enum write_status updateElement(struct file* file, block_index* index, page_header* header, element_t* oldEl, element_t* newEl){
    uint64_t newHeaderSize = 0;
    int64_t offsetOfElement = ftell(file->f) - index->block_offset;
    int64_t delta = (int64_t) newEl->stringValue.size - (int64_t) oldEl->stringValue.size;

    if (delta > 0) {
        if (index->block_offset + header->pageSize != file->header.fileSize) {
            int64_t oldPos = index->block_offset;
            int64_t newPos;
            page_header gapHeader = *header;
            int64_t diff = file->indexes.front ? (int64_t) file->indexes.front->fileSize - (int64_t) header->pageSize - delta : -1;
            if (diff >= 0) {
                block_index gapIndex = getIndex(file, file->indexes.front->index);
                if (gapIndex.type != DEAD_INDEX || updateIndex(file, file->indexes.front->index, index->block_offset,USING_INDEX) != WRITE_OK) return WRITE_WRONG;
                gapHeader.index = file->indexes.front->index; // записываем в хедер для будущей дырки номер индекса текущей
                newPos = index->block_offset = gapIndex.block_offset;
                newHeaderSize = file->indexes.front->fileSize;
                listNode* node = popFront(&file->indexes);
                node->fileSize = header->pageSize;
                insertNode(node, &file->indexes);
                if (diff) {
                    uint8_t startOfUnusedSpaceMark = TYPE_DEFAULT;
                    fseek(file->f, newPos + (int64_t) header->pageSize, SEEK_SET);
                    if (!fwrite(&startOfUnusedSpaceMark, sizeof(uint8_t), 1, file->f)) return WRITE_WRONG;
                }
            } else {
                if ((!file->indexes.botton || file->indexes.botton->fileSize) && !moveFirstBlock(file)) return WRITE_WRONG;
                *index = getIndex(file, header->index);
                if (index->type != USING_INDEX) return WRITE_WRONG;
                if (index->block_offset + header->pageSize != file->header.fileSize) {
                    block_index gapIndex = getIndex(file, file->indexes.botton->index);
                    if (gapIndex.type != NEW_INDEX || updateIndex(file, file->indexes.botton->index, index->block_offset,DEAD_INDEX) != WRITE_OK) return WRITE_WRONG;
                    gapHeader.index = file->indexes.botton->index; // записываем в хедер для будущей дырки номер индекса текущей
                    listNode* node = popBotton(&file->indexes);
                    node->fileSize = header->pageSize;
                    insertNode(node, &file->indexes);
                    oldPos = index->block_offset;
                    newPos = index->block_offset = file->header.fileSize;
                    file->header.fileSize += (int64_t) header->pageSize + delta;
                } else {
                    newPos = oldPos;
                    file->header.fileSize += delta;
                }
                if (writeHeader(file) != WRITE_OK) return WRITE_WRONG;
            }
            if (newPos != oldPos) {
                if (updateIndex(file, header->index, newPos, USING_INDEX) != WRITE_OK || !transferData(file, &oldPos, &newPos, header->pageSize)) return WRITE_WRONG;
                fseek(file->f, oldPos - (int64_t) header->pageSize, SEEK_SET);
                if (!fwrite(&gapHeader, sizeof(page_header), 1, file->f)) return WRITE_WRONG;
            }
        } else {
            file->header.fileSize += delta;
            if (writeHeader(file) != WRITE_OK) return WRITE_WRONG;
        }
    }

    int64_t oldPos = (int64_t) (index->block_offset + offsetOfElement + sizeof(oldEl->type) + sizeof(oldEl->key) + sizeof(oldEl->stringValue.size) + oldEl->stringValue.size);
    int64_t newPos = oldPos + delta;
    if (!transferData(file, &oldPos, &newPos, index->block_offset + header->pageSize - oldPos)) return WRITE_WRONG;
    header->pageSize = newHeaderSize ? newHeaderSize : header->pageSize + delta;
    fseek(file->f, index->block_offset, SEEK_SET);
    if (!fwrite(header, sizeof(page_header), 1, file->f)) return WRITE_WRONG;
    fseek(file->f, index->block_offset + offsetOfElement, SEEK_SET);
    if (!writeElement(file, newEl)) return WRITE_WRONG;
    return WRITE_OK;
}

page_t* readPage(struct file* file, uint64_t index) {
    block_index ind = getIndex(file, index);
    if (ind.type == USING_INDEX) {
        page_header header;
        fseek(file->f, ind.block_offset, SEEK_SET);
        if(!fread(&header, sizeof(page_header), 1, file->f)) return NULL;
        page_t* page = createPage();
        if (page) {
            page->header = header;
            strcpy(page->schema->name, page->header.schemaName);
            uint64_t sz = sizeof(page_header);
            while (sz < header.pageSize) {
                element_t* el = malloc(sizeof(element_t));
                if (!el) {
                    destroyPage(page);
                    return NULL;
                }
                uint64_t correct = readElement(file, el, false);
                if (!correct) {
                    free(el);
                    destroyPage(page);
                    return NULL;
                }
                if(el->type == TYPE_DEFAULT){
                    sz = sizeof(page_header);
                }
                else{
                    sz += correct;
                    if(!addElement(page->schema, el)){
                        free(el);
                        destroyPage(page);
                        return NULL;
                    }
                }
            }
            return page;
        }
        return NULL;
    }
    return NULL;
}

uint64_t writePage(struct file* file, schema_t* schema, uint64_t siblingIndex) {
    if (!schema) return DOCUMENT_NOT_EXISTS;
    int64_t save_pos = ftell(file->f);
    page_header header;
    header.pageSize = calcSizeOfPage(schema);
    header.brotherIndex = siblingIndex;
    header.childIndex = DOCUMENT_NOT_EXISTS;
    memset(header.schemaName, 0, 16);
    strcpy(header.schemaName, schema->name);
    if (!file->indexes.front && !moveFirstBlock(file)) return DOCUMENT_NOT_EXISTS;
    uint64_t newSz = 0;
    int64_t diff = (int64_t)file->indexes.front->fileSize - (int64_t)header.pageSize;
    if (diff >= 0) {
        block_index index = getIndex(file, file->indexes.front->index);
        if (index.type != DEAD_INDEX || updateIndex(file, file->indexes.front->index, index.block_offset, USING_INDEX) != WRITE_OK) return DOCUMENT_NOT_EXISTS;
        header.index = file->indexes.front->index;
        header.id.offset = index.block_offset;
        if (diff) {
            uint8_t space = TYPE_DEFAULT;
            fseek(file->f, index.block_offset + (int64_t)header.pageSize, SEEK_SET);
            if (!fwrite(&space, sizeof(uint8_t), 1, file->f)) return DOCUMENT_NOT_EXISTS;
            newSz = file->indexes.front->fileSize;
        }
        free(popFront(&file->indexes));
    }
    else {
        if (file->indexes.botton->fileSize != 0 && !moveFirstBlock(file) || updateIndex(file, file->indexes.botton->index, file->header.fileSize, USING_INDEX) == WRITE_WRONG ) return DOCUMENT_NOT_EXISTS;
        header.index = file->indexes.botton->index;
        header.id.offset = file->header.fileSize;
        free(popBotton(&file->indexes));
        file->header.fileSize += (int64_t) header.pageSize;
        if (writeHeader(file) != WRITE_OK) return DOCUMENT_NOT_EXISTS;
    }
    uint64_t correct = header.pageSize;
    fseek(file->f, (header.id.offset + (int64_t)sizeof(page_header)), SEEK_SET);
    for (uint64_t i = 0; i < schema->size; i++) {
        correct -= writeElement(file, schema->elements[i]);
    }
    if (newSz) header.pageSize = newSz;
    header.id.timestamp = (uint32_t)time(NULL);
    fseek(file->f, header.id.offset, SEEK_SET);
    correct -= fwrite(&header, 1, sizeof(page_header), file->f);
    if (correct != 0) return DOCUMENT_NOT_EXISTS;
    // Проверка на root_index
    if(file->header.rootIndex == DOCUMENT_NOT_EXISTS){
        file->header.rootIndex = header.index;
        if(writeHeader(file) != WRITE_OK) return DOCUMENT_NOT_EXISTS;
    }
    fseek(file->f, save_pos, SEEK_SET);
    return header.index;
}

void printPage(page_t* page){
    if (page) {
        printf( "%s#%08X%016X {\n", page->header.schemaName, page->header.id.timestamp, page->header.id.offset);
        for (uint64_t i = 0; i < page->schema->size; i++) {
            printf("\t");
            printElement(page->schema->elements[i]);
        }
        printf("}\n");
    } else {
        printf("Document doesn't exist!\n");
    }
}

uint64_t formatElement(element_t element, char* buffer, uint64_t bufferSize) {
    uint64_t offset = 0;

    switch (element.type) {
        case 0: // int
            offset += snprintf(buffer + offset, bufferSize - offset, "int %s: %d\n", element.key, element.intValue);
            break;
        case 1: // float
            offset += snprintf(buffer + offset, bufferSize - offset, "float %s: %f\n", element.key, element.floatValue);
            break;
        case 2: // string
            offset += snprintf(buffer + offset, bufferSize - offset, "string %s: %s\n", element.key, element.stringValue.data);
            break;
        case 3: // bool
            offset += snprintf(buffer + offset, bufferSize - offset, "bool %s: %s\n", element.key, element.booleanValue ? "true" : "false");
            break;
        default:
            printf("Invalid element type!\n");
            break;
    }

    return offset;
}

uint64_t getElementSize(element_t element) {
    uint64_t size = sizeof(element.type) + sizeof(element.key);

    switch (element.type) {
        case 0: // int
            size += sizeof(element.intValue);
            break;
        case 1: // float
            size += sizeof(element.floatValue);
            break;
        case 2: // string
            size += sizeof(element.stringValue.size) + element.stringValue.size;
            break;
        case 3: // bool
            size += sizeof(element.booleanValue);
            break;
        default:
            printf("Invalid element type!\n");
            break;
    }

    return size;
}

char* getPageData(page_t* page) {
    char* data = NULL;
    uint64_t dataLength = 0;

    if (page) {
        // Вычисляем размер буфера для данных
        // Включаем размер заголовка и дополнительные символы форматирования
        uint64_t bufferSize = strlen(page->header.schemaName) + 22;
        for (uint64_t i = 0; i < page->schema->size; i++) {
            bufferSize += getElementSize(*page->schema->elements[i]) + 1;
        }

        // Выделяем память для буфера данных
        data = (char*) malloc(bufferSize);
        if (data == NULL) {
            printf("Failed to allocate memory for data buffer!\n");
            return NULL;
        }

        // Форматируем данные в буфер
        int offset = 0;
        offset += snprintf(data + offset, bufferSize - offset, "%s#%08X%016X {\n", page->header.schemaName, page->header.id.timestamp, page->header.id.offset);
        for (uint64_t i = 0; i < page->schema->size; i++) {
            offset += snprintf(data + offset, bufferSize - offset, "\t");
            offset += formatElement(*page->schema->elements[i], data + offset, bufferSize - offset);
        }
        offset += snprintf(data + offset, bufferSize - offset, "}\n");

        // Устанавливаем размер данных
        dataLength = offset;

    } else {
        // Документ не существует
        data = (char*) malloc(strlen("Document doesn't exist!\n") + 1);
        if (data == NULL) {
            printf("Failed to allocate memory for data buffer!\n");
            return NULL;
        }
        strcpy(data, "Document doesn't exist!\n");
        dataLength = strlen(data);
    }

    // Устанавливаем завершающий нулевой символ
    data[dataLength] = '\0';

    return data;
}

element_t* getElementFromPage(page_t* page, const char* key) {
    return page ? getElement(page->schema, key) : NULL;
}

schema_t* getSchemaFromPage(page_t* page) {
    return page ? page->schema : NULL;
}


void destroyPage(page_t* page){
    if (page) {
        if (page->schema) {
            destroySchema(page->schema);
        }
        free(page);
    }
}


