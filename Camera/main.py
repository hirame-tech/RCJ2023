import cv2
import numpy as np
import subprocess
import serial
import random
import signal,sys
import datetime

rand3=int(random.random()*999)


def atan(x,y): #画像中心の座標から既定の向きの角度を返す
    return np.arctan2(-x,y)+pi

def length(line):
    x1=line[0][0]
    y1=line[0][1]
    x2=line[1][0]
    y2=line[1][1]

    return np.sqrt((x2-x1)**2+(y2-y1)**2)
    
def nichika(img,hsvl,hsvu,yuvl,yuvu): #二値化画像を返す
    #(画像、(hsv最小値),(hsv最大値),(yuv最小値),(yuv最大値))
    hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    hsv2chi=cv2.inRange(hsv,hsvl,hsvu)
    yuv=cv2.cvtColor(img,cv2.COLOR_BGR2YUV)
    yuv2chi=cv2.inRange(yuv,yuvl,yuvu)

    nichika=cv2.bitwise_and(hsv2chi,yuv2chi)
    
    return nichika

def addshape(binimg,img,w,h,rgb): 
    #（二値化画像、マスク処理後画像、画像幅、画像高さ） 返り値は物体の方向(角度)
    nlabels,labels,stats,center=cv2.connectedComponentsWithStats(binimg)
    if nlabels!=1:
        big=np.argmax(stats[1:,4])+1 #最も大きい塊のラベルを取得
        obj=np.fliplr(np.array(list(zip(*np.where(labels==big))))) #最も大きい塊の座標群
        rect=cv2.minAreaRect(obj) #傾き考慮の外接短形
        box=np.intp(cv2.boxPoints(rect))

        cv2.drawContours(img,[box],-1,color=rgb,thickness=1)
        angle=atan(rect[0][0]-w/2,rect[0][1]-h/2) #物体の画像中心からの角度     


        l=stats[big][0] #left end
        r=stats[big][0]+stats[big][2] #right end
        u=stats[big][1] #upper end
        b=stats[big][1]+stats[big][3] #bottom end
        cv2.rectangle(img,(l,u),(r,b),(0,255,0),thickness=10) #物体を長方形で示す
        return angle,rect
    else:
        return None,None
        
def det_own(rect1,rect2,w,h,img):#自己位置推定
    if rect1==None:
        center1=None
    elif rect1[1][0]>rectmin or rect1[1][1]>rectmin:
        center1=rect1[0]
        box1 = cv2.boxPoints(rect1).astype(int)
        if rect1[1][0] < rect1[1][1]:
            qx=(box1[0][0]+box1[3][0])/2
            qy=(box1[0][1]+box1[3][1])/2
            rx=(box1[1][0]+box1[2][0])/2
            ry=(box1[1][1]+box1[2][1])/2
        else:
            qx=(box1[0][0]+box1[1][0])/2
            qy=(box1[0][1]+box1[1][1])/2
            rx=(box1[2][0]+box1[3][0])/2
            ry=(box1[2][1]+box1[3][1])/2
        la=np.round([(qx,qy),(rx,ry)]).astype(int)
    else:
        center1=None

    if rect2==None:
        center2=None
    elif rect2[1][0]>rectmin or rect2[1][1]>rectmin:
        center2=rect2[0]
        box2 = cv2.boxPoints(rect2).astype(int)
        if rect2[1][0] < rect2[1][1]:
            sx=(box2[0][0]+box2[3][0])/2
            sy=(box2[0][1]+box2[3][1])/2
            tx=(box2[1][0]+box2[2][0])/2
            ty=(box2[1][1]+box2[2][1])/2
        else:
            sx=(box2[0][0]+box2[1][0])/2
            sy=(box2[0][1]+box2[1][1])/2
            tx=(box2[2][0]+box2[3][0])/2
            ty=(box2[2][1]+box2[3][1])/2
        lb=np.round([(sx,sy),(tx,ty)]).astype(int)
    else:
        center2=None

    if center1!=None and center2!=None: #両方のゴールを視認
        if abs(length(la)-length(lb))>55:
            if length(la)<length(lb):
                by="y"
            else:
                by="b"
        else:
            by="m"
        #青ゴールと黄色ゴールを結ぶベクトルBY
        vby_x=center2[0]-center1[0]
        vby_y=center2[1]-center1[1]
        #青ゴールと画像中心を結ぶベクトルBC
        vbc_x=w/2-center1[0]
        vbc_y=h/2-center1[1]

        #BYxBC(外積)
        crs=vby_x*vbc_y-vby_y*vbc_x #
        #BYとBCの成す角 [-pi/2,pi/2]
        drad=np.arcsin(crs/(np.sqrt(vby_x**2+vby_y**2)*np.sqrt(vbc_x**2+vbc_y**2)))
        print("drad:",drad)
        if abs(drad)<rad:
            lr="c"
        elif crs>=0:
            lr="r"
        else:
            lr="l"
        
        return lr,by
    else:
        if center1!=None and center2==None: #青のみ認識
            if length(la)>lmin:
                by="b"
            else:
                by="x"
        elif center2!=None: #黄色のみ認識
            if length(lb)>lmin:
                by="y"
            else:
                by="x"
        else: #どちらも認識不可
            by="x"
        
        return "x",by

def running_exit(sig,frame):#終了処理
    try:
        ser.close()
    except:
        pass
    camera.release()
    cv2.destroyAllWindows()
    print("終了処理を実行完了")
    sys.exit(0)

def main():
    
    lr="x"
    by="x"

    ret,img=camera.read()
    if not ret:
        print("Cant read camera")
        return None,None,"x","x"
    else:
        #cv2.imshow("img_bf",img)
        img=img[29:,0:303]
        img=np.insert(img,291,np.zeros((15,303,3)),axis=0)
        img=cv2.rotate(img,cv2.ROTATE_90_CLOCKWISE)
        img=cv2.flip(img,1)


        height, width=img.shape[:2]

        cv2.imshow("img",img)
        img=cv2.circle(img,(int(width/2),int(height/2)),4,color=(0,0,255),thickness=1)
        img=cv2.circle(img,(int(width/2),int(height/2)),int(width/2)+40,color=(0,0,0),thickness=80)
        
        maskb=nichika(img,blue[0],blue[1],blue[2],blue[3])
        masky=nichika(img,yellow[0],yellow[1],yellow[2],yellow[3])
        angleb,rectb=addshape(maskb,img,width,height,(255,0,0)) 
        angley,recty=addshape(masky,img,width,height,(0,255,0))
        lr,by=det_own(rectb,recty,width,height,img)

        try:
            cv2.imshow("img",img)
        except:
            print("disable:show img")
            pass

        return angleb,angley,lr,by

def cvtangle(angle):#角度を時計回りの0~7に変換
    if angle>2*pi:
        angle-=2*pi
    else:
        pass
    n=4*angle/pi
    intn=np.round(n).astype(int)
    if intn==8:
        intn=0
    return intn

"main"

signal.signal(signal.SIGTERM,running_exit)

i=0
pi=np.pi
fps=25
rad=pi/40 #dradの値がrad以下ならlr自己位置を"c"(center)とする。

lmin=30 #片方のゴールしか見えないとき、自己位置推定を行う長辺のしきい値
rectmin=2 #ゴールを認識するための長辺のしきい値

camera=cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT,320)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,320)
camera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc("R","G","B","3"))
camera.set(cv2.CAP_PROP_BUFFERSIZE,1)
camera.set(cv2.CAP_PROP_FPS,fps)

nans42=np.zeros([4,2])
nans42[:,:]=np.nan
blue=[(77,40,0),(120,255,255),(0,0,0),(255,255,255)] #[(lower),(upper)],(色相(/2)、彩度(x2.55)、明度(x2.55))
yellow=[(28,0,0),(52,255,255),(0,0,0),(255,115,255)] 

try:
    ser=serial.Serial("/dev/ttyAMA0", 115200, timeout=0.1)
except:
    print("error:set serial")
    pass

while True:
    try:
        angleb,angley,lr,by=main()
    except Exception as e:
        print(e)
        angleb,angley,lr,by=None,None,"x","x"
    
    if angleb!=None:
        int_angleb=cvtangle(angleb)
    else:
        int_angleb=8
    if angley!=None:
        int_angley=cvtangle(angley)
    else:
        int_angley=8

    data_string=f"{int_angleb},{int_angley},{lr},{by}\n"
    try:
        ser.write(data_string.encode())
        ser.flush()
        print("sent data")
    except:
        print("CANT SEND DATA")
        pass
    key=cv2.waitKey(1)
