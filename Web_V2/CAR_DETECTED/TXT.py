import MySQLdb

db = MySQLdb.connect(host="localhost",
                     user="id",
                     passwd="passwd",
                     db="db",
                     charset='utf8')

cur = db.cursor()

cur.execute("SELECT * FROM [테이블 명] INTO OUTFILE '[파일경로] [파일이름.txt]' FIELDS TERMINATED BY ',' LINES TERMINATED BY'';")
# 경로 /로 구분 FIELDS = 필드 간 토큰 설정 LINES = 라인 간 토큰 설정
cur.close()
conn.close()
db.close()