1.컴퓨터 구성 / 필수 조건 안내 (Prerequisites)
	SW장치
	1. Darknet Frame work
	2. Window 10 환경
	3. Python 3.6V
	4. YOLO V3
	5. Flask Frame work
	6. nVIDIA CUDA
	7. MySQL

	HW장치
	1. GTX 1060 ti 6G 이상의 GPU
	2. 영상을 촬영할 장치 2개 이상

2.설치 안내 (Installation Process)
	1. Darknet builld (Open CV 경로 설정)
	2. Python을 통한 웹 서비스 실행
	3. ngrok 실행 (ngrok http 5000)
	3. YOLO V3 데모 실행(darknet.exe detector mld_last data/carnumber2/obj_totalnumber.data data/carnumber2/yolo-obj_totalnumber.cfg data/carnumber2/yolo-obj_totalnumber_last.weights -c 0 "http://192.168.0.46:39895/videostream.cgi?user=admin&pwd=12345678" data/carnumber2/obj-new16.data data/carnumber2/yolo-obj.cfg data/carnumber2/yolo-obj_91000.weights)
	4. 카메라 차량 진입
	
3.사용법 (Getting Started)
	1. Darknet builld (Open CV 경로 설정)
	2. Python을 통한 웹 서비스 실행
	3. ngrok 실행 (ngrok http 5000)
	3. YOLO V3 데모 실행(darknet.exe detector mld_last data/carnumber2/obj_totalnumber.data data/carnumber2/yolo-obj_totalnumber.cfg data/carnumber2/yolo-obj_totalnumber_last.weights -c 0 "http://192.168.0.46:39895/videostream.cgi?user=admin&pwd=12345678" data/carnumber2/obj-new16.data data/carnumber2/yolo-obj.cfg data/carnumber2/yolo-obj_91000.weights)
	4. 카메라 차량 진입

	1. 차량 카메라를 향해 접근
	2. 카메라 1 차종 인식
	3. 카메라 2 차번 인식
	4. 카메라 2 화면에서 차번 사라짐
	5. 카메라 1 화면에서 차량 사라짐
	6. MySQL에 데이터 저장

4.파일 정보 및 목록 (File Manifest)
	1. carnumberDataset - 차량, 차종, 차번호, 한글 데이터 셋 모음
	2. darknet - yolo를 동작시키기 위한 darknet framework (카메라 모듈1, 2 수정 demo.c 안에 있음)
	3. DB 샘플 (테스팅 샘플 DB)
	4. filesearch&Change&inChange(웹 크롤링을 위한 탐색 파일과, 라벨링한 데이터에 대한 설정 파일 내용 변경을 위한 파이썬 코드)
	5. openc_Tesseract (한글 인식과 LP를 인식하기 위해 테스트로 사용되었던 OPENCV만을 이용한 차량 번호 인식 방식)
	6. Web Server (Flask웹 서버)
	7. 가중치 데이터 파일 (데이터 셋 파일)

5.저작권 및 사용권 정보 (Copyright / End User License)
GPL LICENSE

6.배포자 및 개발자의 연락처 정보 (Contact Information)
배포자: 전현무 
개발자: 전현무(server5619@gmail.com), 이길우(dlrlfdn@gmail.com), 정병철(billy8309@gmail.com)

7.알려진 버그 (Known Issues)
 - 


8.문제 발생에 대한 해결책 (Troubleshooting)
 6번 사항의 개발자 메일로 문의를 넣어주시기 바랍니다.

9.크레딧 (Credit)

10.업데이트 정보 (Change Log)
2019-06-10 beta update NEW release