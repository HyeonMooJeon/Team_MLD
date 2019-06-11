Darknet Frame work
equirements
Windows or Linux
MySQL >= 8.0 : https://dev.mysql.com/downloads/mysql/
CMake >= 3.8 for modern CUDA support: https://cmake.org/download/
CUDA 10.0: https://developer.nvidia.com/cuda-toolkit-archive (on Linux do Post-installation Actions)
OpenCV >= 2.4: use your preferred package manager (brew, apt), build from source using vcpkg or download from OpenCV official site (on Windows set system variable OpenCV_DIR = C:\opencv\build - where are the include and x64 folders image)
uDNN >= 7.0 for CUDA 10.0 https://developer.nvidia.com/rdp/cudnn-archive (on Linux copy cudnn.h,libcudnn.so... as desribed here https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html#installlinux-tar , on Windows copy cudnn.h,cudnn64_7.dll, cudnn64_7.lib as desribed here https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html#installwindows )
GPU with CC >= 3.0: https://en.wikipedia.org/wiki/CUDA#GPUs_supported
on Linux GCC or Clang, on Windows MSVC 2015/2017/2019 https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community
Compiling on Windows by using Cmake-GUI as on this IMAGE: Configure -> Optional platform for generator (Set: x64) -> Finish -> Generate -> Open Project -> x64 & Release -> Build -> Build solution

Compiling on Linux by using command make (or alternative way by using command: cmake . && make )

How to compile on Windows (legacy way)
1. If you have CUDA 10.0, cuDNN 7.4, MYSQL 8.0(with paths: C:\Program Files\MySQL\MySQL Server 8.0\include & C:\Program Files\MySQL\MySQL Server 8.0\lib) and OpenCV 3.x (with paths: C:\opencv\build\include & C:\opencv\build\x64\vc15\lib), then open darknet\src\test.c, set MYSQLUSER , MYSQLPW and MYSQLIP as user, pw, ip.
 1.1 Open build\darknet\darknet.sln, set x64 and Release https://hsto.org/webt/uh/fk/-e/uhfk-eb0q-hwd9hsxhrikbokd6u.jpeg and do the: Build -> Build darknet. Also add Windows system variable CUDNN with path to CUDNN: https://user-images.githubusercontent.com/4096485/53249764-019ef880-36ca-11e9-8ffe-d9cf47e7e462.jpg
 1.2 If you have other version of MYSQL (not 8.0) then open darknet\build\darknet.sln 
  1.2.1 (right click on project) -> properties -> VC+ Directories-> Include Directories : C:\Program Files\MySQL\MySQL Server Version\include
  1.2.2 (right click on project) -> properties -> VC+ Directories-> Library Directories : C:\Program Files\MySQL\MySQL Server Version\lib
  1.2.3 copy file libeay32.dll , ssleay32.dll , libmysql.dll to the folder \build\darknet\x64 near with darknet.exe

 1.3 Check that there are bin and include folders in the C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.0 if aren't, then copy them to this folder from the path where is CUDA installed
 1.4. To install CUDNN (speedup neural network), do the following: 
  1.4.1 download and install cuDNN v7.4.1 for CUDA 10.0: https://developer.nvidia.com/rdp/cudnn-archive
  1.4.2 add Windows system variable CUDNN with path to CUDNN: https://user-images.githubusercontent.com/4096485/53249764-019ef880-36ca-11e9-8ffe-d9cf47e7e462.jpg
  1.4.3 copy file cudnn64_7.dll to the folder \build\darknet\x64 near with darknet.exe

 1.5. If you want to build without CUDNN then: open \darknet.sln -> (right click on project) -> properties -> C/C++ -> Preprocessor -> Preprocessor Definitions, and remove this: CUDNN;

2. If you have other version of CUDA (not 10.0) then open build\darknet\darknet.vcxproj by using Notepad, find 2 places with "CUDA 10.0" and change it to your CUDA-version. Then open \darknet.sln -> (right click on project) -> properties -> CUDA C/C++ -> Device and remove there ;compute_75,sm_75. Then do step 1

3. If you don't have GPU, but have OpenCV 3.0 (with paths: C:\opencv_3.0\opencv\build\include & C:\opencv_3.0\opencv\build\x64\vc14\lib), then open build\darknet\darknet_no_gpu.sln, set x64 and Release, and do the: Build -> Build darknet_no_gpu

4. If you have OpenCV 2.4.13 instead of 3.0 then you should change paths after \darknet.sln is opened

 4.1 (right click on project) -> properties -> C/C++ -> General -> Additional Include Directories: C:\opencv_2.4.13\opencv\build\include

 4.2 (right click on project) -> properties -> Linker -> General -> Additional Library Directories: C:\opencv_2.4.13\opencv\build\x64\vc14\lib

5. If you have GPU with Tensor Cores (nVidia Titan V / Tesla V100 / DGX-2 and later) speedup Detection 3x, Training 2x: \darknet.sln -> (right click on project) -> properties -> C/C++ -> Preprocessor -> Preprocessor Definitions, and add here: CUDNN_HALF;

Note: CUDA must be installed only after Visual Studio has been installed.

Source code
1.darknet.c
 -> darknet main function, set gpu option, 실행 종류에 맞는 함수 호출("train","detector","yolo",...) 및 매개변수 전달
2.detector.c
 -> 실행 옵션 설정 및 data파일,cfg파일,weights파일,name파일 파씽, 옵션에 따른 함수 호출("test","demo"...)
3.demo.c
 -> weight파일에 맞게 네트워크 구성, label 파일 로드, 윈도우 창 출력, Frame Thread , Detection Thread, Draw Bounding Box Thread, Make Detection List
4.image.c
 -> Frame Capture, Resize Image, Draw Bounding Box, Save Image, Show Image
5.test.c
 -> 각종 DB 로직, Division search algorithm, 

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