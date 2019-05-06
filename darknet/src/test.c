#include <stdio.h>
#include <mysql.h>
#include "test.h"
#pragma comment(lib,"libmysql.lib")

#define MYSQLUSER "root"
#define MYSQLPW "sunmoon435"
#define MYSQLIP "localhost"

static MYSQL *conn;
//데이터베이스 연결
void loadmysql()
{
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        Sleep(1000);
        exit(1);
    }
    if (!(mysql_real_connect(conn, MYSQLIP, MYSQLUSER, MYSQLPW, NULL, 0, NULL, 0) == NULL))
    {
        printf("DB connect!\n");
        mysql_set_character_set(conn, "euckr");
    }
    else {
        fprintf(stderr, "DB connect error : %s\n", mysql_error(conn));
        getchar();
    }
    return;
}
//차량정보 저장
int insert_car_info(int carnumber) {
    char  query[250];
    sprintf(query, "insert into carnumber.car values(%d)", carnumber);
    if (mysql_query(conn, query)) return 1;
    return 0;
    
}
//DB 연결 종료
void closemysql() {
    mysql_close(conn);
    printf("DB close\n");
}
//번호판 정렬(선택정렬)
void sort_number(FRAME_INFO *frame) {
    NUMBER temp;
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 6; j++) {
            if (frame->car.full[i].x > frame->car.full[j].x) {
                temp = frame->car.full[j];
                frame->car.full[j] = frame->car.full[i];
                frame->car.full[i] = temp;
            }
        }
    }
}
//프레임 노드 생성
FRAME_NODE* create_node(FRAME_INFO frame) {
    FRAME_NODE * newNode = (FRAME_NODE*)malloc(sizeof(FRAME_NODE));
    newNode->data.car = frame.car;
    newNode->next = NULL;
    return newNode;
}
//프레임 노드 리스트 추가
void insert_frame(FRAME_NODE **list, FRAME_NODE *frame) {
    FRAME_NODE * ptr = *list;
    if (ptr == NULL) *list = frame;
    else {
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = frame;
    }
}
//연결리스트 해제
void releaselist(FRAME_NODE **list) {
    FRAME_NODE * remove;
    while (!(*list)->next == NULL) {
        remove = *list;
        *list = (*list)->next;
        free(remove);
    }
    free(*list);
}
//연결리스트 출력
void print_list(FRAME_NODE ** list) {
    char buffer[10];
    if (*list == NULL) return;

    printf("%d%d%d%d%d%d\n", (*list)->data.car.full[0].num, (*list)->data.car.full[1].num,
        (*list)->data.car.full[2].num, (*list)->data.car.full[3].num, (*list)->data.car.full[4].num,
        (*list)->data.car.full[5].num);
    print_list(&(*list)->next);
}
//연결리스트 갯수
int get_total_node(FRAME_NODE * list) {
    if (list == NULL) return 0;
    return (1 + get_total_node(list->next));
}
//횟수 계산
void get_car_info(FRAME_NODE *list, int size) {
    int ***arr = (int***)malloc(sizeof(int**)*size);
    int a, b, c, i;
    for (a = 0; a < size; a++) {
        *(arr + a) = (int**)malloc(sizeof(int*) * 2);
        for (b = 0; b < 2; b++) {
            *(*(arr + a) + b) = (int*)malloc(sizeof(int) * 6);
        }
    }
    if (arr == NULL) {
        printf("out of memory!!!!!!!\n");
        exit(1);
    }
    else {
        for (a = 0; a < size; a++) {
            for (b = 0; b < 2; b++) {
                for (c = 0; c < 6; c++) {
                    if (b == 0) {
                        arr[a][b][c] = list->data.car.full[c].num;
                    }
                    else {
                        arr[a][b][c] = (int)list->data.car.full[c].prod;
                    }
                }
            }
            list = list->next;
        }
    }

    for (a = 0; a < size; a++) {
        for (b = 0; b < 2; b++) {
            free(*(*(arr + a) + b));
        }
        free(*(arr + a));
        list = list->next;
    }
    free(arr);
}