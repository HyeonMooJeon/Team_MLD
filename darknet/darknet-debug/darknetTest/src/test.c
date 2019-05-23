#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include "test.h"
#include <stdlib.h>
#pragma comment(lib,"libmysql.lib")

#define MYSQLUSER "root"
#define MYSQLPW "sunmoon435"
#define MYSQLIP "localhost"

static MYSQL *conn;
//데이터베이스 연결

void loadmysql()
{
    printf("loadmysql start\n");
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        printf("conn == NULL\n");
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
//차량정보 저장2
int insert_car(int *carnumber, char time[], char path[]) {
    printf("insert db\n");
    char  query[250];
    sprintf(query, "insert into carnumber.recognize values('%d%d%d%d%d%d','%s', '%s','ray')",
        carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4],
        carnumber[5], time, path);
    if (mysql_query(conn, query)) return 1;
    return 0;
}
void closemysql() {
    mysql_close(conn);
    printf("DB close\n");
}

//번호판 정렬
/*
int selected_number(int number, int numbers[]) {
    for (int i = 0; i < sizeof(numbers) / sizeof(int); i++) {
        if (numbers[i] == number) return 0;
    }
    return 1;
}
void sort_number(FRAME_INFO *frame) {
    int selected[6];
    int index;
    int d = 0;

    float temp;
    for (int i = 0; i < 6; i++) {
        if (!selected_number(i, selected)) continue;
        index = i;
        temp = frame->x[i];
        for (int j = 0; j < 6; j++) {
            if (!selected_number(j, selected) || i==j) continue;
            if (temp > frame->x[j]) {
                index = j;
                temp = frame->x[j];
            }
        }
        frame->order[i] = index;
        selected[i] = index;
    }

}
*/
//번호판 정렬 알고리즘(선택정렬)
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
    newNode->data = frame;
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
    /*
    sprintf(buffer, "%d%d%d%d%d%d", list->data.car.full[0].num, list->data.car.full[1].num,
        list->data.car.full[2].num, list->data.car.full[3].num, list->data.car.full[4].num,
        list->data.car.full[5].num);
    printf("%s\n", buffer);
    */
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
//배열 계산
int count_number(int arr[]) {
    int number = 0;
    int temp = 0;
    for (int i = 0; i < 10; i++) {
        //printf("%d %d\n", sizeof(arr) / sizeof(int), arr[i]);
        if (temp < arr[i]) {
            number = i;
            temp = arr[i];
        }
    }
    return number;
}
//횟수 계산
int* get_car_info(FRAME_NODE *list, int size) {
    int count[10] = { 0, };
    int *number = (int*)malloc(sizeof(int*)*6);

    int ***arr = (int***)malloc(sizeof(int**)*size);
    int a, b, c, i;
    for (a = 0; a < size; a++) {
        *(arr + a) = (int**)malloc(sizeof(int*) * 2);
        for (b = 0; b < 2; b++) {
            *(*(arr + a) + b) = (int*)malloc(sizeof(int) * 6);
        }
    }
    if (arr == NULL) {
        printf("outt of memory!!!!!!!\n");
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

    for (i = 0; i < 6; i++) {
        for (a = 0; a < size; a++) {
            count[arr[a][0][i]] += 1;
        }
        number[i] = count_number(count);
        memset(count, 0, sizeof(count));
    }

    for (a = 0; a < size; a++) {
        for (b = 0; b < 2; b++) {
            free(*(*(arr + a) + b));
        }
        free(*(arr + a));
        //list = list->next;
    }
    free(arr);
    return number;
}
char* get_car_model(FRAME_NODE * list) {
    int i,last = 0,temp=0,number[5];
    char **models = (char**)malloc(sizeof(char*) * 5);
    for (i = 0; i < 5; i++)
        models[i] = (char*)malloc(sizeof(char) * 20);
    strcpy(models[0], "i40");
    strcpy(models[1], "morning");
    strcpy(models[2], "ray");
    strcpy(models[3], "santafe");
    strcpy(models[4], "starex");
    int number[5] = { 0, };
    while (list != NULL) {
        if (list->data.car.model.name != NULL) continue;
        if (strcmp(list->data.car.model.name, "i40")) number[0]++;
        else if (strcmp(list->data.car.model.name, "morning")) number[1]++;
        else if (strcmp(list->data.car.model.name, "ray")) number[2]++;
        else if (strcmp(list->data.car.model.name, "santafe")) number[3]++;
        else if (strcmp(list->data.car.model.name, "starex")) number[4]++;
    }
    for (i = 0; i < 5; i++) {
        if (temp < number[i]) {
            temp = number[i];
            last = i;
        }
    }
    char *last_model = (char*)malloc(sizeof(char) * 20);
    strcpy(last_model, models[i]);
    for (i = 0; i < 5; i++)
        free(models[i]);
    free(models);
    return last_model;
}
FRAME_NODE* saveNode(FRAME_NODE ** list, int * carnumber,char* model) {
    //FRAME_NODE * node = (FRAME_NODE*)malloc(sizeof(FRAME_NODE));
    while (!(*list) == NULL) {
        if (carnumber[0] == (*list)->data.car.full[0].num)
            if (carnumber[1] == (*list)->data.car.full[1].num)
                if (carnumber[2] == (*list)->data.car.full[2].num)
                    if (carnumber[3] == (*list)->data.car.full[3].num)
                        if (carnumber[4] == (*list)->data.car.full[4].num)
                            if (carnumber[5] == (*list)->data.car.full[5].num)
                                if (strcmp(model, (*list)->data.car.model.name))
                                    break;
        *list = (*list)->next;
    }
    return *list;
}