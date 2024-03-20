CLIENT_DIR=client_dir
SERVER_DIR=server_dir
DB_DIR=db

.ONESHELL:

all: client_dir server_dir

spec : spec.proto
	protoc-c --c_out=. spec.proto

run_client: client_dir
	./client

run_server: server_dir
	./server testdb
clean: clean_server clean_client
	rm -f spec.pb-c.*

clean_client:
	cd $(CLIENT_DIR)
	rm -f parser.tab.c parser.tab.h parser.output lex.yy.c ../client
	cd ..

clean_server:
	rm -f server

parser: $(CLIENT_DIR)/parser/parser.y
	cd $(CLIENT_DIR)
	bison -d parser/parser.y
	cd ..

lexer: $(CLIENT_DIR)/lexer/lexer.l
	cd $(CLIENT_DIR)
	flex lexer/lexer.l
	cd ..

client: parser lexer $(CLIENT_DIR)/structures/structures.c $(CLIENT_DIR)/serealizator.c  $(CLIENT_DIR)/client.c spec
	cd $(CLIENT_DIR)
	gcc parser.tab.c lex.yy.c structures/structures.c serealizator.c client.c ../spec.pb-c.c -o ../client -lprotobuf-c
	cd ..

server: $(DB_DIR)/src/file_manager/document.c $(DB_DIR)/src/file_manager/page.c $(DB_DIR)/src/query_manager/query.c $(DB_DIR)/src/query_manager/condition.c $(DB_DIR)/src/schema_manager/schema.c $(DB_DIR)/src/schema_manager/element.c $(DB_DIR)/src/utils/list.c $(DB_DIR)/src/utils/iterator.c $(SERVER_DIR)/deserealizator.c $(SERVER_DIR)/server.c  spec
	cd $(SERVER_DIR)
	gcc ../$(DB_DIR)/src/file_manager/document.c ../$(DB_DIR)/src/file_manager/page.c ../$(DB_DIR)/src/query_manager/query.c ../$(DB_DIR)/src/query_manager/condition.c ../$(DB_DIR)/src/schema_manager/schema.c ../$(DB_DIR)/src/schema_manager/element.c ../$(DB_DIR)/src/utils/list.c ../$(DB_DIR)/src/utils/iterator.c ../spec.pb-c.c deserealizator.c server.c -o ../server -lprotobuf-c
	cd ..

.PHONY: spec