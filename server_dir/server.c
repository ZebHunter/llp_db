#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../spec.pb-c.h"
#include <arpa/inet.h>
#include "deserealizator.h"

#define PORT 3112
#define SA struct sockaddr

Element* convertElement(page_t* page, int index){
    if(page){
        Element* element = malloc(sizeof(Element));
        element__init(element);
        FieldsT* field = malloc(sizeof(FieldsT));
        fields_t__init(field);
        element->name = page->schema->elements[index]->key;
        printf("%s\n", element->name);
        printf("%zu\n", page->schema->size);
        switch(page->schema->elements[index]->type){
            case TYPE_INTEGER:
                element->type = ELEMENT_TYPE__INTEGER;
                field->integervalue = page->schema->elements[index]->intValue;
                break;
            case TYPE_FLOAT:
                element->type = ELEMENT_TYPE__FLOAT;
                field->doublevalue = page->schema->elements[index]->floatValue;
                break;
            case TYPE_BOOLEAN:
                element->type = ELEMENT_TYPE__BOOLEAN;
                field->booleanvalue = page->schema->elements[index]->booleanValue;
                break;
            case TYPE_STRING:
                element->type = ELEMENT_TYPE__STRING;
                field->stringvalue = page->schema->elements[index]->stringValue.data;
                break;
        }
        element->field = field;
        if (page->schema->size > (index + 1)) element->el = convertElement(page, index + 1);
        return element;
    }
    return NULL;
}

PageT* convertPage(struct file* file, iterator_t* iterator){
    if(iterator && hasNext(iterator)){
        page_t* page = next(file, iterator);
        PageT* pageT = malloc(sizeof(PageT));
        page_t__init(pageT);
        pageT->timestamp = (int32_t)page->header.id.timestamp;
        pageT->name = page->header.schemaName;
        Element* element = malloc(sizeof(Element));
        element__init(element);
        element = convertElement(page, 0);
        pageT->list = element;
        if(hasNext(iterator)) pageT->page = convertPage(file, iterator);
        return pageT;
    }
    return NULL;
}

void send_message(Response *msg, int sockfd) {
    uint64_t message_size = response__get_packed_size(msg);
    uint8_t *buffer = malloc(message_size);
    response__pack(msg, buffer);
    send(sockfd, buffer, message_size, 0);
    free(buffer);
}

void server_jobs(int connfd, struct sockaddr_in cli, char* filename) {
    struct file* file = loadFile(filename);
    if (!file) {
        file = createFile(filename);
        if(!file){
            fprintf(stderr, "Error: couldn't open or create file!\n");
        }
    }
    for (;;) {
        char res_str[1024];
        memset(res_str, 0, 1024);
        uint8_t buffer[4096];
        uint64_t bytes_received = recv(connfd, &buffer, 4096, 0);
        AstNodeT *msg = ast_node_t__unpack(NULL, bytes_received, buffer);
        //printStruct(msg);

        Response* res = malloc(sizeof(Response));
        response__init(res);
        AstNodeT* querySetNode = msg->right;
        while (querySetNode) {
            AstNodeT* queryNode = querySetNode->left;
            query_t* q = deserializeQueryNode(queryNode);
            switch (msg->type) {
                case NODE_TYPE_T__SELECT_QUERY_NODE: {
                    iterator_t* it;
                    executeSelectQuery(file, &it, q) == CRUD_OK ?  strcat(res_str, "Successful SELECT!\n") : strcat(res_str, "Failed to SELECT!\n");
                    res->page_list = convertPage(file, it);
                    while (hasNext(it)) {
                        page_t* page = next(file, it);
                        printPage(page);
                        destroyPage(page);
                    }
                    destroyIterator(it);
                    break;
                }
                case NODE_TYPE_T__INSERT_QUERY_NODE:
                    (executeInsertQuery(file, q) == CRUD_OK) ? strcat(res_str, "Successful INSERT!\n") : strcat(res_str, "Failed to INSERT!\n");

                    break;
                case NODE_TYPE_T__UPDATE_QUERY_NODE:
                    (executeUpdateQuery(file, q) == CRUD_OK) ? strcat(res_str,"Successful UPDATE!\n") : strcat(res_str,"Failed to UPDATE!\n");
                    break;
                case NODE_TYPE_T__DELETE_QUERY_NODE:
                    (executeDeleteQuery(file, q) == CRUD_OK) ? strcat(res_str,"Successful DELETE!\n"): strcat(res_str,"Failed to DELETE!\n");
                    break;
            }

            querySetNode = querySetNode->right ? querySetNode->right : NULL;
            destroyQuery(q);
        }



        res->result = res_str;
        send_message(res, connfd);
        ast_node_t__free_unpacked(msg, NULL);
    }

    close(connfd);
    exit(0);
}

int main(int argc, char* argv[]) {
    int sockfd, connfd;
    uint32_t len;
    struct sockaddr_in servaddr, cli;
    char* filename = argv[1];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *) &servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    len = sizeof(cli);


    listen_loop:;
    connfd = accept(sockfd, (SA *) &cli, &len);
    if (connfd < 0) {
        printf("server_dir accept failed...\n");
        exit(0);
    } else if (!fork()) {
        printf("Got Client connection: %d %s\n", connfd, inet_ntoa(cli.sin_addr));
        server_jobs(connfd, cli, filename);
    } else {
        goto listen_loop;
    }


}