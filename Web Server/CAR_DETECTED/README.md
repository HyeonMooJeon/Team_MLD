		*Team_MLD Template*
@�ʼ� pip �ν���
 - pip�ν��� �� python ������ �� Ȯ�����ּ���
   => [window] -> cmd���� -> python -V���� Ȯ���� 3.7�� �ƴѰ�� �缳ġ ���ּ���.
       https://www.python.org/downloads/release/python-370/
 - ��ġ ���
 - pip��� ���� = 19.0.3
pip3 install pandas
pip3 install flask
pip3 install Flask-MySQLdb
pip3 install mysqlclient
pip3 install simplejson
pip3 install opencv-python

@���� ����
1. static - HTML ���� �ʿ��� CSS, JS ���ϵ� �� �ʿ��� img�ҽ� ����
2. templates - HTML �ҽ��ڵ�
3. app.py - ���� ���� �ڵ�
 - ȸ�� ���� �κ�
  -> login = �α��� ȭ��
  -> logout = �α׾ƿ� 
  -> login_CHK = �α��� Ȯ�� ����
  -> register = ȸ�� ��� ȭ��
  -> register_user = ȸ�� ��� ����
  -> forgot_password = ��й�ȣ �н�

 - ���� ȭ��
  -> home = Ȩ������
  -> intro_main = �Ұ� ȭ��
  -> Video_feed = ���� ������ ĸ�� ����
  -> real_time = Video_feed�� ���� ĸ�� ȭ�� ǥ��
  -> show_my_password = ��й�ȣ �����ֱ�
  -> recogINFO = �ν� ���� CSV���Ϸ� ���
  -> chart = ������ ��Ʈ�� ���� ����ȭ ȭ��
  -> lookup = �⺻ ���̺� ǥ�� ȭ��
    -> carStatus = ���� ��ȣ�� ���� ��� ǥ��
    -> show_illegal = ���� ���� ǥ��
    -> Show_recognize = �ν� ���� ǥ��
    -> Show_Goverment = ���� Tableǥ��

@���� ���ǻ���
 - app.py�� 19��° �ڵ忡�� ������� MySQL ������ ����ڸ�, ��й�ȣ, DB�̸����� ������ ���ּ��� �մϴ�.
 - ī�޶� ���� ��� �ǽð� ���� ȭ��(/real_time)���� ȭ���� ǥ�ð� �ȵ˴ϴ�.
 - �α���(login), �α׾ƿ�(logout), ��й�ȣ �н�(forgot_password), ȸ�����(regist), ���α׷� �Ұ�(intro_main)ȭ�鿡���� �α��� ���� ����� �����մϴ�.
   �׿��� ������� ȭ���� �α��� �� ����� �����մϴ�.
 - ȸ�����Խ� ���ǻ���
    = ��й�ȣ�� 4�ڸ� �̻� ����� �ּ���
    = �̸����� �ߺ���� �Ұ��� �մϴ�.
    = �ƹ��͵� �Է����� ���� ��� ���� ��� �Ұ��� �մϴ�.

@���̼���
 - ���ø� ���̼��� - MIT LICENSE (# [Start Bootstrap - SB Admin 2](https://startbootstrap.com/template-overviews/sb-admin-2/))
 - ������Ʈ ���μ��� - GPL LICENSE
 - YOLO ���̼��� - YOLO LICENSE1.0