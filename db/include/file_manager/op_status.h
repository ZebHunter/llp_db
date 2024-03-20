
#ifndef LLP_OP_STATUS_H
#define LLP_OP_STATUS_H
enum open_status {
    OPEN_OK = 0,
    OPEN_WRONG
};

enum close_status {
    CLOSE_OK = 0,
    CLOSE_WRONG
};


enum read_status{
    READ_OK = 0,
    READ_WRONG
};

enum write_status {
    WRITE_OK = 0,
    WRITE_WRONG
};

enum crud_status {
    CRUD_OK = 0,
    CRUD_WRONG
};

#endif //LLP_OP_STATUS_H
