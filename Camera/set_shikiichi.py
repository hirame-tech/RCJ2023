import cv2
import numpy as np

fps=5
global click_x
global click_y

def nichika(img,hsv_l,hsv_u,yuv_l,yuv_u):
    hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    yuv=cv2.cvtColor(img,cv2.COLOR_BGR2YUV)

    hsv2=cv2.inRange(hsv,np.array(hsv_l),np.array(hsv_u))
    yuv2=cv2.inRange(yuv,np.array(yuv_l),np.array(yuv_u))

    nichika=cv2.bitwise_and(hsv2,yuv2)

    return hsv2,yuv2,nichika

def det_click(event,x,y,frags,param): 
    if event==cv2.EVENT_LBUTTONDOWN:
        hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
        yuv=cv2.cvtColor(img,cv2.COLOR_BGR2YUV)
        print(f"(x,y)={(x,y)}")
        print(f"hsv:{hsv[y][x]}")
        print(f"yuv:{yuv[y][x]}")


camera=cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT,320)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,320)
camera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc("R","G","B","3"))
camera.set(cv2.CAP_PROP_BUFFERSIZE,1)
camera.set(cv2.CAP_PROP_FPS,fps)

lhsv,uhsv=[0,0,0],[179,255,255]
lyuv,uyuv=[0,0,0],[255,255,255]

i=0

while True:

    ret,img=camera.read()
    img=img[29:,0:303]
    img=np.insert(img,291,np.zeros((15,303,3)),axis=0)
       
    height, width=img.shape[:2]
    img=cv2.circle(img,(int(width/2),int(height/2)),int(width/2)+6+25,color=(0,0,0),thickness=50)

    
    if not ret:
        break
    
    key=cv2.waitKey(1)
    if key==32:
        lhsv_in=input("Low(h,s,v)=").split(",")
        uhsv_in=input("Up(h,s,v)=").split(",")
        lyuv_in=input("Low(y,u,v)=").split(",")
        uyuv_in=input("Up(y,u,v)=").split(",")
        for outlist,inlist in [[lhsv,lhsv_in],[uhsv,uhsv_in],[lyuv,lyuv_in],[uyuv,uyuv_in]]:
            for n in range(0,3):
                if inlist[n]=="":
                    pass
                else:
                    print(inlist[n])
                    try:
                        outlist[n]=int(inlist[n])
                    except:
                        pass

        print(f"[hsv]=[{lhsv},{uhsv}]")
        print(f"[yuv]=[{lyuv},{uyuv}]")
    elif key==112:
        if i==0:
            i=1
            picname=input("~~~.jpg")
            pic=cv2.imread(f"/home/pi/Desktop/imgs/{picname}.jpg")
            img2=pic
        else:
            i=0
            img2=img
    else:
        if i==0:
            img2=img
        else:
            img2=pic

    hsv,yuv,nichi=nichika(img2,lhsv,uhsv,lyuv,uyuv)
    cv2.imshow("nichi",nichi)
    cv2.imshow("hsv",hsv)
    cv2.imshow("yuv",yuv)
    cv2.imshow("img",img2)

    cv2.setMouseCallback("hsv",det_click,param=img2)
    cv2.setMouseCallback("yuv",det_click,param=img2)
    cv2.setMouseCallback("img",det_click,param=img2)

    if key==27:
        break
    

    
camera.release()
cv2.destroyAllWindows()

