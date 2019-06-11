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
    printf("%s\n", query);
    if (mysql_query(conn, query)) return 1;
    return 0;
}
//차량정보 저장3(웹캠,ip카메라 연동)
int insert_car_model(int *carnumber, char model[],char time[],char path_number[],char path_model[]) {
    char  query[250];
    sprintf(query, "insert into carnumber.recognize values('%d%d%d%d%d%d','%s',%s', '%s','%s')",
        carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4],
        carnumber[5], model,time, path_number,path_model);
    if (mysql_query(conn, query)) return 1;
    return 0;
}
char* get_model(int *carnumber) {
    char query[200];
    //sprintf(query, "select model_car from carnumber.go as g, carnumber.model as m where g.GO_License_Plate = '%d%d%d%d%d%d' and g.GO_car_model = m.model_key",
    //    carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4], carnumber[5]);
    sprintf(query, "select model_car from (select *,concat(left(GO_License_Plate, 2), right(GO_License_Plate, 4) )as plate from team_mld.go) as g, team_mld.model as m where g.plate = '%d%d%d%d%d%d' and g.GO_car_model = m.model_key",
        carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4], carnumber[5]);
    mysql_query(conn, query);
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);
    if (row == NULL) return NULL;
    char *model = (char*)malloc(sizeof(char) * 20);
    strcpy(model, row[0]);
    return model;
}
//차량정보 저장(상태 및 모델 체크 후 삽입)
int insert_car_infomation(int *carnumber, char model[], char time[], char path_number[], char path_model[]) {
    char query[300];
    //sprintf(query, "select model_car from carnumber.go as g, carnumber.model as m where g.GO_License_Plate = '%d%d%d%d%d%d' and g.GO_car_model = m.model_key",
    //    carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4], carnumber[5]);
    //mysql_query(conn, query);
    //MYSQL_RES *result = mysql_store_result(conn);
    //MYSQL_ROW row = mysql_fetch_row(result);
    //mysql_free_result(result);
    char *row = get_model(carnumber);
    if (row == NULL || strcmp(row, model))
        sprintf(query, "insert into carnumber.recognize values('%d%d%d%d%d%d',(select model_key from carnumber.model where model_car = '%s'),'%s',1,'%s', '%s', 102);",
            carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4], carnumber[5], model, time, path_number, path_model);
    else
        sprintf(query, "insert into carnumber.recognize values('%d%d%d%d%d%d',(select model_key from carnumber.model where model_car = '%s'),'%s',1,'%s', '%s',(select GO_car_state from (select *,concat(left(GO_License_Plate, 2), right(GO_License_Plate, 4) ) as plate from team_mld.go)goes where plate = '%d%d%d%d%d%d')); ",
            carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4], carnumber[5], model, time, path_number, path_model, carnumber[0], carnumber[1], carnumber[2], carnumber[3], carnumber[4], carnumber[5]);
    printf("%s\n", query);
    free(row);
    if (mysql_query(conn, query)) return 1;
    return 0;
}
void closemysql() {
    mysql_close(conn);
    printf("DB close\n");
}
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
        cvReleaseImage(&(remove->data.image));
        cvReleaseImage(&(remove->data.image_model));
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
    int *number = (int*)malloc(6);

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
//횟수 계산
int* get_car_info_weight(FRAME_NODE *list, int size) {
    int count[10] = { 0, };
    int *number = (int*)malloc(6);

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
            count[arr[a][0][i]] += arr[a][1][i] / size;
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
//최종 차량 모델 추출
char* get_car_model(FRAME_NODE * list) {
    int i, last = 0, temp = 0, number[5] = { 0, };
    char **models = (char**)malloc(sizeof(char*) * 5);
    for (i = 0; i < 5; i++)
        models[i] = (char*)malloc(sizeof(char) * 20);
    strcpy(models[0], "i40");
    strcpy(models[1], "morning");
    strcpy(models[2], "ray");
    strcpy(models[3], "santafe");
    strcpy(models[4], "starex");
    while (list != NULL) {
        if (list->data.car.model.name == NULL) continue;
        if (!strcmp(list->data.car.model.name, "i40")) number[0]++;
        else if (!strcmp(list->data.car.model.name, "morning")) number[1]++;
        else if (!strcmp(list->data.car.model.name, "ray")) number[2]++;
        else if (!strcmp(list->data.car.model.name, "santafe")) number[3]++;
        else if (!strcmp(list->data.car.model.name, "starex")) number[4]++;
        list = list->next;
    }
    for (i = 0; i < 5; i++) {
        if (temp < number[i]) {
            temp = number[i];
            last = i;
        }
    }
    char *last_model = (char*)malloc(sizeof(char) * 20);
    strcpy(last_model, models[last]);
    for (i = 0; i < 5; i++)
        free(models[i]);
    free(models);
    return last_model;
}
//최종 차량 노드 추출
FRAME_NODE* saveNode(FRAME_NODE ** list, int * carnumber, char* model) {
    //FRAME_NODE * node = (FRAME_NODE*)malloc(sizeof(FRAME_NODE));
    while (!(*list) == NULL) {
        if (carnumber[0] == (*list)->data.car.full[0].num)
            if (carnumber[1] == (*list)->data.car.full[1].num)
                if (carnumber[2] == (*list)->data.car.full[2].num)
                    if (carnumber[3] == (*list)->data.car.full[3].num)
                        if (carnumber[4] == (*list)->data.car.full[4].num)
                            if (carnumber[5] == (*list)->data.car.full[5].num)
                                if (!strcmp(model, (*list)->data.car.model.name))
                                    break;
        *list = (*list)->next;
    }
    return *list;
}