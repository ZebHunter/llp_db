#include <time.h>
#include "../../include/tests/test.h"

uint32_t true_rand(uint32_t start, uint32_t end) {
    register uint32_t r;
    asm("rdrand %0"
            :"=r"(r)
            );

    return start + r % (end - start + 1);
}

void testInsert(const char* fileName, query_t* insertRoot){
    struct file* file = createFile(fileName);
    FILE* add = fopen("add.csv", "r+");
    fprintf(add, "index,time\n");
    fclose(add);
    if(executeInsertQuery(file, insertRoot) != CRUD_OK){
        printf("ERROR");
        return;
    }
    uint64_t index = 0;
    uint64_t clock_start;
    uint64_t duration;
    bool flag = true;
    while (flag){
        add = fopen("add.csv", "a");
        schema_t* child = createSchema("child");
        addElement(child, createIntegerElement("id", (int32_t)index));
        addElement(child, createIntegerElement("childInt1", 111));
        addElement(child, createIntegerElement("childInt2", 222));
        query_t* insert = createInsertQuery("root", NULL, NULL);
        addInnerQuery(insert, createInsertQuery(NULL, child, NULL));
        clock_start = clock();
        if(executeInsertQuery(file, insert) != CRUD_OK) {
            printf("error");
            flag = false;
        }
        duration = clock() - clock_start;
        fprintf(add, "%lu, %lu\n", index, duration);
        fclose(add);
        index += 1;
        destroyQuery(insert);
        destroySchema(child);
    }
}

void testDelete(const char* fileName, const char* schemaName){
    struct file* file = loadFile(fileName);
    FILE* add = fopen("del.csv", "r+");
    fprintf(add, "index,time\n");
    fclose(add);
    uint64_t index = 0;
    uint64_t clock_start;
    uint64_t duration;
    bool flag = true;
    while (flag){
        add = fopen("del.csv", "a");
        query_t* delete = createDeleteQuery("root", NULL);
        query_t* delQuery = createDeleteQuery(schemaName, conditionEquals(createIntegerElement("id", (int32_t)index)));
        addInnerQuery(delete, delQuery);
        clock_start = clock();
        if(executeDeleteQuery(file, delete) != CRUD_OK) {
            printf("error");
            flag = false;
        }
        duration = clock() - clock_start;
        destroyQuery(delete);
        fprintf(add, "%lu, %lu\n", index, duration);
        fflush(add);
        fclose(add);
        index += 1;
    }
}

void testAddDelete(const char* fileName, query_t* insertRoot){
    struct file* file = createFile(fileName);
    FILE* add = fopen("add_del.csv", "r+");
    fprintf(add, "index,time\n");
    fclose(add);
    if(executeInsertQuery(file, insertRoot) != CRUD_OK){
        printf("ERROR");
        return;
    }
    int32_t index = 1;
    int32_t indexes[400];
    uint64_t operation = 0;
    uint32_t end = 500;
    for(int i = 0; i < 400; i++){
        indexes[i] = 0;
    }
    uint64_t clock_start;
    uint64_t duration;
    bool flag = true;
    while (flag){
        for(index; index <= end; index++){
            add = fopen("add_del.csv", "a");
            schema_t* child = createSchema("child");
            int32_t copy = index;
            bool correct = true;
            for(int i = 0; i < 400; i++){
                if(indexes[i] != 0){
                    index = indexes[i];
                    indexes[i] = 0;
                    correct = false;
                    break;
                }
            }
            if(correct){
                index = copy;
            }
            addElement(child, createIntegerElement("id", index));
            query_t* insert = createInsertQuery("root", NULL, NULL);
            addInnerQuery(insert, createInsertQuery(NULL, child, NULL));


            clock_start = clock();
            if(executeInsertQuery(file, insert) != CRUD_OK) {
                printf("error");
                flag = false;
            }
            duration = clock() - clock_start;
            fprintf(add, "%lu, %lu\n", operation, duration);
            fclose(add);
            destroyQuery(insert);
            destroySchema(child);
            operation += 1;
        }



        for(int i = 0; i < 400; i++){
            uint32_t rand = 0;
            bool flags = true;
            while(flags){
                rand = true_rand(1, end);
                bool a = true;
                for(int j = 0; j < 400; j++){
                    if(rand == indexes[j]){
                        a = false;
                        break;
                    }
                }
                if(a){
                    indexes[i] = (int32_t)rand;
                    flags = false;
                }
            }

            add = fopen("add_del.csv", "a");
            query_t* delete = createDeleteQuery("root", NULL);
            query_t* delQuery = createDeleteQuery("child", conditionEquals(createIntegerElement("id", (int32_t)rand)));
            addInnerQuery(delete, delQuery);
            clock_start = clock();
            if(executeDeleteQuery(file, delete) != CRUD_OK) {
                printf("error");
                flag = false;
            }
            duration = clock() - clock_start;
            destroyQuery(delete);
            fprintf(add, "%lu, %lu\n", operation, duration);
            fflush(add);
            fclose(add);
            operation += 1;
        }
        end += 100;
    }
}
