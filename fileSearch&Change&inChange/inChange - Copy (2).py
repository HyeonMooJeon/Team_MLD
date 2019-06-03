
import os 
path = "C:\\Users\\person\\Desktop\\darknet-master\\Yolo_mark-master\\Yolo_mark-master\\x64\\Release\\data\\img\\" # \는 두 번 적는다.

def rename_file():
   # idx = 379
    for filename in os.listdir(path):
        #print(filename)
   
        if filename.lower().endswith((".txt")):
            if filename.lower().startswith(("car")):
                #print(filename)
                f = open(path+filename, 'r+t')
                line = f.readline()
                if "10 " in line :
                    print(filename)

if __name__ == "__main__":

    rename_file()
