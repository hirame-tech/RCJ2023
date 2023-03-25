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

void get_cam(Stream *serial,int *yellow,int *blue){
    
}

//#endif