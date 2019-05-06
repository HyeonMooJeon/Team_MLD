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