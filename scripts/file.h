#ifndef MMN14_FILE_H
#define MMN14_FILE_H
void read_file(char *filename, char **file_content, int *size);

typedef enum bool{
    FALSE,
    TRUE
} bool;

typedef struct line{
    char *content;
    int size;
    bool ext;
    bool ent;
};

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
