import csv
from csv import excel

import pandas as pd
from flask import Flask, render_template, request, Response, session, json, send_file
import MySQLdb
import cv2
from flask.json import jsonify
import json as j
from pandas.io.json import json_normalize
from flask_csv import send_csv
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


#여기 없는 페이지에 대한 에러처리
@app.errorhandler(404)
def page_error(error):
    return render_template("error.html"), 404


#json table 테스트용
@app.route("/test")
def test():
    import numpy as np

    cursor = conn.cursor()
    cursor.execute("SELECT recognize.re_plate, recognize.re_time,  location.location_now, model.model_car FROM recognize LEFT JOIN go ON go.GO_License_Plate = recognize.re_plate INNER JOIN location ON recognize.re_location = location.location_key INNER JOIN model ON recognize.re_model = model.model_key;")
    r = [dict((cursor.description[i][0], value)
              for i, value in enumerate(row))
         for row in cursor.fetchall()]


    df = pd.DataFrame(r)
    df = df.rename(columns={'re_plate': '차량 번호'})
    df = df.rename(columns={'location_now': '위치'})
    df = df.rename(columns={'model_car': '차량 모델'})
    df = df.rename(columns={'re_time': '인식된 시간'})
    


    return df.to_html()










#비디오 동영상 HTML
@app.route("/video")
def streaming():
    if 'user' in session:
        return render_template('video.html')
    else:
        return login()



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
        return login()



@app.route("/intro_not_regist")
def intro_not_regist():
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
        return render_template('login.html')



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
    return login()


#홈페이지
@app.route("/home")
def index():
    if session.get('user'):
        return render_template('index.html')
    else:
        return login()



#비밀번호 분실
@app.route('/forgot_password')
def forgot_password():
    return render_template('/forgot_password.html')


@app.route('/show_my_password',  methods=["POST"])
def show_my_password():
    email = str(request.form["Email"])
    name = str(request.form["real_name"])
    if len(email) == 0 or len(name) == 0:
        return "형식을 다 입력해주세요."
    cursor = conn.cursor()
    cursor.execute("SELECT PW FROM user where Email='"+email+"' AND User_name='"+name+"'")
    password = cursor.fetchall()
    if password:
        return jsonify(password)
    else:
        return "존재하지 않는 이메일입니다."



#회원 등록
@app.route('/register')
def register():
    return render_template('/register.html')


@app.route("/register_user", methods=["POST"])
def register_user():
    email = str(request.form["Email"])
    name = str(request.form["user_name"])
    PW = str(request.form["Password"])
    CHK_PW = str(request.form["Password_CHK"])
    if PW == CHK_PW:
        cursor = conn.cursor()
        cursor.execute("select User_name from user where Email='"+email+"'")
        CHK = cursor.fetchall()
        if CHK:
            return "이미 이메일이 존재합니다."
        else:
            cursor.execute("INSERT INTO user(Email,PW,User_name) VALUE('"+email+"', '"+PW+"', '"+name+"')")
            conn.commit()
            conn.close()
            return "회원가입 완료"
    else:
        return "비밀번호와 재입력 비밀번호가 다릅니다."



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
    if len(email) == 0 or len(PW) == 0:
        return "형식을 다 입력해주세요."

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

#정부 파일 저장하는 로직
@app.route("/GovINFO")
def testTable():
    cursor = conn.cursor()
    try:
        cursor.execute("SELECT go.GO_License_Plate, car_status.car_status_now, model.model_car "
                        "FROM go "
                        "INNER JOIN car_status ON go.GO_car_state = car_status.car_status_key "
                        "INNER JOIN model ON go.GO_car_model = model.model_key "
                        "into outfile 'C:/Users/Public/Goverment_Table.csv.csv' fields terminated by ',' ;")
    except (MySQLdb.Error, MySQLdb.Warning) as e:
        print(e)
    finally:
        conn.close()
        return index()
    #try catch를 통해 파일이 존재할경우 메인 화면으로 이동하도록 만듬



if __name__ == "__main__":
    app.run(debug=True, threaded=True)