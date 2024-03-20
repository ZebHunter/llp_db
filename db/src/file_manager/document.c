#include "../../include/file_manager/document.h"
#include "../../include/file_manager/page.h"

enum write_status writeHeader(struct file* file)
{
    int64_t save_pos = ftell(file->f);
    rewind(file->f);
    if (!fwrite(&file->header, sizeof(document_header), 1, file->f)) {
        return WRITE_WRONG;
    }
    fseek(file->f, save_pos, SEEK_SET);
    return WRITE_OK;
}

enum read_status loadIndexList(struct file* file) {
    if (file) {
        int64_t position = sizeof(document_header);
        for (uint64_t i = 0; i < file->header.indexCount; i++) {
            position += sizeof(block_index);
            block_index index;
            if (!fread(&index, sizeof(block_index), 1, file->f)) return READ_WRONG;
            if (index.type == NEW_INDEX) {
                insertNode(createNode(0, i), &file->indexes);
            }
            else if (index.type == DEAD_INDEX) {
                page_header header;
                fseek(file->f, index.block_offset, SEEK_SET);
                if (!fread(&header, sizeof(page_header), 1, file->f)) return READ_WRONG;
                insertNode(createNode(header.pageSize, i), &file->indexes);
                fseek(file->f, position, SEEK_SET);
            }
        }
        return READ_OK;
    }
    return READ_WRONG;
}

enum write_status updateIndex(struct file* file, uint64_t number, uint64_t offset, uint8_t type) {
    if (number > file->header.indexCount) return WRITE_WRONG;
    int64_t pos = ftell(file->f);
    block_index index = getIndex(file, number);
    index.block_offset = offset;
    index.type = type;
    fseek(file->f, (sizeof(document_header) + sizeof(block_index) * number), SEEK_SET);
    if (!fwrite(&index, sizeof(block_index), 1, file->f)) return WRITE_WRONG;
    fseek(file->f, pos, SEEK_SET);
    return WRITE_OK;
}

block_index getIndex(struct file* file, uint64_t index_number)
{
    block_index index = { 0, UNKNOWN_INDEX };
    if (index_number < file->header.indexCount) {
        fseek(file->f, (sizeof(document_header) + sizeof(block_index) * index_number), SEEK_SET);
        fread(&index, sizeof(block_index), 1, file->f);
    }
    return index;
}


enum write_status writeNewIndexes(struct file* file, uint64_t count, uint64_t last_index)
{
    block_index index = { 0 , NEW_INDEX };
    if(file->header.indexCount == 0)
        fseek(file->f, sizeof(document_header), SEEK_SET);
    else fseek(file->f, sizeof(document_header) + sizeof(block_index) * file->header.indexCount, SEEK_SET);
    uint64_t wrtBytes = 0;
    for (uint64_t i = last_index; i < count + last_index; i++) {
        wrtBytes += fwrite(&index, sizeof(block_index), 1, file->f);
        insertNode(createNode(0, file->header.indexCount++), &file->indexes);
    }

    if (wrtBytes == count) return WRITE_OK;

    return WRITE_WRONG;
}

struct file* createFile(const char* name)
{
    struct file* file = malloc(sizeof(struct file));
    if (file) {
        file->f = fopen(name, "w+b");
        file->header = (document_header) { sizeof(document_header) + sizeof(block_index) * DEFAULT_VALUE,
                                                  0, 0, DOCUMENT_NOT_EXISTS};
        file->indexes = (sortList) { NULL, NULL };
        if (file->f && writeNewIndexes(file, DEFAULT_VALUE, 0) == WRITE_OK && writeHeader(file) == WRITE_OK) {
            return file;
        }
    }
    closeFile(file);
    return NULL;
}

struct file* loadFile(const char* name)
{
    struct file* file = malloc(sizeof(struct file));
    if (file) {
        file->f = fopen(name, "r+b");
        file->indexes = (sortList){ NULL, NULL };
        if (file->f && fread(&file->header, sizeof(document_header), 1, file->f) ) {
            if( loadIndexList(file) == READ_OK)
                return file;
            else {
                closeFile(file);
                return NULL;
            }
        }
    }
    closeFile(file);
    return NULL;

}

void closeFile(struct file* file)
{
    if (file) {
        if (file->f) {
            fclose(file->f);
        }
        destroyList(&file->indexes);
        free(file);
    }
}


bool transferData(struct file* file, int64_t* old_position, int64_t* new_position, uint64_t size) {
    while (size) {
        uint64_t bufferSize;
        if (size > BUFFER_SIZE)
            bufferSize = BUFFER_SIZE;
        else
            bufferSize = size;
        size -= bufferSize;
        uint64_t* buf = malloc(bufferSize);
        fseek(file->f, *old_position, SEEK_SET);
        if (!fread(buf, bufferSize, 1, file->f)) {
            free(buf);
            return false;
        }
        *old_position += bufferSize;
        fseek(file->f, *new_position, SEEK_SET);
        if (!fwrite(buf, bufferSize, 1, file->f)) {
            free(buf);
            return false;
        }
        *new_position += bufferSize;
        free(buf);
    }
    return true;
}

bool moveFirstBlock(struct file* file) {
    uint64_t count = file->header.indexCount;
	int64_t newPosition;
	int64_t oldPosition = (int64_t) sizeof(document_header) + (int64_t) sizeof(block_index) * file->header.indexCount + file->header.firstPageOffset;
	int64_t needSpace = sizeof(block_index) * DEFAULT_VALUE;
	int64_t availableSpace = file->header.firstPageOffset;
	while (needSpace > availableSpace) {
		fseek(file->f, oldPosition, SEEK_SET);
		page_header header;
		if (!fread(&header, sizeof(page_header), 1, file->f)) return false;
        block_index index = getIndex(file, header.index);
        uint64_t newHeader = 0;
        if(index.type == USING_INDEX){
            if(file->indexes.front && file->indexes.front->fileSize >= header.pageSize){
                block_index gap = getIndex(file, file->indexes.front->index);
                if(gap.type != DEAD_INDEX || updateIndex(file, file->indexes.front->index, 0, NEW_INDEX) != WRITE_OK) return false;
                newPosition = (int64_t) gap.block_offset;
                newHeader = file->indexes.front->fileSize;
                listNode* node = popFront(&file->indexes);
                node->fileSize = 0;
                insertNode(node, &file->indexes);
                if(newHeader > header.pageSize) {
                    uint8_t startUnused = TYPE_DEFAULT;
                    fseek(file->f, newPosition + (int64_t) header.pageSize, SEEK_SET);
                    if (!fwrite(&startUnused, sizeof(uint8_t), 1, file->f)) return false;
                }
            }
            else {
                newPosition = (int64_t) file->header.fileSize;
                file->header.fileSize += header.pageSize;
                if(writeHeader(file) != WRITE_OK) return false;
            }
            if(updateIndex(file, header.index, newPosition, USING_INDEX) != WRITE_OK || !transferData(file, &oldPosition, &newPosition, header.pageSize)) return false;
        }
        else if(index.type == DEAD_INDEX){
            if(!removeByIndex(&file->indexes, header.index) || updateIndex(file, header.index, 0, NEW_INDEX) != WRITE_OK) return false;
            insertNode(createNode(0, header.index), &file->indexes);
            oldPosition += (int64_t) header.pageSize;
        }
        else return false;

        availableSpace += (int64_t)header.pageSize;
        if(newHeader){
            fseek(file->f, newPosition - (int64_t)header.pageSize, SEEK_SET);
            header.pageSize = newHeader;
            if(!fwrite(&header, sizeof(page_header), 1, file->f)) return false;
        }
	}
    file->header.firstPageOffset = availableSpace % sizeof(block_index);

    if(writeNewIndexes(file, availableSpace / sizeof(block_index), count - 1) != WRITE_OK || writeHeader(file) != WRITE_OK) return false;

    return true;
}
