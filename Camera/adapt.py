import cv2
import numpy as np
import time

camera=cv2.VideoCapture(0)
fps=1000

#ワールド座標の長さの単位は[mm]統一

pi=np.pi

a=7.809 #双曲線パラメータ
b=12.8083218
c=np.sqrt(a**2+b**2)

f=3.29 #カメラ焦点距離

nx=8#格子数：計算機の処理能力に応じて調整
ny=3 
w=720 #w x h=変換画像のサイズ
h=480 
fh=200 #視点と変換画像間の距離
alph=[0,pi/3,2*pi/3] #変換画像の方位角
n=len(alph)
beta=pi/2
gamm=0 #変換画像のロール角
#r=1109.0 #This value depend on w,h,fh,alph,beta,gamm

u=[[],[],[],[]] #変換画像の格子点,以下多次元なのは複数の方位角について値を渡すため
v=[[],[],[],[]]
x=[[],[],[],[]] #入力画像の格子点
y=[[],[],[],[]]

pu=[[],[],[],[]] 
pv=[[],[],[],[]]
px=[[],[],[],[]] 
py=[[],[],[],[]]
prer=[]

adapt=[[],[],[],[]]
frag=[[],[],[],[]] #パノラマ画像の一片

def rolmat(alph): #回転行列を渡す、引数は変換画像の方位角
    #変換画像上の座標を三次元に変換する回転行列
    R=np.matrix([[np.cos(alph),-np.sin(alph),0],[np.sin(alph),np.cos(alph),0],[0,0,1]])*np.matrix([[1,0,0],[0,np.cos(beta),-np.sin(beta)],[0,np.sin(beta),np.cos(beta)]])*np.matrix([[np.cos(gamm),0,np.sin(gamm)],[0,1,0],[-np.sin(gamm),0,np.cos(gamm)]])
    return R

for a in range(0,n): #複数の方位角について値を渡すため
    al=alph[a] #方位角を与える
    R=rolmat(al)
    for i in range(0,nx+1):
        for j in range(0,ny+1):
            pu[a].append(w*i/nx-w/2) #変換画像上の格子点の座標のリスト(ただし画像中心が(0,0))
            pv[a].append(h*j/ny-h/2)
            
            u[a].append(int(pu[a][i*(ny+1)+j]+w/2)) #hidariue(0,0)
            v[a].append(int(-pv[a][i*(ny+1)+j]+h/2))
            xw,yw,zw=R*np.matrix([[pu[a][i*(ny+1)+j]],[pv[a][i*(ny+1)+j]],[-fh]]) #変換画像上の格子点のワールド座標

            px[a].append(int(xw*f*(b**2-c**2)/((b**2+c**2)*zw-2*b*c*np.sqrt(xw**2+yw**2+zw**2))*1000)) #入力画像上の格子点、中心が(0,0)(*1000はint型で精度を上げるため)
            py[a].append(int(yw*f*(b**2-c**2)/((b**2+c**2)*zw-2*b*c*np.sqrt(xw**2+yw**2+zw**2))*1000))
            
            prer.append(np.sqrt(px[a][i*(ny+1)+j]**2+py[a][i*(ny+1)+j]**2))
r=max(prer)
for a in range(0,n):
    for i in range(0,nx+1):
        for j in range(0,ny+1):
            x[a].append(px[a][i*(ny+1)+j]+r/2)
            y[a].append(-py[a][i*(ny+1)+j]+r/2)
            
for a in range(0,n):
    for i in range(0,nx+1):
        for j in range(0,ny+1):
            if (i==0 or i%nx!=0) and (j==0 or j%ny!=0):
                img1=np.array([[x[a][i*(ny+1)+j],y[a][i*(ny+1)+j]], [x[a][i*(ny+1)+j+1],y[a][i*(ny+1)+j+1]], [x[a][i*(ny+1)+j+ny+1],y[a][i*(ny+1)+j+ny+1]],[x[a][i*(ny+1)+j+1+ny+1],y[a][i*(ny+1)+j+1+ny+1]]],dtype=np.float32) #入力画像格子点の四角
                img2=np.array([[u[a][i*(ny+1)+j],v[a][i*(ny+1)+j]], [u[a][i*(ny+1)+j+1],v[a][i*(ny+1)+j+1]], [u[a][i*(ny+1)+j+ny+1],v[a][i*(ny+1)+j+ny+1]],[u[a][i*(ny+1)+j+1+ny+1],v[a][i*(ny+1)+j+1+ny+1]]],dtype=np.float32) #変換画像格子点の四角
                adapt[a].append(cv2.getPerspectiveTransform(img1,img2)) #線形変換のための変換行列
            else:
                adapt[a].append(0) #dummy
                
blank=np.zeros((4,h,w,3),dtype=np.uint8) #黒単色画像の生成、変形画像を貼り付けるためのもの
while True:
    ret,img=camera.read() #実装時に画像の幅方向を双曲面ぴったりにカットする必要あり！！！！！！（リサイズ時に入力画像座標上の双曲面の半径の長さを入力画像の幅のサイズから取得するため）
    hI,wI=img.shape[:2] #入力画像のサイズ
    hd=int((wI-hI)/2) #高さ方向の欠けたピクセル数（片方分)

    sqimg=np.insert(np.insert(img,0,np.zeros((hd,wI,3)),axis=0),hI+hd,np.zeros((hd,wI,3)),axis=0) #上下に枠を付け正方形にした入力画像
    reimg=cv2.resize(sqimg,dsize=(int(2*r),int(2*r))) #幅サイズ指定のリサイズ、入力画像で双曲面の高さ方向は欠けててもok


    for a in range(0,n):
        for i in range(0,nx):
            for j in range(0,ny):
                print(a,i,j,i*(ny+1)+j) 
                temp=cv2.warpPerspective(reimg,adapt[a][i*(ny+1)+j],dsize=(w,h)) #線形変換画像、補完方法flagは処理速度に要相談
                blank[a][v[a][i*(ny+1)+j+1]:v[a][i*(ny+1)+j],u[a][i*(ny+1)+j]:u[a][i*(ny+1)+j+ny+1]]=temp[v[a][i*(ny+1)+j+1]:v[a][i*(ny+1)+j],u[a][i*(ny+1)+j]:u[a][i*(ny+1)+j+ny+1]]

    for a in range(0,n):
        cv2.imshow(str(a),blank[a])
    
    key=cv2.waitKey(int(1000/fps))

    if key==27: #escで終了
        
        break