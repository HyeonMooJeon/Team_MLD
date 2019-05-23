#include "image.h"

typedef struct {
    int num;
    float prod;
    float x;
    float y;
}NUMBER;
typedef struct {
    char name[20];
    float prod;
}MODEL;
typedef struct {
    NUMBER front[2];
    NUMBER back[4];
    NUMBER full[6];
    MODEL model;
}CAR;
typedef struct {
    CAR car;
    IplImage* image;
    IplImage* image_model;
    char path[50];
    char path_model[50];
    char time[50];
}FRAME_INFO;
typedef struct frame_node {
    FRAME_INFO data;
    struct frame_node* next;
}FRAME_NODE;

FRAME_NODE* saveNode(FRAME_NODE ** list, int * carnumber, char* model);
int* get_car_info(FRAME_NODE *list, int size);
char* get_car_model(FRAME_NODE * list);
FRAME_NODE* create_node(FRAME_INFO frame);
int get_total_node(FRAME_NODE * list);
void print_list(FRAME_NODE ** list);
void releaselist(FRAME_NODE **list);
void insert_frame(FRAME_NODE **list, FRAME_NODE *frame);
void sort_number(FRAME_INFO *frame);
void loadmysql();
//int insert_car_info(int carnumber);
int insert_car(int *carnumber, char time[], char path[]);
int insert_car_model(int *carnumber, char model[],char time[], char path_number[], char path_model[]);
void closemysql();