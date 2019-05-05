from flask import Flask, render_template, request,  Response, session
import MySQLdb
import cv2
from flask.json import jsonify
from werkzeug.wrappers import json


app = Flask(__name__)
app.secret_key = "super secret key"
vc = cv2.VideoCapture(0)
#DB로부터 데이터를 받을때 ASCII코드로 바뀌는걸 방지.
app.config['JSON_AS_ASCII'] = False
conn = MySQLdb.connect(host="localhost", user="root", password="root", db="team_mld", charset='utf8')

#카메라 동작 방법.
def gen():
    while True:
        rval, frame = vc.read()
        cv2.imwrite('t.jpg', frame)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + open('t.jpg', 'rb').read() + b'\r\n')



#비디오 동영상 HTML
@app.route("/video")
def streaming():
    if 'user' in session:
        return render_template('video.html')
    else:
        return index()



#비디오 동영상 동작
@app.route('/video_feed')
def video_feed():
    return Response(gen(), mimetype='multipart/x-mixed-replace; boundary=frame')


#시작화면
@app.route("/")
@app.route('/login')
def login():
    return render_template('/login.html')


@app.route("/intro")
def intro():
    if 'user' in session:
        return render_template('intro.html')
    else:
        return index()



@app.route("/intro_not_regist")
def intro_not_regist():
    if 'user' in session:
        return index()
    else:
        return render_template('intro_not_regist.html')




@app.route("/chart")
def chart():
    if 'user' in session:
        return render_template('charts.html')
    else:
        index()



#차량 검색
@app.route("/lookup")
def lookup():
    if 'user' in session:
        return render_template('lookup.html')
    else:
        index()



#사용자 관리
@app.route("/user_manage")
def user_manage():
    if 'user' in session:
        return render_template('user_manage.html')
    else:
        index()



#실시간 영상
@app.route("/real_time")
def real_time():
    if 'user' in session:
        return render_template('real_time.html')
    else:
        index()



#DB관리
@app.route("/DB_manage")
def DB_manage():
    if 'user' in session:
        return render_template('DB_manage.html')
    else:
        index()




#로그아웃
@app.route('/logout')
def logout():
    session.pop('admin', None)
    return render_template('/homt')


#홈페이지
@app.route("/home")
def index():
    if 'user' in session:
        return render_template('index.html')
    else:
        index()



#비밀번호 분실
@app.route('/forgot_password')
def forgot_password():
    return render_template('/forgot_password.html')


#회원 등록
@app.route('/register')
def register():

    return render_template('/register.html')


#정부 DB 가져오기
@app.route("/GetGoverment")
def goverment():
     cursor = conn.cursor()
     cursor.execute("SELECT go.GO_License_Plate, car_status.car_status_now, model.model_car FROM go  INNER JOIN car_status ON go.GO_car_state = car_status.car_status_key INNER JOIN model ON go.GO_car_model = model.model_key;")
     r = [dict((cursor.description[i][0], value)
               for i, value in enumerate(row)) for row in cursor.fetchall()]
     return jsonify(r)

@app.route("/GetRecognize")
def GetRecognize():
     cursor = conn.cursor()
     cursor.execute("SELECT recognize.re_plate, location.location_now, model.model_car FROM recognize INNER JOIN location ON recognize.re_location = location.location_key INNER JOIN model ON recognize.re_model = model.model_key;")
     r = [dict((cursor.description[i][0], value)
               for i, value in enumerate(row)) for row in cursor.fetchall()]
     return jsonify(r)


@app.route("/Getillegal")
def getilleal():
     cursor = conn.cursor()
     cursor.execute("SELECT recognize.re_plate, recognize.re_time,  location.location_now, model.model_car FROM recognize LEFT JOIN go ON go.GO_License_Plate = recognize.re_plate INNER JOIN location ON recognize.re_location = location.location_key INNER JOIN model ON recognize.re_model = model.model_key;")
     r = [dict((cursor.description[i][0], value)
               for i, value in enumerate(row)) for row in cursor.fetchall()]
     return jsonify(r)


@app.route("/ChkStatus", methods=["POST"])
def ChkStatus():
    LP = str(request.form["license_plate"])

    cursor = conn.cursor()
    cursor.execute("SELECT recognize.re_plate, location.location_now, model.model_car, recognize.re_time FROM recognize INNER JOIN location ON recognize.re_location = location.location_key INNER JOIN model ON recognize.re_model = model.model_key where recognize.re_plate = '"+LP+"';")
    data = cursor.fetchall()
    if data:
        return jsonify(data)
    else:
        return "인식된 차량 없음."


@app.route("/login_CHK", methods=["POST"])
def CHK_login():
    email = str(request.form["Email"])
    PW = str(request.form["Password"])
    cursor = conn.cursor()
    cursor.execute("SELECT User_name FROM user WHERE Email='"+email+"'")
    EmailCHK = cursor.fetchall()
    #fetchall() 은 전부 가져오고 fetchone()은 한줄만 불러옴. 전부 가져와서 비교해야돼니깐..
    #대소 문자 구별이 안되게 로그인이 되고있음.. //charset="utf8"로 수정됨.
    if EmailCHK:
        final_chk= cursor.execute("SELECT User_name FROM user WHERE Email='"+email+"' AND PW='"+PW+"' ")
        if final_chk:
            session['user'] = final_chk
            return index()
        else:
            return "비밀번호가 틀렸습니다."
    else:
        return "로그인 다시입력해봐"


# DB Execl 다운로드
@app.route('/CSV')
def Excel():
    db = MySQLdb.connect(host="hostname",    # 호스트 명
                     user="id",         # 유저이름
                     passwd="password",  # 비밀번호
                     db="db")        # 데이스베이스 명

    cur = db.cursor()

# 추출할 데이터베이스 / where 문 사용하여 조건 / 대포차량, 노후경유차량 데이터 따로 저장 가능
    cur.execute("SELECT * FROM DB WHERE 조건 = 1")

    for row in cur.fetchall():
        print row[0]

    db.close()
    return


if __name__ == "__main__":
    app.run(host='127.0.0.1', debug=True, threaded=True)