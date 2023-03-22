import cv2
import numpy as np

fps=15

def nichika(img,hsv_l,hsv_u,yuv_l,yuv_u):
    hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    yuv=cv2.cvtColor(img,cv2.COLOR_BGR2YUV)

    hsv2=cv2.inRange(hsv,hsv_l,hsv_u)
    yuv2=cv2.inRange(yuv,yuv_l,yuv_u)

    nichika=cv2.bitwise_and(hsv2,yuv2)

    return hsv2,yuv2,nichika


camera=cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT,320)
camera.set(cv2.CAP_PROP_FRAME_WIDTH,320)
camera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc("R","G","B","3"))
camera.set(cv2.CAP_PROP_BUFFERSIZE,1)
camera.set(cv2.CAP_PROP_FPS,fps)

hl,hu=0,255
sl,su=0,255
vl,vu=0,255

yl,yu=0,255
ul,uu=0,255
v2l,v2u=0,255

while True:
    ret,img=camera.read()
    img=img[29:,0:303]
    img=np.insert(img,291,np.zeros((15,303,3)),axis=0)
    

    height, width=img.shape[:2]
    img=cv2.circle(img,(int(width/2),int(height/2)),int(width/2)+6+25,color=(0,0,0),thickness=50)
    if not ret:
        break
    
    key=cv2.waitKey(1)

    if key==97:
        hl+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==122:
        hl-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==115:
        sl+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==120:
        sl-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==100:
        vl+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==99:
        vl-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==103:
        hu+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==98:
        hu-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==104:
        su+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==110:
        su-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==106:
        vu+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    if key==109:
        vu-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==49: #press 1
        yl+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==113: #press Q
        yl-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==50: #press 2
        ul+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==119: #press W
        ul-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    
    if key==51: #press 3
        v2l+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    
    if key==101: #press E
        v2l-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==55: #press 7
        yu+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==117: #press u
        yu-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)
    
    if key==56: #press 8
        uu+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==105: #press i
        uu-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==57: #press 9
        v2u+=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    if key==111: #press o
        v2u-=1
        print("hsv:",hl,sl,vl,hu,su,vu,"\nyuv",yl,ul,v2l,yu,uu,v2u)

    hsv,yuv,nichi=nichika(img,(hl,sl,vl),(hu,su,vu),(yl,ul,v2l),(yu,uu,v2u))
    cv2.imshow("nichi",nichi)
    cv2.imshow("hsv",hsv)
    cv2.imshow("yuv",yuv)
    cv2.imshow("img",img)
    
    if key==27:
        break

    
camera.release()
cv2.destroyAllWindows()

