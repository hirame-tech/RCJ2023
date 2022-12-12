import cv2
camera=cv2.VideoCapture(0)
left=0
up=0
ret,img=camera.read()
bottom,right=img.shape[:2]
bottom1=bottom
right1=right
while True:
	
	ret,img=camera.read()
	img=img[up:bottom,left:right]
    
	h,w=img.shape[:2]
	img=cv2.circle(img,(int(w/2),int(h/2)),4,color=(255,0,0),thickness=1)
    
	cv2.imshow("img",img)
	key=cv2.waitKey()
	print(key)
	if key==122 and left>0:#press z
		left-=1
	if key==120 and left<right:#press x
		left+=1
	if key==110 and right>left:#press n
		right-=1
	if key==109 and right<right1:# press m
		right+=1
	if key==99 and up>0:#press c
		up-=1
	if key==100 and up<bottom:#press d
		up+=1
	if key==98 and bottom>up:#press b
		bottom-=1
	if key==103 and bottom<bottom1:#press g
		bottom+=1
		
	if key==27:
		print("up:bottom,left:right=",up,bottom,left,right)
		break
	
