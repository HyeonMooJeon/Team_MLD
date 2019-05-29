
import os 
path = "C:\\Users\\person\\Desktop\\darknet-master\\Yolo_mark-master\\Yolo_mark-master\\x64\\Release\\data\\img\\" # \는 두 번 적는다.

def rename_file():
   # idx = 379
    for filename in os.listdir(path):
        #print(filename)
   
        if filename.lower().endswith((".txt")):
            if filename.lower().startswith(("license")):
                print(filename)
                f = open(path+filename, 'r+t')
                line = f.readline()
                txt_str= ""
                for i in range(2,37) :
                    txt_str += line[i]
                print(line)
                print(txt_str)
                print(f.tell())
                f.seek(0)
                f.write("10 "+txt_str)
                #line[0] = "16 " 
                #print(line)
                
                f.close()

                '''
                newName =idx
                idx=idx+1
                print(newName)
          
                os.rename(""+filename, ""+str(newName)+".jpg")       
                '''

if __name__ == "__main__":

    rename_file()
