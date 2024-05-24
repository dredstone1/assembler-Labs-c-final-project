#include "compiler.h"
#include "systems/file.h"
#include "assembler/assembler.h"

int main(int argc, char **argv){
    file file1;
    error error;

    error.error_type = NOTHING;
    file1.filename = "C:\\Users\\mayan\\Desktop\\shared\\mmn14_files\\ps2.as";
    read_file(&file1, &error);
    assembler(file1);





    return 0;
}

