#ifndef MMN14_ERROR_H
#define MMN14_ERROR_H

#include "../data/boolean.h"

typedef enum error_type{
    NOTHING,
    FILE_NOT_FOUND,
    UNDEFINED_TAG_NAME,
    MEMORY_ALLOCATION_FAILED
}error_type;

typedef struct error {
    error_type error_type;
    bool temp;

    char **error_data;
}error;

void handel_error(error *error);

#endif //MMN14_ERROR_H
