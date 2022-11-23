import cv2

def nichika(img,lower,upper):
    hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    nichika=cv2.inRange(hsv,lower,upper)
    
    return nichika


camera=cv2.VideoCapture(0)
hl,hu=0,255
sl,su=0,255
vl,vu=0,255

while True:
    ret,img=camera.read()
    if not ret:
        break
    
    key=cv2.waitKey(1)
    if key==97:
        hl+=1
        print(hl,sl,vl,hu,su,vu)
    if key==122:
        hl-=1
        print(hl,sl,vl,hu,su,vu)
    if key==115:
        sl+=1
        print(hl,sl,vl,hu,su,vu)
    if key==120:
        sl-=1
        print(hl,sl,vl,hu,su,vu)
    if key==100:
        vl+=1
        print(hl,sl,vl,hu,su,vu)
    if key==99:
        vl-=1
        print(hl,sl,vl,hu,su,vu)
    if key==103:
        hu+=1
        print(hl,sl,vl,hu,su,vu)
    if key==98:
        hu-=1
        print(hl,sl,vl,hu,su,vu)
    if key==104:
        su+=1
        print(hl,sl,vl,hu,su,vu)
    if key==110:
        su-=1
        print(hl,sl,vl,hu,su,vu)
    if key==106:
        vu+=1
        print(hl,sl,vl,hu,su,vu)
    if key==109:
        vu-=1
        print(hl,sl,vl,hu,su,vu)
    
    
    cv2.imshow("camera",nichika(img,(hl,sl,vl),(hu,su,vu)))
    
    if key==27:
        break
    
camera.release()
cv2.destroyAllWindows()

