//#ifndef
#include <Arduino.h>


//LINEPIN set_linepin();

typedef struct ledpin
{
    int line_state;
    int cam_state;
    int gyro_state;
    int IR_state;
    int gyro_L;
    int gyro_R;
}LEDPIN;

//LEDPIN set_ledpins();

/**
* @brief get IR value
* @param *serial Serial class which using IR
* @param *detection_r return detection(radian)
* @param *distance return distance(0~15)
*/
void get_IR(Stream *serial,float *detection_r,int *distance){
    uint8_t data;
    if(serial->available()){
        data = serial->read();
        //Serial.println(data);
        *detection_r = 2*PI - (data & 0x0F) *PI/8;

        //以下2台目のIR反転用
        *detection_r = 2*PI - *detection_r;

        *distance = (data & 0XF0) >> 4;
    }

}

/**
* @brief get gyro value
* @param *serial Serial class which using gyro
* @return gyro value(0-255)
*/
int get_gyro(Stream *serial,int led_pin){
    static int value = 127;
    if(serial->available()){
        digitalWrite(led_pin,HIGH);
        value = serial->read()+127;
        if(value > 255){
            value -= 256;
        }
        return value;  
    }else{
        digitalWrite(led_pin,LOW);
        return value;
    }  
}

void get_cam(Stream *serial,int *blue){
    int angleb;//angle of blue    0~7:angle(center:0) 8:can't detect
    int angley;//angle of yellow
    char lr;// x = can't detect l = left r = right
    char by;// x = can't detect m = center b = blue y = yellow
    if(serial->available()){
        String input = serial->readStringUntil('\n');
        Serial.println(input);
        angleb = input[0] - '0';
        angley = input[1] - '0';
        lr = input[2];
        by = input[3];
        if(angleb > 3){
            angleb =- 4;
        }else{
            angleb =+ 4;
        }  
        *blue = angleb;
    }
}

//#endif