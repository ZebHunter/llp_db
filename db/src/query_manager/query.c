#include "../../include/query_manager/query.h"
#include "../../include/file_manager/document.h"

query_t* createQuery(type_t type, const char* schemaName, schema_t* newValue, condition_t* condition){
    query_t* query = malloc(sizeof(query_t));
    if(query){
        query->queryType = type;
        query->schemaName = schemaName;
        query->newValue = newValue;
        query->condition = condition;
        query->innerQuery = NULL;
        query->size = 0;
        return query;
    }
    return NULL;
}

query_t* createSelectQuery(const char* schemaName, condition_t* condition){
    if(strlen(schemaName) <= 15){
        return createQuery(SELECT_QUERY, schemaName, NULL, condition);
    }
    return NULL;
}

query_t* createInsertQuery(const char* schemaName, schema_t* newValues, condition_t* condition){
    if (!schemaName && (!newValues || condition)) {
        return NULL;
    }

    if (schemaName && (newValues || strlen(schemaName) > 15)) {
        return NULL;
    }
    return createQuery(INSERT_QUERY, schemaName, newValues, condition);
}

query_t* createUpdateQuery(const char* schemaName, schema_t* newValues, condition_t* condition){
    if(strlen(schemaName) <= 15 && schemaName && newValues){
        return createQuery(UPDATE_QUERY, schemaName, newValues, condition);
    }
    return NULL;
}


query_t* createDeleteQuery(const char* schemaName, condition_t* condition){
    if(schemaName && strlen(schemaName) <= 15){
        return createQuery(REMOVE_QUERY, schemaName, NULL, condition);
    }
    return NULL;
}


enum crud_status executeSelectQuery(struct file* file, iterator_t** iterator, query_t* query){
    if(query && query->queryType == SELECT_QUERY){
        *iterator = createIterator();
        uint64_t rootIndex = file->header.rootIndex;
        if (find(file, *iterator, query, &rootIndex, NULL)) return CRUD_OK;
    }
    return CRUD_WRONG;
}
enum crud_status executeInsertQuery(struct file* file, query_t* query){
    if(query && query->queryType == INSERT_QUERY && (query->schemaName || file->header.rootIndex == DOCUMENT_NOT_EXISTS)){
        uint64_t rootIndex = file->header.rootIndex;
        if(find(file, NULL, query, &rootIndex, &insertPage)) return CRUD_OK;
    }
    return CRUD_WRONG;
}

enum crud_status executeUpdateQuery(struct file* file, query_t* query){
    if(query && query->queryType == UPDATE_QUERY){
        uint64_t root = file->header.rootIndex;
        if(find(file, NULL, query, &root, &updatePage)) return CRUD_OK;
    }
    return CRUD_WRONG;
}

enum crud_status executeDeleteQuery(struct file* file, query_t* query){
    if(query && query->queryType == REMOVE_QUERY){
        uint64_t rootIndex = file->header.rootIndex;
        if (find(file, NULL, query, &rootIndex, &removePage)) return CRUD_OK;
    }
    return CRUD_WRONG;
}

bool checkPage(struct file* file, uint64_t index, query_t* query){
    if(query->queryType == INSERT_QUERY && !query->schemaName) return true;

    page_t* page = readPage(file, index);
    if(page){
        bool result = true;
        if(!strcmp(page->header.schemaName, query->schemaName)){
            if(query->newValue){
                for(uint64_t i = 0; i < query->newValue->size; i++){
                    if(!getElement(page->schema, query->newValue->elements[i]->key)){
                        destroyPage(page);
                        return false;
                    }
                }
            }
            if(query->condition){
                uint64_t count = 0;
                for(uint64_t i = 0; i < page->schema->size; i++){
                    if(checkCondition(query->condition, page->schema->elements[i])){
                        count += 1;
                    }
                }
                if(count == 0) result = false;
            }
            destroyPage(page);
            return !query->condition || result;
        }
        destroyPage(page);
    }
    return false;
}

enum crud_status insertPage(struct file* file, query_t* q, uint64_t* parentIndex){
    if(!q->newValue) return (q->innerQuery == 0);
    if(*parentIndex == DOCUMENT_NOT_EXISTS){
        *parentIndex = writePage(file, q->newValue, DOCUMENT_NOT_EXISTS);
        if(*parentIndex == DOCUMENT_NOT_EXISTS) return CRUD_WRONG;
    }
    else{
        block_index prtIndex = getIndex(file, *parentIndex);
        if(prtIndex.type != USING_INDEX) return CRUD_WRONG;
        fseek(file->f, prtIndex.block_offset, SEEK_SET);
        page_header pageHeader;
        if(!fread(&pageHeader, sizeof(page_header), 1, file->f)) return CRUD_WRONG;
        uint64_t i = writePage( file, q->newValue, pageHeader.childIndex);
        if(i == DOCUMENT_NOT_EXISTS) return CRUD_WRONG;
        prtIndex = getIndex(file, *parentIndex);
        if(prtIndex.type != USING_INDEX) return CRUD_WRONG;
        //перезапись заголовка родителя со ссылкой на нового ребенка
        pageHeader.childIndex = i;
        fseek(file->f, prtIndex.block_offset, SEEK_SET);
        if(!fwrite(&pageHeader, sizeof(page_header), 1, file->f)) return CRUD_WRONG;
        *parentIndex = i;
    }
    return CRUD_OK;
}

enum crud_status updatePage(struct file* file, query_t* q, uint64_t* index){
    if(!q->newValue) return (q->innerQuery == 0);
    block_index ind = getIndex(file, *index);
    if(ind.type == USING_INDEX) {
        fseek(file->f, ind.block_offset, SEEK_SET);
        page_header header;
        if(!fread(&header, sizeof(page_header), 1, file->f)) return CRUD_WRONG;
        uint64_t count = header.pageSize - sizeof(page_header);
        while(count > 0){
            element_t oldElement;
            uint64_t tmp = readElement(file, &oldElement, true);
            if(!tmp) return CRUD_WRONG;
            if(oldElement.type == TYPE_DEFAULT) count = 0;
            else{
                count -= tmp;
                element_t* newElement = getElement(q->newValue, oldElement.key);
                if(!newElement) return CRUD_WRONG;
                fseek(file->f, (int64_t) -tmp, SEEK_CUR);
                if(newElement->type == TYPE_STRING){
                    if(updateElement(file, &ind, &header, &oldElement, newElement) != WRITE_OK){
                        free(newElement);
                        return CRUD_WRONG;
                    }
                    else if(!writeElement(file, newElement)){
                        free(newElement);
                        return CRUD_WRONG;
                    }
                    fseek(file->f, 0, SEEK_CUR);
                }
            }
        }
        return CRUD_OK;
    }
    return CRUD_WRONG;
}

enum crud_status removePage(struct file* file, query_t* q, uint64_t* index){
    if(q && q->innerQuery) return CRUD_OK;
    block_index ind = getIndex(file, *index);
    if(ind.type != USING_INDEX) return CRUD_WRONG;
    fseek(file->f, ind.block_offset, SEEK_SET);
    page_header header;
    if(!fread(&header, sizeof(page_header), 1, file->f)) return CRUD_WRONG;
    uint64_t childIndex = header.childIndex;
    if(childIndex != DOCUMENT_NOT_EXISTS && removePage(file, NULL, &childIndex) != CRUD_OK) return CRUD_WRONG;
    if(!q){
        uint64_t siblingIndex = header.brotherIndex;
        if(siblingIndex != DOCUMENT_NOT_EXISTS && removePage(file, NULL, &siblingIndex) != CRUD_OK) return CRUD_WRONG;
    }

    insertNode(createNode(header.pageSize, *index), &file->indexes);

    if(file->header.rootIndex == *index){
        file->header.rootIndex = DOCUMENT_NOT_EXISTS;
        if(writeHeader(file) != WRITE_OK) return CRUD_WRONG;
    }
    if(updateIndex(file, *index, ind.block_offset, DEAD_INDEX)) return CRUD_WRONG;
    *index = DOCUMENT_NOT_EXISTS;
    return CRUD_OK;
}


bool addInnerQuery(query_t* query, query_t* inner){
    if(query && inner && query->queryType == inner->queryType){
        if((inner->queryType == INSERT_QUERY || inner->queryType == UPDATE_QUERY)
            && !inner->newValue && !inner->innerQuery) return false;
        query_t** tmp = realloc(query->innerQuery, sizeof(query_t*) * (query->size + 1));
        if(tmp){
            query->innerQuery = tmp;
            query->innerQuery[query->size++] = inner;
            return true;
        }
    }
    return false;
}

bool find(struct file* file, iterator_t* iterator, query_t* query, uint64_t* index,
          enum crud_status(*change)(struct file*, query_t*, uint64_t*)){

    bool search = checkPage(file, *index, query);
    if(search && (query->queryType == SELECT_QUERY || change(file, query, index) == CRUD_OK)){
        if(query->innerQuery){
            block_index parent = getIndex(file, *index);
            if(parent.type != USING_INDEX) return false;
            fseek(file->f, parent.block_offset, SEEK_SET);
            page_header header;
            if(!fread(&header, sizeof(page_header), 1, file->f)) return false;
            for(uint64_t i = 0; i < query->size; i++){
                if(query->queryType == INSERT_QUERY && query->innerQuery[i]->newValue){
                    uint64_t indexNumber = *index;
                    if(!find(file, iterator,  query->innerQuery[i], &indexNumber, change)) return false;
                }
                else {
                    bool atLeastOneFound = false;
                    uint64_t childIndexNumber = header.childIndex;
                    uint64_t lastBrotherIndex = DOCUMENT_NOT_EXISTS;
                    while (childIndexNumber != DOCUMENT_NOT_EXISTS) {
                        block_index childIndex = getIndex(file, childIndexNumber);
                        if (childIndex.type != USING_INDEX) return false;
                        fseek(file->f, childIndex.block_offset, SEEK_SET);
                        page_header childHeader;
                        if (!fread(&childHeader, sizeof(page_header), 1, file->f)) return false;
                        atLeastOneFound |= find(file, iterator, query->innerQuery[i], &childIndexNumber, change);
                        if (query->queryType == REMOVE_QUERY && childIndexNumber == DOCUMENT_NOT_EXISTS) {
                            block_index correct = getIndex(file, header.childIndex);
                            if(correct.type == USING_INDEX && lastBrotherIndex != DOCUMENT_NOT_EXISTS){
                                block_index lastBrother = getIndex(file, lastBrotherIndex);
                                page_header brother;
                                fseek(file->f, lastBrother.block_offset, SEEK_SET);
                                if(!fread(&brother, sizeof(page_header), 1, file->f)) return false;
                                brother.brotherIndex = childHeader.brotherIndex;
                                fseek(file->f, lastBrother.block_offset, SEEK_SET);
                                if (!fwrite(&brother, sizeof(page_header), 1, file->f)) return false;
                            }
                            else {
                                header.childIndex = childHeader.brotherIndex;
                                fseek(file->f, parent.block_offset, SEEK_SET);
                                if (!fwrite(&header, sizeof(page_header), 1, file->f)) return false;
                            }
                        }
                        if(childIndexNumber != DOCUMENT_NOT_EXISTS) lastBrotherIndex = childIndexNumber;
                        else lastBrotherIndex = childHeader.brotherIndex;
                        childIndexNumber = childHeader.brotherIndex;
                    }
                    if (!atLeastOneFound) return false;
                }
            }
            return true;
        } else if (query->queryType == SELECT_QUERY) {
            return addPage(iterator, *index);
        }
        return true;
    }
    return false;
}


void destroyQuery(query_t* query){
    if(query){
        if(query->innerQuery){
            for(uint64_t i = 0; i < query->size; i++){
                destroyQuery(query->innerQuery[i]);
            }
            free(query->innerQuery);
        }
        free(query);
    }
}
