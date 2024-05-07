#ifndef MMN14_POS_H
#define MMN14_POS_H

typedef struct pos{
    int line;
    int column;
} pos;

void set_pos(pos *pos, int line, int index);

#endif //MMN14_POS_H
