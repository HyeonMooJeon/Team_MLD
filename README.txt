1.��ǻ�� ���� / �ʼ� ���� �ȳ� (Prerequisites)
	SW��ġ
	1. Darknet Frame work
	2. Window 10 ȯ��
	3. Python 3.6V
	4. YOLO V3
	5. Flask Frame work
	6. nVIDIA CUDA
	7. MySQL

	HW��ġ
	1. GTX 1060 ti 6G �̻��� GPU
	2. ������ �Կ��� ��ġ 2�� �̻�

2.��ġ �ȳ� (Installation Process)
	1. Darknet builld (Open CV ��� ����)
	2. Python�� ���� �� ���� ����
	3. ngrok ���� (ngrok http 5000)
	3. YOLO V3 ���� ����(darknet.exe detector mld_last data/carnumber2/obj_totalnumber.data data/carnumber2/yolo-obj_totalnumber.cfg data/carnumber2/yolo-obj_totalnumber_last.weights -c 0 "http://192.168.0.46:39895/videostream.cgi?user=admin&pwd=12345678" data/carnumber2/obj-new16.data data/carnumber2/yolo-obj.cfg data/carnumber2/yolo-obj_91000.weights)
	4. ī�޶� ���� ����
	
3.���� (Getting Started)
	1. Darknet builld (Open CV ��� ����)
	2. Python�� ���� �� ���� ����
	3. ngrok ���� (ngrok http 5000)
	3. YOLO V3 ���� ����(darknet.exe detector mld_last data/carnumber2/obj_totalnumber.data data/carnumber2/yolo-obj_totalnumber.cfg data/carnumber2/yolo-obj_totalnumber_last.weights -c 0 "http://192.168.0.46:39895/videostream.cgi?user=admin&pwd=12345678" data/carnumber2/obj-new16.data data/carnumber2/yolo-obj.cfg data/carnumber2/yolo-obj_91000.weights)
	4. ī�޶� ���� ����

	1. ���� ī�޶� ���� ����
	2. ī�޶� 1 ���� �ν�
	3. ī�޶� 2 ���� �ν�
	4. ī�޶� 2 ȭ�鿡�� ���� �����
	5. ī�޶� 1 ȭ�鿡�� ���� �����
	6. MySQL�� ������ ����

4.���� ���� �� ��� (File Manifest)
	1. carnumberDataset - ����, ����, ����ȣ, �ѱ� ������ �� ����
	2. darknet - yolo�� ���۽�Ű�� ���� darknet framework (ī�޶� ���1, 2 ���� demo.c �ȿ� ����)
	3. DB ���� (�׽��� ���� DB)
	4. filesearch&Change&inChange(�� ũ�Ѹ��� ���� Ž�� ���ϰ�, �󺧸��� �����Ϳ� ���� ���� ���� ���� ������ ���� ���̽� �ڵ�)
	5. openc_Tesseract (�ѱ� �νİ� LP�� �ν��ϱ� ���� �׽�Ʈ�� ���Ǿ��� OPENCV���� �̿��� ���� ��ȣ �ν� ���)
	6. Web Server (Flask�� ����)
	7. ����ġ ������ ���� (������ �� ����)

5.���۱� �� ���� ���� (Copyright / End User License)
GPL LICENSE

6.������ �� �������� ����ó ���� (Contact Information)
������: ������ 
������: ������(server5619@gmail.com), �̱��(dlrlfdn@gmail.com), ����ö(billy8309@gmail.com)

7.�˷��� ���� (Known Issues)
 - 


8.���� �߻��� ���� �ذ�å (Troubleshooting)
 6�� ������ ������ ���Ϸ� ���Ǹ� �־��ֽñ� �ٶ��ϴ�.

9.ũ���� (Credit)

10.������Ʈ ���� (Change Log)
2019-06-10 beta update NEW release