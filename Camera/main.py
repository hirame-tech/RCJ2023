import cv2
import numpy as np
import time
import subprocess
import serial

i=0
pi=np.pi
fps=25
rad=pi/3

lmin=30 #片方のゴールしか見えないとき、ゴールの長辺がこれを超えたら自己位置を確定する


def decode_fourcc(v): #画像のフォーマットを確認する。デバッグ用の関数
    v=int(v)
    return "".join([chr((v>>8*i)&0xFF) for i in range(4)])
    
def shell(com): #コマンドシェルのコマンドを実行する。デバッグ用の関数
	proc=subprocess.run(com,shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,text=True)
	result=proc.stdout.split("=")
	return result

camera=cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT,320)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,320)
camera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc("R","G","B","3"))
camera.set(cv2.CAP_PROP_BUFFERSIZE,1)
camera.set(cv2.CAP_PROP_FPS,fps)


#print(decode_fourcc(camera.get(cv2.CAP_PROP_FOURCC)))

nans42=np.zeros([4,2])
nans42[:,:]=np.nan
blue=[(71,40,0),(97,255,255),(136,0,0),(196,255,255)] #[(lower),(upper)],(色相(/2)、彩度(x2.55)、明度(x2.55))
yellow=[(32,0,0),(52,255,255),(0,0,0),(255,115,255)] 


def atan(x,y): #画像中心の座標から既定の向きの角度を返す
    return np.arctan2(-x,y)+pi

def length(line):
    x1=line[0][0]
    y1=line[0][1]
    x2=line[1][0]
    y2=line[1][1]

    return np.sqrt((x2-x1)**2+(y2-y1)**2)
    
def nichika(img,hsvl,hsvu,yuvl,yuvu): #画像を二値化する引数は(画像、(hsv最小値),(hsv最大値),(yuv最小値),(yuv最大値))
    hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    hsv2chi=cv2.inRange(hsv,hsvl,hsvu)
    yuv=cv2.cvtColor(img,cv2.COLOR_BGR2YUV)
    yuv2chi=cv2.inRange(yuv,yuvl,yuvu)

    nichika=cv2.bitwise_and(hsv2chi,yuv2chi)
    
    return nichika

def addshape(binimg,img,w,h,rgb): #引数は（二値化画像、マスク処理後画像、画像幅、画像高さ） 返り値は物体の方向(角度)
    nlabels,labels,stats,center=cv2.connectedComponentsWithStats(binimg)
    if nlabels!=1:
        big=np.argmax(stats[1:,4])+1 #最も大きい塊のラベルを取得
        obj=np.fliplr(np.array(list(zip(*np.where(labels==big))))) #最も大きい塊の座標群
        rect=cv2.minAreaRect(obj) #傾き考慮の外接短形,rect=((左上座標),w,h,回転角)
        box=np.intp(cv2.boxPoints(rect)) #実際の実行時はいらない

        cv2.drawContours(img,[box],-1,color=rgb,thickness=3)
        angle=atan(rect[0][0]-w/2,rect[0][1]-h/2) #物体の中心からの角度     


        """l=stats[big][0] #left end
        r=stats[big][0]+stats[big][2] #right end
        u=stats[big][1] #upper end
        b=stats[big][1]+stats[big][3] #bottom end
        cv2.rectangle(img,(l,u),(r,b),(0,255,0),thickness=10) #物体を長方形で示す"""
        return angle,rect
    else:
        return None,None
        
def det_own(rect1,rect2,w,h,img):
    if rect1==None:
        center1=None
    elif rect1[1][0]>3 or rect1[1][1]>3:
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
        print("blue l:",length(la))
    else:
        center1=None

    if rect2==None:
        center2=None
    elif rect2[1][0]>3 or rect2[1][1]>3:
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
        print("yellow l:",length(lb))
    else:
        center2=None

    print("rect1,recr2:",rect1,rect2)

    if center1!=None and center2!=None: #両方のゴールを視認
        

        def det_center(p1,p2,q1,q2,w,h):
            global i
            if (not all(np.isnan(p1)))&(not all(np.isnan(p2)))&(not all(np.isnan(q1)))&(not all(np.isnan(q2))):
                
                #print(type(p1))
                cv2.line(img,p1,q2,(0,0,255),thickness=2)
                cv2.line(img,p2,q1,(0,0,255),thickness=2)
            
                s1=p1[0]-w/2
                t1=p1[1]-h/2
                s2=p2[0]-w/2
                t2=p2[1]-h/2
                u1=q1[0]-w/2
                r1=q1[1]-h/2
                u2=q2[0]-w/2
                r2=q2[1]-h/2

                bumbo=(t2-r1)*(u2-s1)-(s2-u1)*(r2-t1)
                nua=(s2-u1)*(t1-r1)-(t2-r1)*(s1-u1)
                nub=(u2-s1)*(t1-r1)-(r2-t1)*(s1-u1)
                ua=nua/bumbo
                ub=nub/bumbo
                
                x=s1+ua*(u2-s1)
                y=t1+ua*(r2-t1)
                if (not (np.isinf(x) or np.isnan(x)) )and(not (np.isinf(y) or np.isnan(y)))and((0<x and x<w) & (0<y and y<h)):
                    cv2.arrowedLine(img,(int(w/2),int(h/2)),(int(x+w/2),int(y+h/2)),color=(255,0,0),thickness=4)

                    return x,-y
                elif not((0<x+w/2 and x+w/2<w) & (0<y+h/2 and y+h/2<h)):
                    if i==0:
                        i=1
                        x,y=det_center(p1,p2,q2,q1,w,h)
                        return x,y
                    else:
                        i=0
                        return np.nan,np.nan
                else:
                    return x,-y
            else:
                return np.nan,np.nan
            
        rad1=atan(center1[0]-w/2,center1[1]-h/2)
        rad2=atan(center2[0]-w/2,center2[1]-h/2)
        drad=rad1-rad2
        if drad>pi or -pi>drad:
            if drad>pi:
                drad=drad-2*pi
            else:
                drad=2*pi-drad
        else:
            pass

        print("rad1:",rad1)
        print("rad2:",rad2)
        print("drad:",drad)

        if drad<rad and -rad<drad:
            print("-rad<drad<rad")
            if drad>0:
                print("drad>0")
                lr="l"
            else:
                print("drad<0")
                drad=-drad
                lr="r"
            if length(la)<length(lb):
                by="y"
            else:
                by="b"
        else:
            print("rad<drad,drad<-rad");  
            x,y=det_center(la[0],la[1],lb[0],lb[1],w,h) #x,yは画像中心、右と上が正
            if length(la)<length(lb):
                by="y"
            else:
                by="b"
            vby_x=center2[0]-center1[0]
            vby_y=center2[1]-center1[1]
            vbc_x=w/2-center1[0]
            vbc_y=h/2-center1[1]

            crs=vby_x*vbc_y-vby_y*vbc_x
            print(crs)
            if crs>=0:
                lr="r"
            else:
                lr="l"


        return lr,by
    else:
        if center1!=None and center2==None: #青のみ認識
            print("det blue")
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



def main():
    
    lr="x"
    by="x"

    ret,img=camera.read()
    if not ret:
        print("notret")
    #cv2.imshow("img_bf",img)
    img=img[29:,0:303]
    img=np.insert(img,291,np.zeros((15,303,3)),axis=0)
    img=cv2.rotate(img,cv2.ROTATE_90_CLOCKWISE)
    img=cv2.flip(img,1)

    height, width=img.shape[:2]

    cv2.imshow("img",img)

    img=cv2.circle(img,(int(width/2),int(height/2)),4,color=(0,0,255),thickness=1)
    img=cv2.circle(img,(int(width/2),int(height/2)),int(width/2)+6+25,color=(0,0,0),thickness=50)
    
    maskb=nichika(img,blue[0],blue[1],blue[2],blue[3])
    #maskedb=cv2.bitwise_and(img,img,mask=maskb)

    masky=nichika(img,yellow[0],yellow[1],yellow[2],yellow[3])
    #maskedy=cv2.bitwise_and(img,img,mask=masky)
    angleb,rectb=addshape(maskb,img,width,height,(255,0,0)) 
    angley,recty=addshape(masky,img,width,height,(0,255,0))
    lr,by=det_own(rectb,recty,width,height,img)
    cv2.imshow("img_aft",img)

    return angleb,angley,lr,by

def cvtangle(angle):
    if angle>2*pi:
        angle-=2*pi
    else:
        pass
    n=4*angle/pi
    intn=np.round(n).astype(int)
    if intn==8:
        intn=0
    return intn
            
if __name__=='__main__':
    ser=serial.Serial('/dev/ttyACM0', 115200, timeout=0.1)
    ser.flush()
    while True:
        try:
            angleb,angley,lr,by=main()
        except:
            angleb,angley,lr,by=None,None,"x","x"
        """
         7  0  1     
          ¥ | /      
           ¥|/
        6---+----2
           /|¥
          / | ¥
         5  4  3      角度を左に様に3bitで表せるように変形"""
        if angleb!=None:
            int_angleb=cvtangle(angleb)
        else:
            int_angleb=8
        if angley!=None:
            int_angley=cvtangle(angley)
        else:
            int_angley=8

        data_string=f"{int_angleb},{int_angley},{lr},{by}\n"
        ser.write(data_string.encode())
        
        key=cv2.waitKey(1)
        if key==27:
            break


camera.release()
cv2.destroyAllWindows()