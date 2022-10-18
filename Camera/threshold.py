import cv2
from matplotlib import image
import numpy as np
from PIL import Image, ImageTk
import tkinter as tk
import tkinter.ttk as ttk

def binarization(val):
    global var_A_max,var_A_min,var_B_max,var_B_min,var_L_max,var_L_min,image_rgb,canvas1,b_image_tk,textBox
    L_max = var_L_max.get()
    L_min = var_L_min.get()

    A_max = var_A_max.get()
    A_min = var_A_min.get()

    B_max = var_B_max.get()
    B_min = var_B_min.get()
    img_lab = cv2.cvtColor(image_rgb,cv2.COLOR_RGB2LAB)
    b_img = cv2.inRange(img_lab,np.array([L_min,A_min,B_min]),np.array([L_max,A_max,B_max]))
    b_image_pil = Image.fromarray(b_img)
    b_image_tk = ImageTk.PhotoImage(b_image_pil)
    canvas1.delete('All')
    canvas1.create_image(0,0,image=b_image_tk,anchor=tk.NW)#set image on canvas
    value = "{0},{1},{2},{3},{4},{5}".format(L_min,A_min,B_min,L_max,A_max,B_max)
    textBox.delete(0, tk.END)
    textBox.insert(tk.END,value)


window = tk.Tk()

cap = cv2.VideoCapture(0)
cap.set(3,960)
cap.set(4,720)
img = cap.read()[1]#cv2.imread("image1.png")#read image
#--conversion image format--
image_rgb = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
image_rgb = cv2.resize(image_rgb,(480,360))
image_pil = Image.fromarray(image_rgb)
image_tk = ImageTk.PhotoImage(image_pil)

window.title("thereshold editor")
window.geometry('960x500+0+0')
#window.state('zoomed')#full screen
canvas0 = tk.Canvas(window, bg="white", height=360,width=480)
canvas0.place(x=0,y=0)
canvas0.create_image(0,0,image=image_tk,anchor=tk.NW)#set image on canvas
canvas1 = tk.Canvas(window, bg="white", height=360,width=480)
canvas1.place(x=480,y=0)
canvas1.create_image(0,0,image=image_tk,anchor=tk.NW)#set image on canvas

var_L_max = tk.IntVar(window)
var_L_min = tk.IntVar(window)


var_A_max = tk.IntVar(window)
var_A_min = tk.IntVar(window)

var_B_max = tk.IntVar(window)
var_B_min = tk.IntVar(window)

scaleL_max = tk.Scale(window,variable=var_L_max,orient="horizontal",showvalue=False,length=640,from_=0,to=255,command=binarization)
scaleL_min = tk.Scale(window,variable=var_L_min,orient="horizontal",showvalue=False,length=640,from_=0,to=255,command=binarization)
scaleA_max = tk.Scale(window,variable=var_A_max,orient="horizontal",showvalue=False,length=640,from_=0,to=255,command=binarization)
scaleA_min = tk.Scale(window,variable=var_A_min,orient="horizontal",showvalue=False,length=640,from_=0,to=255,command=binarization)
scaleB_max = tk.Scale(window,variable=var_B_max,orient="horizontal",showvalue=False,length=640,from_=0,to=255,command=binarization)
scaleB_min = tk.Scale(window,variable=var_B_min,orient="horizontal",showvalue=False,length=640,from_=0,to=255,command=binarization)

scaleL_max.set(255)
scaleA_max.set(255)
scaleB_max.set(255)

scaleL_max.place(x=160,y=360)
scaleL_min.place(x=160,y=380)

scaleA_max.place(x=160,y=400)
scaleA_min.place(x=160,y=420)

scaleB_max.place(x=160,y=440)
scaleB_min.place(x=160,y=460)

label1 = tk.Label(text='L max')
label1.place(x=800, y=360)

label2 = tk.Label(text='A max')
label2.place(x=800, y=400)

label3 = tk.Label(text='B max')
label3.place(x=800, y=440)

label4 = tk.Label(text='L min')
label4.place(x=120, y=380)

label5 = tk.Label(text='A min')
label5.place(x=120, y=420)

label6 = tk.Label(text='B min')
label6.place(x=120, y=460)

textBox = tk.Entry(width=40)
textBox.place(x=120, y=480)

#scaleL.pack()

window.mainloop()
"""
while True:
    #cap.read()[1]
    if cv2.waitKey(1) & 0xFF == ord('s'):
        break
"""