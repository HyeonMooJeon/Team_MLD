import cv2
import imutils
import numpy as np
import PIL
import pytesseract

#일반버전 번호검출

#src = cv2.imread("C:\\Users\\person\\Desktop\\Raspberry-Pi-License-Plate-Reader.jpg", cv2.IMREAD_COLOR)
src = cv2.imread("C:\\Users\\person\\Desktop\\carnumber\\LicensePlate\\carlicenseplate3.jpg")
gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)

gray = cv2.bilateralFilter(gray, 11, 17, 17)

edged = cv2.Canny(gray, 30, 200) #Perform Edge detection

#contours, hierarchy=cv2.findContours(edged, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

cnts = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
cnts = sorted(cnts, key = cv2.contourArea, reverse = True)[:10]
screenCnt = None

for c in cnts:
                # 대략적인 윤곽선
                peri = cv2.arcLength(c, True)
                approx = cv2.approxPolyDP(c, 0.018 * peri, True)

                # 근사 개요에 4 점이있는 경우,
                # 화면을 찾았다 고 가정 해 봄
                if len(approx) == 4:
                      screenCnt = approx
                      break

# drawContours() 함수는 영상 내의 각 외곽선을 그릴 수 있는 함수
#                           외곽선 색상 두께
cv2.drawContours(src,[approx],0,(0,255,0),3)
cv2.imshow('src', src )


# 마스킹 구간 뺴고 전부 지워버리기
mask = np.zeros(gray.shape,np.uint8)
new_image = cv2.drawContours(mask,[screenCnt],0,255,-1,)
new_image = cv2.bitwise_and(src,src,mask=mask)   
cv2.imshow('new_image', new_image )

# 잘라내기
(x, y) = np.where(mask == 255)
(topx, topy) = (np.min(x), np.min(y))
(bottomx, bottomy) = (np.max(x), np.max(y))
Cropped = gray[topx:bottomx+1, topy:bottomy+1]
cv2.imshow('Cropped', Cropped )


#텍스트 추출
#테서랙트 경로설정
pytesseract.pytesseract.tesseract_cmd = 'C:\\Program Files (x86)\\Tesseract-OCR\\tesseract'
kernel=np.ones((3,3),np.uint8)
Cropped = cv2.erode(Cropped,kernel,iterations=1)
text = pytesseract.image_to_string(Cropped, config='-l kor --oem 3 --psm 11')
print("Detected Number is:",text)

cv2.waitKey(0)
cv2.destroyAllWindows()