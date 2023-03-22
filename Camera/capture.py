import cv2
import numpy as np

pi=np.pi
fps=10
print("r")
camera=cv2.VideoCapture(0)
#camera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc("B","G","R","8"))
camera.set(cv2.CAP_PROP_FRAME_HEIGHT,320)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,320)
camera.set(cv2.CAP_PROP_BUFFERSIZE,1)
camera.set(cv2.CAP_PROP_FPS,fps)

print(camera.get(cv2.CAP_PROP_FOURCC))

nans42=np.zeros([4,2])
nans42[:,:]=np.nan
blue=[(93,59,68),(166,255,255)] #[(lower),(upper)],(色相(/2)、彩度(x2.55)、明度(x2.55))
yellow=[(10,51,51),(35,255,255)] 
print("s")

def atan(x,y):
    return np.arctan2(-x,y)+pi
    
def nichika(img,lower,upper): #画像を二値化する引数は(画像、(hsvタプル),(hsvタプル))
    hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    nichika=cv2.inRange(hsv,lower,upper)
    
    return nichika

def addshape(binimg,img,w,h,rgb): #引数は（二値化画像、マスク処理後画像、画像幅、画像高さ） 返り値は物体の方向(角度)
    nlabels,labels,stats,center=cv2.connectedComponentsWithStats(binimg)
    if nlabels!=1:
        big=np.argmax(stats[1:,4])+1 #最も大きい塊のラベルを取得
        obj=np.fliplr(np.array(list(zip(*np.where(labels==big))))) #最も大きい塊の座標群
        rect=cv2.minAreaRect(obj) #傾き考慮の外接短形,rect=((左上座標),w,h,回転角)
        box=np.intp(cv2.boxPoints(rect)) #傾き考慮の外接短形の四隅

        cv2.drawContours(img,[box],-1,color=rgb,thickness=3)
        angle=atan(center[big][0]-w/2,center[big][1]-h/2) #物体の中心からの角度     


        """l=stats[big][0] #left end
        r=stats[big][0]+stats[big][2] #right end
        u=stats[big][1] #upper end
        b=stats[big][1]+stats[big][3] #bottom end
        cv2.rectangle(img,(l,u),(r,b),(0,255,0),thickness=10) #物体を長方形で示す"""
        return angle,box,np.array(center[big])
    else:
        return None,nans42,(np.nan,np.nan)
        
def det_own(box1,c1,box2,c2,w,h):
    if (box1!=nans42).all and (box2!=nans42).all:

        p11=box1[0]
        p12=box1[1]
        p13=box1[2]
        p14=box1[3]
        p21=box2[0]
        p22=box2[1]
        p23=box2[2]
        p24=box2[3]

        """l1=l1-w/2
        r1=r1-w/2
        u1=-(u1-h/2)
        b1=-(b1-h/2)
        
        l2=l2-w/2
        r2=r2-w/2
        u2=-(u2-h/2)
        b2=-(b2-h/2)"""
        
        x1=c1[0]-w/2
        y1=-c1[1]+h/2
        x2=c2[0]-w/2
        y2=-c2[1]+h/2
        #print("c1",c1)
        #grad=(y2-y1)/(x2-x1)

        def det_center(p1,p2,q1,q2,w,h):
            s1=p1[0]-w/2 #gazou shuusinn ni henkou, migi ga x sei
            t1=-p1[1]+h/2# ue ga y sei
            s2=p2[0]-w/2
            t2=-p2[1]+h/2
            u1=q1[0]-w/2
            r1=-q1[1]+h/2
            u2=q2[0]-w/2
            r2=-q2[1]+h/2
            x=((u1-s2)*(u2-s1)*(t1-t2)+(r1-t2)*(u2-s1)*s2-(u1-s2)*(r2-t1)+s1)/((r1-t2)*(u2-s1)-(r2-t1)*(u1-s2))
            y=(r2-t1)*(x-s1)/(u2-s1)+t1
            return x,y
        d11=(p11[0]-p12[0])**2+(p11[1]-p12[1])**2
        d12=(p11[0]-p13[0])**2+(p11[1]-p13[1])**2
        d21=(p23[0]-p24[0])**2+(p23[1]-p24[1])**2
        d22=(p22[0]-p24[0])**2+(p22[1]-p24[1])**2
        if(d11>d12):
            if (d21>d22): 
                x,y=det_center(p11,p12,p23,p24,w,h)
                if (p11[1]+p12[1])>(p23[1]+p24[1]):#青が上
                    if x>0:
                        lr="r"
                    else:
                        lr="l"
                    if y>0:
                        by="y"
                    else:
                        by="b"
                else:#青が下
                    if x>0:
                        lr="l"
                    else:
                        lr="r"
                    if y>0:
                        by="b"
                    else:
                        by="y"

            else:
                x,y=det_center(p11,p12,p22,p24,w,h)
                if (p11[1]+p12[1])>(p22[1]+p24[1]):#青が上
                    if x>0:
                        lr="r"
                    else:
                        lr="l"
                    if y>0:
                        by="y"
                    else:
                        by="b"
                else:#青が下
                    if x>0:
                        lr="l"
                    else:
                        lr="r"
                    if y>0:
                        by="b"
                    else:
                        by="y"
        else:
            if (d21>d22): 
                x,y=det_center(p11,p13,p23,p24,w,h)
                if (p11[1]+p13[1])>(p23[1]+p24[1]):#青が上
                    if x>0:
                        lr="r"
                    else:
                        lr="l"
                    if y>0:
                        by="y"
                    else:
                        by="b"
                else:#青が下
                    if x>0:
                        lr="l"
                    else:
                        lr="r"
                    if y>0:
                        by="b"
                    else:
                        by="y"

            else:
                x,y=det_center(p11,p13,p22,p24,w,h)
                if (p11[1]+p13[1])>(p22[1]+p24[1]):#青が上
                    if x>0:
                        lr="r"
                    else:
                        lr="l"
                    if y>0:
                        by="y"
                    else:
                        by="b"
                else:#青が下
                    if x>0:
                        lr="l"
                    else:
                        lr="r"
                    if y>0:
                        by="b"
                    else:
                        by="y"
        #r=abs(x**2+y**2)    
        return by,lr,x,y
    elif (box1!=nans42).all:
        if -c1[1]+h/2>0:
            if c1[0]-w/2>0:
                lr="r"
            else:
                lr="l"
        else:
            if c1[0]-w/2>0:
                lr="l"
            else:
                lr="r"
        return None,lr,None,None
    elif (box2!=nans42).all:
        if -c2[1]+h/2>0:
            if c2[0]-w/2>0:
                lr="l"
            else:
                lr="r"
        else:
            if c1[0]-w/2>0:
                lr="r"
            else:
                lr="l"
        return None,lr,None,None
    else:
        return None,None,None,None

def main():
    while True:
        ret,img=camera.read()
        if not ret:
            break
        cv2.imshow("img_bf",img)
        ##img=img[38:480,87:544]
        #img=np.insert(img,0,np.zeros((28,457,3)),axis=0)
        img=cv2.rotate(img,cv2.ROTATE_90_CLOCKWISE)
        img=cv2.flip(img,1)

        width, height=img.shape[:2]
        img=cv2.circle(img,(int(width/2),int(height/2)),4,color=(0,0,255),thickness=1)
        cv2.circle(img,(int(width/2),int(height/2)),256,color=(0,0,0),thickness=80)
        maskb=nichika(img,blue[0],blue[1])
        maskedb=cv2.bitwise_and(img,img,mask=maskb)

        masky=nichika(img,yellow[0],yellow[1])
        maskedy=cv2.bitwise_and(img,img,mask=masky)
        angleb,boxb,cb=addshape(maskb,img,width,height,(255,0,0)) 
        angley,boxy,cy=addshape(masky,img,width,height,(0,255,0))   
        by,lr,x,y=det_own(boxb,cb,boxy,cy,width,height)
        if (np.isnan(x)==0 and np.isinf(x)==0 and np.isinf(y)==0):
            print("->")
            cv2.arrowedLine(img,(int(width/2),int(height/2)),(int(x+width/2),int(-y+height/2)),(255,0,0),thickness=10)
        print(x,y,lr,by)
        #cv2.imshow("masked",cv2.bitwise_or(maskedb,maskedy))
        cv2.imshow("img_aft",img)
        key=cv2.waitKey(1)

        if key==27:
            break
            
main()

camera.release()
cv2.destroyAllWindows()
