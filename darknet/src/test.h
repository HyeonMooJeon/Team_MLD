typedef struct {
    int num;
    float prod;
    float x;
    float y;
}NUMBER;
typedef struct {
    NUMBER front[2];
    NUMBER back[4];
    NUMBER full[6];
}CARNUMBER;
typedef struct {
    CARNUMBER car;
}FRAME_INFO;
typedef struct frame_node {
    FRAME_INFO data;
    struct frame_info* next;
}FRAME_NODE;

void sort_number(FRAME_INFO *frame);
void loadmysql();
int insert_car_info(int carnumber);
void closemysql();
void get_car_info(FRAME_NODE *list, int size);
FRAME_NODE* create_node(FRAME_INFO frame);
int get_total_node(FRAME_NODE * list);
void print_list(FRAME_NODE ** list);
void releaselist(FRAME_NODE **list);
void insert_frame(FRAME_NODE **list, FRAME_NODE *frame);