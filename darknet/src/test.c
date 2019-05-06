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