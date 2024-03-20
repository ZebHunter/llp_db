#include <stdio.h>
#include "../include/query_manager/query.h"
#include "../include/tests/test.h"



int main(int argc, char** argv) {
    struct file* file = loadFile("demo_db");
    if (!file) {
        file = createFile("demo_db");
        if (!file) {
            printf("Error\n");
            exit(-1);
        }
    }


    schema_t *rootSchema = createSchema("root");
    if (rootSchema) {
        addElement(rootSchema, createIntegerElement("rootInt1", 123));
        addElement(rootSchema, createIntegerElement("rootInt2", 456));
        addElement(rootSchema, createIntegerElement("rootInt3", 789));
        addElement(rootSchema, createDoubleElement("rootDouble", 128.128));
    }

    query_t* insert = createInsertQuery(NULL, rootSchema, NULL);
    executeInsertQuery(file, insert);

    schema_t *newRoot = createSchema(NULL);
    addElement(rootSchema, createIntegerElement("rootInt1", 12));
    addElement(rootSchema, createIntegerElement("rootInt2", 6));
    addElement(rootSchema, createIntegerElement("rootInt3", 789));
    addElement(rootSchema, createDoubleElement("rootDouble", 128.19));

    query_t* update = createUpdateQuery("root", newRoot, NULL);
    if(executeUpdateQuery(file, update) != CRUD_OK) printf("JOPA");

//    testInsert("add.db", insert);
//   testDelete("add.db", "child");

//    testAddDelete("add_del.db", insert);
    closeFile(file);
    return 0;
}