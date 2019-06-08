import pandas as pd
from flask import Flask, render_template, request, Response, session, get_flashed_messages, redirect, url_for, flash
import MySQLdb
import cv2
from flask.json import jsonify
import json
from django.http import HttpResponse, JsonResponse
import time
from threading import Thread
import threading
from django.shortcuts import render

User_Name=""
app = Flask(__name__)
app.secret_key = "super secret key"
vc = cv2.VideoCapture(0)
#DB로부터 데이터를 받을때 ASCII코드로 바뀌는걸 방지.
app.config['JSON_AS_ASCII'] = False
conn = MySQLdb.connect(host="localhost", user="root", password="sm435", db="team_mld", charset='utf8')
cursor = conn.cursor()
newRecog=""
myThread = threading.Thread()


#카메라 동작 방법.
def gen():
    while True:
        rval, frame = vc.read()
        cv2.imwrite('t.jpg', frame)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + open('t.jpg', 'rb').read() + b'\r\n')

#새로운 데이터 확인하기. recognize 테이블에 새로운 차번호 들어오면 인식.
#5초마다 확인하며, 간단하게 데이터 2개를 처음에 같은값 그후 비교하면서 다르면 새로운 차량 인식된걸로 됨.
def refresh():
    cursor.execute("SELECT re_plate FROM recognize where re_index=(select MAX(re_index) from recognize);")
    r = [dict((cursor.description[i][0], value)
              for i, value in enumerate(row))
         for row in cursor.fetchall()]
    conn.commit()
    data1 = json.dumps(r)
    print("처음 차번 = "+data1)
    while True:
        time.sleep(1)
        cursor.execute("SELECT re_plate FROM recognize where re_index=(select MAX(re_index) from recognize);")
        r = [dict((cursor.description[i][0], value)
             for i, value in enumerate(row))
                 for row in cursor.fetchall()]
        conn.commit()
        data2 = json.dumps(r)
        if(data1 != data2):
             send = data2.split("\"")
             print("추가된 차번 = "+send[3])
             global newRecog
             newRecog = send[3]
             print(send[3])
             data1=data2



#홈페이지
@app.route("/")
@app.route("/home")
def index():
    #global myThread
    #myThread = Thread(target=refresh())
    #myThread.start()

    if session.get('user'):
        #return render_template('index.html' ,name = User_Name)
        return render_template("index.html", name="테스터", newCar="12다3456")
    else:
        return login()



#여기 없는 페이지에 대한 에러처리
@app.errorhandler(404)
def page_error(error):
    return render_template('error.html', err_code="FAIL", err_message1="페이지를 찾을수 없습니다."), 404

#table형식으로 표시하는법.
@app.route("/Show_recognize")
def show_Recognize():
    cursor.execute("SELECT recognize.re_plate, recognize.re_time,  location.location_now, model.model_car, car_status.car_status_now  FROM recognize  LEFT JOIN go ON go.GO_License_Plate = recognize.re_plate  INNER JOIN location ON recognize.re_location = location.location_key  INNER JOIN model ON recognize.re_model = model.model_key INNER JOIN car_status ON car_status.car_status_key = recognize.re_status;")
    r = [dict((cursor.description[i][0], value)
              for i, value in enumerate(row))
         for row in cursor.fetchall()]
    conn.commit()
    df = pd.DataFrame(r)
    df.columns.name = '인덱스'
    df = df.rename(columns={'re_plate': '차량 번호'})
    df = df.rename(columns={'location_now': '위치'})
    df = df.rename(columns={'model_car': '차량 모델'})
    df = df.rename(columns={'re_time': '인식된 시간'})
    df = df.rename(columns={'car_status_now': '범죄 유형'})

    cursor.execute("SELECT file_model_location FROM recognize;")
    model=cursor.fetchall()

    cursor.execute("SELECT file_location FROM recognize;")
    plate=cursor.fetchall()


    return render_template("lookup.html", tables=[df.to_html(classes='data')] , rows1=model, rows2=plate , r1="차종", r2="번호판")



#회원 등록
@app.route('/test')
def test():
    cursor.execute("SELECT file_model_location FROM recognize;")
    row=cursor.fetchall()

    return render_template("test.html", rows=row)







@app.route("/Show_Goverment")
def show_goverment():
     cursor.execute("SELECT go.GO_License_Plate, car_status.car_status_now, model.model_car FROM go  INNER JOIN car_status ON go.GO_car_state = car_status.car_status_key INNER JOIN model ON go.GO_car_model = model.model_key;")
     r = [dict((cursor.description[i][0], value)
               for i, value in enumerate(row))
                   for row in cursor.fetchall()]
     conn.commit()

     df = pd.DataFrame(r)
     df.columns.name = '인덱스'

     df = df.rename(columns={'GO_License_Plate': '차량 번호'})
     df = df.rename(columns={'car_status_now': '범죄 유형'})
     df = df.rename(columns={'model_car': '차량 모델'})


     return render_template("lookup.html", tables=[df.to_html(classes='data')], titles="정부 데이터베이스 확인하기")



@app.route("/show_illegal", methods=("POST", "GET"))
def show_illegal():
     cursor.execute("SELECT recognize.re_plate, recognize.re_time,  location.location_now, model.model_car, car_status.car_status_now FROM recognize LEFT JOIN go ON go.GO_License_Plate = recognize.re_plate INNER JOIN location ON recognize.re_location = location.location_key INNER JOIN model ON recognize.re_model = model.model_key INNER JOIN car_status ON recognize.re_status = car_status_key where go.GO_License_Plate = recognize.re_plate;")
     r = [dict((cursor.description[i][0], value)
               for i, value in enumerate(row)) for row in cursor.fetchall()]
     conn.commit()

     df = pd.DataFrame(r)
     df.columns.name = '인덱스'

     df = df.rename(columns={'location_now':'위치'})
     df = df.rename(columns={'model_car':'차종'})
     df = df.rename(columns={'re_plate':'차량번호'})
     df = df.rename(columns={'re_time':'인식 시간'})
     df = df.rename(columns={'car_status_now':'범죄 유형'})


     return render_template("lookup.html", tables=[df.to_html(classes='data')], titles="범죄차량 확인하기")

#비디오 동영상 동작
@app.route('/video_feed')
def video_feed():
    return Response(gen(), mimetype='multipart/x-mixed-replace; boundary=frame')


#시작화면
@app.route("/")
@app.route('/login')
def login():
    return render_template('/login.html')


#차량 검색
@app.route("/lookup")
def lookup():
    if 'user' in session:
        return render_template("lookup.html" ,name = User_Name)
    else:
        return render_template('login.html')



#실시간 영상
@app.route("/real_time")
def real_time():
    if 'user' in session:
        return render_template('real_time.html' , name = User_Name)
    else:
        index()




#로그아웃
@app.route('/logout')
def logout():
    session.pop('user', None)
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
        return render_template('error.html', err_code="FAIL", err_message1="형식을 마저 입력해주세요.", err_message2="다시 확인해주세요")
    cursor.execute("SELECT PW FROM user where Email='"+email+"' AND User_name='"+name+"'")
    password = cursor.fetchall()
    if password:
        return jsonify(password)
    else:
        return render_template('error.html', err_code="FAIL", err_message1="존재하지 않는 이메일입니다", err_message2="다시 확인해주세요")



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
    if len(email) == 0 or len(PW) == 0 or len(name) == 0 or len(CHK_PW) == 0:
        return render_template('error.html', err_code="FAIL", err_message1="위의 형식이 비어있습니다.", err_message2="형식을 다 입력해주세요.")

    if len(PW) < 4:
        return render_template('error.html', err_code="FAIL", err_message1="비밀번호를 재설정 해주세요.", err_message2="최소 4글자 이상으로 해주세요.")

    if PW == CHK_PW:
        cursor.execute("select User_name from user where Email='"+email+"'")
        CHK = cursor.fetchall()
        if CHK:
            return render_template('error.html', err_code="FAIL", err_message1="해당 이메일이 존재합니다.", err_message2="다른 이메일을 입력해주세요.")
        else:
            cursor.execute("INSERT INTO user(Email,PW,User_name) VALUE('"+email+"', '"+PW+"', '"+name+"')")
            conn.commit()
            #회원가입 완료는 alert사용해서 다시 완성할것.
            return "회원가입 완료"
    else:
        return render_template('error.html', err_code="FAIL", err_message1="비밀번호가 서로 다릅니다.",err_message2="다른 확인해주세요.")


#차량조회
@app.route("/carStatus", methods=["POST"])
def ChkStatus():
    LP = str(request.form["LP"])
    #print(LP)

    cursor.execute("SELECT recognize.re_plate, location.location_now, model.model_car, recognize.re_time FROM recognize INNER JOIN location ON recognize.re_location = location.location_key INNER JOIN model ON recognize.re_model = model.model_key where recognize.re_plate = '"+LP+"';")
    r = [dict((cursor.description[i][0], value)
              for i, value in enumerate(row)) for row in cursor.fetchall()]
    conn.commit()
    #print(r)
    if len(r) == 0:
        return render_template('error.html', err_code="FAIL", err_message1="데이터 형식이 비어있습니다..",err_message2="차번을 입력해주세요.")

    if r:
        df = pd.DataFrame(r)
        #print("test")
        df.columns.name = '인덱스'

        df = df.rename(columns={'re_plate': '차량 번호'})
        df = df.rename(columns={'location_now': '위치'})
        df = df.rename(columns={'model_car': '차량 모델'})
        df = df.rename(columns={'re_time': '인식된 시간'})

        cursor.execute("SELECT file_model_location FROM recognize where re_plate='"+LP+"';")
        model = cursor.fetchall()

        cursor.execute("SELECT file_location FROM recognize where re_plate='"+LP+"';")
        plate = cursor.fetchall()

        return render_template("lookup.html", tables=[df.to_html(classes='data')], rows1=model, rows2=plate, r1="차종", r2="번호판")


    else:
        return render_template('error.html', err_code="FAIL", err_message1="인식된 차량이 없어요.",err_message2="기다려주세요.")




@app.route("/login_CHK", methods=["POST"])
def CHK_login():
    email = str(request.form["Email"])
    PW = str(request.form["Password"])
    if len(email) == 0 or len(PW) == 0:
        return render_template('error.html', err_code="FAIL", err_message1="형식이 비어있습니다.", err_message2="형식을 다 입력해주세요.")

    cursor.execute("SELECT User_name FROM user WHERE Email='"+email+"'")
    EmailCHK = cursor.fetchall()
    #fetchall() 은 전부 가져오고 fetchone()은 한줄만 불러옴. 전부 가져와서 비교해야돼니깐..
    #대소 문자 구별이 안되게 로그인이 되고있음.. //charset="utf8"로 수정됨.
    if EmailCHK:
        cursor.execute("SELECT User_name FROM user WHERE Email='"+email+"' AND PW='"+PW+"' ")
        final_chk = [dict((cursor.description[i][0], value)
                  for i, value in enumerate(row)) for row in cursor.fetchall()]
        split_data = json.dumps(final_chk, ensure_ascii=False)

        #사용자 이름 알아내기 session['user']이걸로는 숫자 1만 나옴....큰따음표 사이로 split으로 나누어서 데이터 뽑아냄
        if final_chk:
            session['user'] = final_chk
            #print(split_data)
            data = split_data.split("\"")
            print(data[3])
            global User_Name
            global newRecog
            User_Name = data[3]
            return render_template("index.html", name = User_Name, newCar = newRecog)
        else:
            return render_template('error.html', err_code="FAIL", err_message1="비밀번호가 틀렸습니다..",err_message2="다시 입력해 주세요.")
    else:
            return render_template('error.html', err_code="FAIL", err_message1="비밀번호나 이메일이 틀렸습니다.",err_message2="다시 입력해 주세요.")


#정부 파일 저장하는 로직
@app.route("/GovINFO")
def GovTable():
    try:
        cursor.execute("SELECT go.GO_License_Plate, car_status.car_status_now, model.model_car "
                        "FROM go "
                        "INNER JOIN car_status ON go.GO_car_state = car_status.car_status_key "
                        "INNER JOIN model ON go.GO_car_model = model.model_key "
                        "into outfile 'C:/Users/Public/Goverment_Table.csv' fields terminated by ',' ;")
        conn.commit()

    except (MySQLdb.Error, MySQLdb.Warning) as e:
        print(e)
    finally:
        return index()
    #try catch를 통해 파일이 존재할경우 메인 화면으로 이동하도록 만듬

#위치는 추후 수정해야함.
@app.route('/data', methods=["GET", "POST"])
def data():
    cursor.execute(
        "select count(case when re_location=1 then 1 end) as '1번 위치',count(case when re_location=2 then 1 end) as '2번 위치',count(case when re_location=3 then 1 end) as '3번 위치'from recognize;")
    r = [dict((cursor.description[i][0], value)
              for i, value in enumerate(row)) for row in cursor.fetchall()]
    jsonData = jsonify(r)
    conn.commit()

    return jsonData

@app.route('/intro_main')
def intro_main():
    return render_template('intro_main.html')


@app.route('/chart', methods=["GET", "POST"])
def chart():
    cursor.execute(
        "select count(case when re_location=1 then 1 end) as 'location1',count(case when re_location=2 then 1 end) as 'location2',count(case when re_location=3 then 1 end) as 'location3',count(case when re_location=4 then 1 end) as 'location4' from recognize where re_time >= DATE_SUB(NOW(), INTERVAL 7 DAY);")
    r = [dict((cursor.description[i][0], value)
              for i, value in enumerate(row)) for row in cursor.fetchall()]
    json_data=json.dumps(r, ensure_ascii=False)
    #print(json_data)
    location1 = 0
    location2 = 0
    location3 = 0
    location4 = 0


    split_data = json_data.split(": ")
    i=0
    for temp in split_data:
        if i==0:
            i=i+1
            continue
        recognize = 0
        for k in temp:
            #print(k)
            if CHKnumber(k):
                if recognize != 0:
                    recognize = recognize*10
                recognize=recognize + int(k)
            else:
                break
        if i==1:
            location1 = recognize
        if i==2:
            location2 = recognize
        if i==3:
            location3 = recognize
        if i==4:
            location4 = recognize


        #print(recognize,"그리고",i)
        i=i+1


    return render_template('charts.html', loc1 = location1, loc2 = location2, loc3 = location3, loc4 = location4, name= User_Name)

def CHKnumber(i):
    try:
        float(i)
        return True
    except ValueError:
        return False






if __name__ == "__main__":
    app.run(debug=True, threaded=True)
