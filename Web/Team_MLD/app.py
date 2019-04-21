from flask import Flask, render_template, request, redirect, url_for, jsonify
import MySQLdb
from werkzeug.wrappers import json

app = Flask(__name__)
#DB로부터 데이터를 받을때 ASCII코드로 바뀌는걸 방지.
app.config['JSON_AS_ASCII'] = False
conn = MySQLdb.connect(host="localhost", user="root", password="root", db="team_mld", charset='utf8')


@app.route("/")
@app.route("/index")
def index():
    return render_template('index.html', title="SignUp")


@app.route("dev_login", methods=["POST"])
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




if __name__ == "__main__":
    app.run()