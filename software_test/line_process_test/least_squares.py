import cv2
import numpy as np
import math
import serial
import time


data = np.zeros(30)

if __name__ == '__main__':
    ser = serial.Serial('COM4',115200,timeout=None)
    theta = np.zeros(30)
    r = 200
    x = np.zeros(30)
    y = np.zeros(30)

    for i in range(0,30):
        theta[i] = 2 * math.pi / 30* i
        x[i] = 250 + r*math.sin(theta[i])
        y[i] = 250 - r*math.cos(theta[i])

    try:
        while True:
            raw_data = ser.readline()[:59].decode()#read serial
            print(type(raw_data))
            data_list = raw_data.split(',')
            for i in range(30):
                data[i] = int(data_list[i])

            #最小二乗法
            sigma_x = sum(x[:] * data[:])
            sigma_x2 = sum((x[:] * data[:])*(x[:] * data[:]))
            sigma_y = sum(y[:] * data[:])
            sigma_xy = sum(x[:] * y[:] * data[:])
            a = (sigma_xy - sigma_x*sigma_y/sum(data[:]))/(sigma_x2 -sigma_x*sigma_x/sum(data[:]))
            b = sum(data[:]*y[:] - a*data[:]*x[:])/sum(data[:])
            epsilon = sum((a*data[:]*x[:] + b - data[:]*y[:]) * (a*data[:]*x[:] + b - data[:]*y[:]))
            print(sigma_x2)
            print("-------")

            img = np.full((500, 500, 3), 64, dtype=np.uint8)


            #センサの描画
            for i in range(0,30):
                if (data[i] == 1):
                    cv2.circle(img, (int(x[i]), int(y[i])), 5, (255, 255, 255), thickness=-1)
                    print(x[i]*x[i])
                else:
                    cv2.circle(img, (int(x[i]), int(y[i])), 5, (255, 255, 255), thickness=1)


            #直線の絵画
            if sum(data) != 0:
                cv2.line(img,
                    pt1=(0, int(b)),
                    pt2=(500, int(a*500+b)),
                    color=(0, 255, 0),
                    thickness=3,
                    lineType=cv2.LINE_4,
                    shift=0)


            cv2.imshow("window",img)
            time.sleep(0.1)
    except KeyboardInterrupt:
        cv2.waitKey(-1)
        ser.close()