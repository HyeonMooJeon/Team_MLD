		*Team_MLD Template*
@필수 pip 인스톨
 - pip인스톨 전 python 버젼을 꼭 확인해주세요
   => [window] -> cmd실행 -> python -V으로 확인후 3.7이 아닌경우 재설치 해주세요.
       https://www.python.org/downloads/release/python-370/
 - 설치 목록
 - pip사용 버젼 = 19.0.3
pip3 install pandas
pip3 install flask
pip3 install Flask-MySQLdb
pip3 install mysqlclient
pip3 install simplejson
pip3 install opencv-python

@파일 설명
1. static - HTML 에서 필요한 CSS, JS 파일들 및 필요한 img소스 모음
2. templates - HTML 소스코드
3. app.py - 내부 로직 코드
 - 회원 관리 부분
  -> login = 로그인 화면
  -> logout = 로그아웃 
  -> login_CHK = 로그인 확인 로직
  -> register = 회원 등록 화면
  -> register_user = 회원 등록 로직
  -> forgot_password = 비밀번호 분실

 - 동작 화면
  -> home = 홈페이지
  -> intro_main = 소개 화면
  -> Video_feed = 웹켐 프레임 캡쳐 로직
  -> real_time = Video_feed을 통한 캡쳐 화면 표시
  -> show_my_password = 비밀번호 보여주기
  -> recogINFO = 인식 정보 CSV파일로 출력
  -> chart = 데이터 차트를 통한 도식화 화면
  -> lookup = 기본 테이블 표시 화면
    -> carStatus = 차량 번호에 따른 결과 표시
    -> show_illegal = 범죄 차량 표시
    -> Show_recognize = 인식 차량 표시
    -> Show_Goverment = 정부 Table표시

@사용시 주의사항
 - app.py의 19번째 코드에서 사용자의 MySQL 서버의 사용자명, 비밀번호, DB이름까지 설정을 해주서야 합니다.
 - 카메라가 없는 경우 실시간 감식 화면(/real_time)에서 화면이 표시가 안됩니다.
 - 로그인(login), 로그아웃(logout), 비밀번호 분실(forgot_password), 회원등록(regist), 프로그램 소개(intro_main)화면에서는 로그인 없이 사용이 가능합니다.
   그외의 기능적인 화면은 로그인 후 사용이 가능합니다.
 - 회원가입시 유의사항
    = 비밀번호는 4자리 이상 사용해 주세요
    = 이메일은 중복사용 불가능 합니다.
    = 아무것도 입력하지 않을 경우 또한 등록 불가능 합니다.

@라이선스
 - 템플릿 라이선스 - MIT LICENSE (# [Start Bootstrap - SB Admin 2](https://startbootstrap.com/template-overviews/sb-admin-2/))
 - 프로젝트 라인선스 - GPL LICENSE
 - YOLO 라이선스 - YOLO LICENSE1.0