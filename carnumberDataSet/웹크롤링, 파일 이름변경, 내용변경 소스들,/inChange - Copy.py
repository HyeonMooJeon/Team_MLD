
import os 
path = "C:\\Users\\person\\Desktop\\carnumber\\hm_licenseplate2\\" 

# \는 두 번 적는다.
##version 빈 텍스트파일 지우기

def rename_file():
   # idx = 379
    for filename in os.listdir(path):
        #print(filename)
   
        if filename.lower().endswith((".txt")):
            if filename.lower().startswith(("car")):
            
                f = open(path+filename, 'r+t')
                line = f.readline()
                #print(line)
                
                if not line:
                   print(filename)

                   if len(filename) == 22 :
                       os.remove(path+"carlicenseplate"+filename[15]+filename[16]+filename[17]+".jpg")

                   if len(filename) == 21 :
                       #print(filename[15]+filename[16]+".jpg")
                       os.remove(path+"carlicenseplate"+filename[15]+filename[16]+".jpg")
                   
                        
                   f.close()
                   os.remove(path+filename)

                #print(f.tell())

                #line[0] = "16 " 
                #print(line)
                
               

                '''
                newName =idx
                idx=idx+1
                print(newName)
          
                os.rename(""+filename, ""+str(newName)+".jpg")       
                '''

if __name__ == "__main__":

    rename_file()
