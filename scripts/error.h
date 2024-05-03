#ifndef MMN14_ERROR_H
#define MMN14_ERROR_H
#include "boolean.h"

typedef enum error_type{
    NOTHING,
    FILE_NOT_FOUND
}error_message;


typedef struct error {
    error_message error_message;
    bool temp;

    char **error_data;
}error;

#endif //MMN14_ERROR_H
