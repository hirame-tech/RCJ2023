import cv2
import numpy as np

fps=25
camera=cv2.VideoCapture(0)
blue=[(93,59,68),(166,255,255)] #[(lower),(upper)],(色相(/2)、彩度(x2.55)、明度(x2.55))
yellow=[(10,51,51),(35,255,255)]

def nichika(img,lower,upper): #画像を二値化する引数は(画像、(hsvタプル),(hsvタプル))
    hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    nichika=cv2.inRange(hsv,lower,upper)
    
    return nichika

def addshape(binimg,img,w,h): #引数は（二値化画像、マスク処理後画像、画像幅、画像高さ） 返り値は物体の方向(角度)
    nlabels,labels,stats,center=cv2.connectedComponentsWithStats(binimg)
    if nlabels!=1:
        big=np.argmax(stats[1:,4])+1 #最も大きい塊のラベルを取得
        cv2.rectangle(img,(stats[big][0],stats[big][1]),(stats[big][0]+stats[big][2],stats[big][1]+stats[big][3]),(0,255,0),thickness=10) #物体を長方形で示す
        angle=np.arctan2(h/2-center[big][1],center[big][0]-w/2) #物体の中心からの角度
        return angle

while True:
    ret,img=camera.read()
    if not ret:
        break
    width, height=img.shape[:2]
    
    maskb=nichika(img,blue[0],blue[1])
    maskedb=cv2.bitwise_and(img,img,mask=maskb)

    masky=nichika(img,yellow[0],yellow[1])
    maskedy=cv2.bitwise_and(img,img,mask=masky)

    addshape(maskb,maskedb,width,height) 
    addshape(masky,maskedy,width,height)   
    cv2.imshow("maskedBlue",maskedb)
    cv2.imshow("maskedYellow",maskedy)
    key=cv2.waitKey(int(1000/fps))
    if key==27:
        break

camera.release()
cv2.destroyAllWindows()