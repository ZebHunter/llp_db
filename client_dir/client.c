#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include "serealizator.h"
#include "../spec.pb-c.h"

#define PORT 3112
#define SA struct sockaddr

extern void yyset_lineno(int num);

extern void yyrestart(FILE* f);

extern int yyparse(astNode** ast);

void deserializeElement(Element* element){
    printf("\t");
    printf("%s = ", element->name);
    switch (element->type) {
        case ELEMENT_TYPE__INTEGER:
            printf("%d\n", element->field->integervalue);
            break;
        case ELEMENT_TYPE__FLOAT:
            printf("%f\n", element->field->doublevalue);
            break;
        case ELEMENT_TYPE__BOOLEAN:
            printf("%s\n", element->field->booleanvalue ? "true" : "false");
            break;
        case ELEMENT_TYPE__STRING:
            printf("%s\n", element->field->stringvalue);
            break;
    }
    if(element->el) deserializeElement(element->el);
}

void deserializePage(PageT* page){
    printf("time: %d\n", page->timestamp);
    printf("%s {\n", page->name);
    deserializeElement(page->list);
    printf("}\n");
    if(page->page) deserializePage(page->page);
}

void deserializeResponse(Response* response){
    printf("%s", response->result);
    if(response->page_list){
        deserializePage(response->page_list);
    }
}

void send_message(AstNodeT *msg, int sockfd) {
    uint64_t message_size = ast_node_t__get_packed_size(msg);
    uint8_t *buffer = malloc(message_size);
    ast_node_t__pack(msg, buffer);
    send(sockfd, buffer, message_size, 0);
    free(buffer);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        exit(0);
    }

    AstNodeT* msg;
    uint8_t buf[4096];
    astNode* ast = NULL;
    do {
        printf("> ");
        yyset_lineno(1); // сброс счётчика строк лексера
        yyrestart(stdin); // чистка буфера (чтобы после ошибок всё было ок)
        if (yyparse(&ast) == 0) {
            //printNode(ast, 0);
            // Выход по слову "exit":
            if (!ast) {
                printf("Leaving...\n");
                break;
            }
            msg = serialize(NULL, ast);
            send_message(msg, sockfd);

            uint64_t recieved = recv(sockfd, &buf, 4096, 0);
            Response* res = response__unpack(NULL, recieved, buf);
            deserializeResponse(res);
            response__free_unpacked(res, NULL);
        }

    } while (true);

    return 0;
}
