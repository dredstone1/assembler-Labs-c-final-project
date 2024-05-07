#include "pos.h"

void set_pos(pos *pos, int line, int index){
    pos->line = line;
    pos->column = index;
}