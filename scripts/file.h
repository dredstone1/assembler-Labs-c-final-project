#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#define LINE_SIZE 80
void read_file(char *filename, char **file_content, int *size);

typedef enum bool{
    FALSE,
    TRUE
} bool;

typedef struct line{
    char content[LINE_SIZE];
    int size;
    bool ext;
    bool ent;
} line;

typedef struct pos{
    int line;
    int column;
} pos;

typedef struct file{
    line **lines;
    int size;
    char *filename;
    bool ext;
    bool ent;
    pos pos;
} file;


#endif //MMN14_FILE_H
