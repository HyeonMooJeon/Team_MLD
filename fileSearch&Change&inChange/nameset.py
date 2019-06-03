
import os 
#path = "C:\\Users\\person\\Desktop\\darknet-master\\Yolo_mark-master\\Yolo_mark-master\\x64\\Release\\data\\img\\" # \는 두 번 적는다.
#path = "C:\\Users\\person\\Desktop\\carnumber\\result_of_The_Tesing_data\\result_of_The_firstData\\"
path = "C:\\Users\\person\\Desktop\\새 폴더 (2)\\수\\"
def rename_file():
    idx = 1
    for filename in os.listdir(path):
        print(filename)
   
        if filename.lower().endswith((".jpg")):
            newName ="soo_plate"+str(idx)
            idx=idx+1
            print(newName)
          
            os.rename(path+filename, path+str(newName)+".jpg")

      

if __name__ == "__main__":

    rename_file()