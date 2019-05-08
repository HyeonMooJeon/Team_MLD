import cv2
import imutils
import numpy as np
import PIL
import pytesseract

#가우시안 필터 적용 및 숫자 검출률을 높인 버전

img=cv2.imread("testimg3.jpg",cv2.IMREAD_COLOR)
copy_img=img.copy()  #just image Read
cv2.imshow('src', copy_img )

img2=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY) #그레이 적용
cv2.imshow('gray_img2', img2 )

blur = cv2.GaussianBlur(img2,(3,3),0) #가우시안 필터 블루
cv2.imwrite('blur.jpg',blur)
cv2.imshow('blur', blur )

canny = cv2.Canny(blur,100,200) #엣지 검출
cv2.imwrite('canny.jpg',canny)
cv2.imshow('canny', canny ) 

'''
cv2.Canny에 적용되는 Threshold1,2 의 값은 특정 사진에 맞춰 설정해준 값입니다. 이외에도 가우시안 값에 적용시킨 3x3 매트릭스 값도 마찬 가지입니다.  
Edge에 대한 설명을 조금 추가하자면, 흑백영상에서 명암의 밝기 차이에 대한 변화율이라고 생각하시면 됩니다.
 흑백이미지에 대한 미분 값
'''

#같은 에너지를 가지는 점들을 연결한 선 (번호판) 검은 바탕에 찾는 물체는 흰색 그래서 그레이로 변환

contours, hierarchy = cv2.findContours(canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = cv2.drawContours(canny, contours, -1, 3)

#전체 이미지에서 Contour의 가로 세로 비율 값과 면적을 통해, 
#번호판 영역에 벗어난 걸로 추정되는 값들은 제외 시켜주어야함
box1=[]
f_count=0
select=0
plate_width=0

for i in range(len(contours)):
    cnt=contours[i]
    area = cv2.contourArea(cnt)
    x,y,w,h = cv2.boundingRect(cnt)
    rect_area=w*h #area size
    aspect_ratio = float(w)/h #ratio = width/height

    if(aspect_ratio>0.2)and(aspect_ratio<=1.0)and(rect_area>=100)and(rect_area<=700):
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),1)
        box1.append(cv2.boundingRect(cnt))

cv2.imshow('contour', img ) #콘투어 다 찾기

###절취선 이미지라이트 때문에 그런듯 콘투어는 다 찾아짐
for i in range(len(box1)): ##버블정렬을 이용 
    for j in range(len(box1)-(i+1)):
        if box1[j][0]>box1[j+1][0]:
            temp=box1[j]
            box1[j]=box1[j+1]
            box1[j+1]=temp

for m in range(len(box1)):
    count=0
    for n in range(m+1, (len(box1)-1)):
        delta_x=abs(box1[n+1][0]-box1[m][0])
        if delta_x >150:
            break
        delta_y=abs(box1[n+1][1]-box1[m][1])
        if delta_x ==0:
            delta_x=1
        if delta_y ==0:
            delta_y=1
        gradient=float(delta_y)/float(delta_x)
        if gradient<0.25:
            count=count+1
        #measure number plate size
    if count > f_count:
        select = m
        f_Count = count
        plate_width=delta_x
    cv2.imwrite('snake.jpg',img)

number_plate=copy_img[box1[select][1]-10:box1[select][3]+box1[select][1]+20,box1[select][0]-10:140+box1[select][0]] 

resize_plate=cv2.resize(number_plate,None,fx=1.8,fy=1.8,interpolation=cv2.INTER_CUBIC+cv2.INTER_LINEAR) 
plate_gray=cv2.cvtColor(resize_plate,cv2.COLOR_BGR2GRAY)
ret,th_plate = cv2.threshold(plate_gray,150,255,cv2.THRESH_BINARY)
          
cv2.imwrite('plate_th.jpg',th_plate)
kernel = np.ones((3,3),np.uint8)
er_plate = cv2.erode(th_plate,kernel,iterations=1)
er_invplate = er_plate
cv2.imwrite('er_plate.jpg',er_invplate)
result = pytesseract.image_to_string(Image.open('er_plate.jpg'), lang='kor')
result.replace(" ","")

recogtest=Recognition()
result=recogtest.ExtractNumber()
print(result)
###절취선 이미지라이트 때문에 그런듯
cv2.waitKey(0)
cv2.destroyAllWindows()