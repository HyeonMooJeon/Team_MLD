<<<<<<< HEAD
from flask import Flask, render_template, request, redirect, url_for,  Response, session, jsonify
import MySQLdb
import cv2
import json
from flask_sqlalchemy import SQLAlchemy
=======
from flask import Flask, render_template, request, redirect, url_for, jsonify
#import MySQLdb
#from werkzeug.wrappers import json
>>>>>>> f9e70ceece80a3ee0c9aec73f6840ccfd5fba55e

app = Flask(__name__)
app.secret_key = "super secret key"
vc = cv2.VideoCapture(0)
#DB로부터 데이터를 받을때 ASCII코드로 바뀌는걸 방지.
<<<<<<< HEAD
app.config['JSON_AS_ASCII'] = False
conn = MySQLdb.connect(host="localhost", user="root", password="sm435", db="testdb", charset='utf8')
=======
#app.config['JSON_AS_ASCII'] = False
#conn = MySQLdb.connect(host="localhost", user="root", password="root", db="team_mld", charset='utf8')
>>>>>>> f9e70ceece80a3ee0c9aec73f6840ccfd5fba55e

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


@app.route("/dev_page")
def managePage():
    return render_template("chart_test.html")


@app.route("/GetGoverment")
def goverment():
     cursor = conn.cursor()
     cursor.execute("SELECT go.GO_License_Plate, car_status.car_status_now, model.model_car FROM go  INNER JOIN car_status ON go.GO_car_state = car_status.car_status_key INNER JOIN model ON go.GO_car_model = model.model_key;")
     r = [dict((cursor.description[i][0], value)
               for i, value in enumerate(row)) for row in cursor.fetchall()]
     return jsonify(r)




@app.route('/login_normal')
def test_page():
    return render_template('test_index.html')


@app.route('/logout')
def logout():
    session.pop('admin', None)
    return render_template('/index.html')


@app.route('/login')
def login():
    return render_template('/login.html')


@app.route('/chart')
def chart():
    return render_template('/chart.html')





#YOLO 멀티 쓰레드
import threading
import os
def yolo():
    #darknet.exe path 설정
    os.chdir("C:\\Users\\cps435\\Desktop\\Loo\\Team_MLD\\darknet\\build\\darknet\\x64")
    #웹캠 연동
    os.system("darknet.exe detector demo data/coco.data cfg/yolov3-tiny.cfg weights/yolov3-tiny.weights data/test.mp4")

if __name__ == "__main__":
<<<<<<< HEAD
    app.run(host='127.0.0.1', debug=True, threaded=True)
=======
    yoloThread = threading.Thread(target = yolo, args = ())
    yoloThread.start()
    app.run()
>>>>>>> f9e70ceece80a3ee0c9aec73f6840ccfd5fba55e
