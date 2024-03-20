cmake_minimum_required(VERSION 3.16)
project(llp_3 C)
set(CMAKE_C_COMPILER gcc)

set(CMAKE_C_STANDARD 11)
set(CLIENT_DIR client_dir)
set(SERVER_DIR server_dir)
set(DB_DIR db)


add_executable(client_exec ${CLIENT_DIR}/client.c
        ${CLIENT_DIR}/structures/structures.h
        ${CLIENT_DIR}/structures/structures.c
        ${CLIENT_DIR}/parser.tab.h
        ${CLIENT_DIR}/parser.tab.c
        ${CLIENT_DIR}/lex.yy.c
        ${CLIENT_DIR}/serealizator.h
        ${CLIENT_DIR}/serealizator.c
        protobuf/protobuf-c/protobuf-c/protobuf-c.h
        protobuf/protobuf-c/protobuf-c/protobuf-c.c
        ./spec.pb-c.h
        ./spec.pb-c.c
        )

add_executable(server_exec  ${SERVER_DIR}/server.c
        ${DB_DIR}/include/file_manager/document.h
        ${DB_DIR}/include/file_manager/op_status.h
        ${DB_DIR}/src/file_manager/document.c
        ${DB_DIR}/include/file_manager/page.h
        ${DB_DIR}/src/file_manager/page.c
        ${DB_DIR}/include/query_manager/query.h
        ${DB_DIR}/src/query_manager/query.c
        ${DB_DIR}/include/query_manager/condition.h
        ${DB_DIR}/src/query_manager/condition.c
        ${DB_DIR}/include/schema_manager/schema.h
        ${DB_DIR}/src/schema_manager/schema.c
        ${DB_DIR}/include/schema_manager/element.h
        ${DB_DIR}/src/schema_manager/element.c
        ${DB_DIR}/include/utils/list.h
        ${DB_DIR}/src/utils/list.c
        ${DB_DIR}/include/utils/iterator.h
        ${DB_DIR}/src/utils/iterator.c
        ${SERVER_DIR}/deserealizator.h
        ${SERVER_DIR}/deserealizator.c
        protobuf/protobuf-c/protobuf-c/protobuf-c.h
        protobuf/protobuf-c/protobuf-c/protobuf-c.c
        ./spec.pb-c.h
        ./spec.pb-c.c
)
