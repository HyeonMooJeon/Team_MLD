from flask import Flask, render_template, request, redirect, url_for, jsonify
import MySQLdb
from werkzeug.wrappers import json

app = Flask(__name__)
#DB로부터 데이터를 받을때 ASCII코드로 바뀌는걸 방지.
app.config['JSON_AS_ASCII'] = False
conn = MySQLdb.connect(host="localhost", user="root", password="root", db="team_mld", charset='utf8')


@app.route("/")
def index():
    return render_template('test_index.html', title="SignUp")


#등록 하는 법.
@app.route("/signUp", methods=["POST"])
def signUp():
    admin_id = str(request.form["id"])
    admin_pw= str(request.form["pw"])
    admin_name = str(request.form["userName"])

    cursor = conn.cursor()
    cursor.execute("SELECT name FROM login WHERE id='"+admin_id+"' ")
    check_id = cursor.fetchall()
    #등록하려는 아이디가 짧은 경우.
    if len(admin_id) < 2:
        return "아이디가 너무 짧습니다"

    #아이디가 존재하는 경우
    elif check_id:
        return "id가 존재한다."

    #아이디 등록.
    else:
        cursor.execute("insert into login (id, pw, name) values(%s, %s, %s)", (admin_id, admin_pw, admin_name))
        conn.commit()
        return redirect(url_for("login"))


#로그인시 나오는 페이지
@app.route("/develop")
def login():
    return render_template("login.html", title="data")


@app.route("/login", methods=["POST"])
def check():
    ID = str(request.form["log_id"])
    PW= str(request.form["log_pw"])
    cursor = conn.cursor()
    cursor.execute("SELECT name FROM login WHERE id='"+ID+"' AND pw='"+PW+"' ")
    user = cursor.fetchall()

    #fetchall() 은 전부 가져오고 fetchone()은 한줄만 불러옴. 전부 가져와서 비교해야돼니깐..
    #대소 문자 구별이 안되게 로그인이 되고있음.. //charset="utf8"로 수정됨.
    if user:
        return redirect(url_for("develop"))
    else:
        return "로그인 다시입력해봐"


@app.route("/board", methods=["POST"])
def develop():
    Dis_id = str(request.form["id"])
    Dis_pw= str(request.form["pw"])
    content = str(request.form["contents"])

    cursor = conn.cursor()
    cursor.execute("SELECT * FROM board")
    data = cursor.fetchall()
    payload = []

    for result in data:
        content = {'id': result[0], 'pw': result[1], "contents": result[2]}
        payload.append(content)

    if request.form["submit_button"] == "삽입":
        cur = conn.cursor()
        cur.execute("insert into board (id, pw, contents) values(%s, %s, %s)", (Dis_id, Dis_pw, content))
        cur.commit()
        return "삽입 완료"

    elif request.form["submit_button"] == "삭제":
        return "삭제"
    elif request.form["submit_button"] == "수정":
        return render_template('dev_page.html', data=jsonify(payload))
    #현제 테스트중,,,,삭제는 데이터 전송 확인, 수정은 표 표시하게 하기..

    elif request.form["submit_button"] == "확인":
        return jsonify(payload)

if __name__ == "__main__":
    app.run()