
import os 
path = "C:\\Users\\person\\Desktop\\carnumber\\hm_licenseplate2\\" # \는 두 번 적는다.

def rename_file():
    idx = 1
    for filename in os.listdir(path):
        print(filename)
   
        if filename.lower().endswith((".jpg")):
            
            newName ="carnumber"+str(idx)
            idx=idx+1
            print(newName)
          
            os.rename(path+filename, path+str(newName)+".jpg")

            if len(filename) == 22 :
                os.rename(path+"carlicenseplate"+filename[15]+filename[16]+filename[17]+".txt", path+str(newName)+".txt")
            if len(filename) == 21 :
                #print(filename[15]+filename[16]+".jpg")
                os.rename(path+"carlicenseplate"+filename[15]+filename[16]+".txt", path+str(newName)+".txt")
            if len(filename) == 20 :
                os.rename(path+"carlicenseplate"+filename[15]+".txt", path+str(newName)+".txt")

if __name__ == "__main__":

    rename_file()