from flask import Flask, render_template, request, redirect, url_for, jsonify, Response, session
import MySQLdb
import cv2
from werkzeug.wrappers import json


app = Flask(__name__)
app.secret_key = "super secret key"
vc = cv2.VideoCapture(0)
#DB로부터 데이터를 받을때 ASCII코드로 바뀌는걸 방지.
app.config['JSON_AS_ASCII'] = False
conn = MySQLdb.connect(host="localhost", user="root", password="sm435", db="testdb", charset='utf8')

def gen():
    while True:
        rval, frame = vc.read()
        cv2.imwrite('t.jpg', frame)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + open('t.jpg', 'rb').read() + b'\r\n')


#시작화면
@app.route("/")
@app.route("/index")
def index():
    return render_template('index.html')


#비디오 동영상 HTML
@app.route("/video")
def streaming():
    return render_template('video.html')


#비디오 동영상 동작
@app.route('/video_feed')
def video_feed():
    return Response(gen(), mimetype='multipart/x-mixed-replace; boundary=frame')


#로그인시 나오는 페이지
@app.route("/login_admin")
def login_admin():
    return render_template("login_admin.html", title="data")


@app.route("/admin_login", methods=["POST"])
def check():
    ID = str(request.form["ID"])
    PW = str(request.form["PW"])
    cursor = conn.cursor()
    cursor.execute("SELECT admin_name FROM administor WHERE admin_id='"+ID+"' AND admin_pw='"+PW+"' ")
    user = cursor.fetchall()
    #fetchall() 은 전부 가져오고 fetchone()은 한줄만 불러옴. 전부 가져와서 비교해야돼니깐..
    #대소 문자 구별이 안되게 로그인이 되고있음.. //charset="utf8"로 수정됨.
    if user:
        session['admin'] = user
        return redirect(url_for("management"))
    else:
        return "로그인 다시입력해봐"


@app.route("/management")
def management():
    if 'admin' in session:
        return render_template('dev_page.html')
    else:
        return "Login First!"

@app.route('/test')
def test_page():
    return render_template('test_index.html')


@app.route('/logout')
def logout():
    session.pop('admin', None)
    return render_template('/index.html')


@app.route('/login')
def login():
    return render_template('/login.html')


if __name__ == "__main__":
    app.run(host='127.0.0.1', debug=True, threaded=True)