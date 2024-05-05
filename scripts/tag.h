#ifndef MMN14_TAG_H
#define MMN14_TAG_H

#include "boolean.h"

#define MAX_TAG_SIZE 31

typedef struct tag{
    char name[MAX_TAG_SIZE];
    bool tag;
} tag;

#endif //MMN14_TAG_H
