//#ifndef
#include <Arduino.h>

struct LINEPIN{
    int ICpin1[4];
    int ICpin2[4];
    int Apin1;
    int Apin2;
};
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
 * @brief convert degree to radian 
 * @param degree
 * @return float 
 */
float rad(int degree){
    if(degree < 0){
        degree += 360;
    }
    else if(degree >= 360){
        degree -=360;
    }
    return ((float)degree * PI /180);
}



/**
 * @brief move motor
 * @param direction_d move direction(radian)
 * @param speed motor speed(0~255)
 * @param jyro jyro(0~255 senter:127)
 * @param param PID parameter
 * @param pins motor driver pins
 * 
void move(float detection_r,int speed,int gyro,float param[],int pins[]){
    float detection_r,p,i,d;
    float motor_speed[4];
    motor_speed[0] = speed * cos(- (PI/4) + detection_r);
    motor_speed[1] = speed * cos((PI/4) + detection_r);
    motor_speed[2] = speed * cos(- (PI/4) + detection_r);
    motor_speed[3] = speed * cos((PI/4) + detection_r);

    p = param[0];
    i = param[1];
    d = param[2];
    

}
*/

/*
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
    }
    *detection_r = 2*PI - (data & 0x0F) *PI/8;
    *distance = (data & 0XF0) >> 4;
}

/*
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

bool get_line(LINEPIN pins,int value[],int threshold){
    int sum = 0;
    int value_a[30];
    uint8_t mux_channel[16][4] = {
        {0, 0, 0, 0},  // 0
        {1, 0, 0, 0},  // 1
        {0, 1, 0, 0},  // 2
        {1, 1, 0, 0},  // 3
        {0, 0, 1, 0},  // 4
        {1, 0, 1, 0},  // 5
        {0, 1, 1, 0},  // 6
        {1, 1, 1, 0},  // 7
        {0, 0, 0, 1},  // 8
        {1, 0, 0, 1},  // 9
        {0, 1, 0, 1},  // 10
        {1, 1, 0, 1},  // 11
        {0, 0, 1, 1},  // 12
        {1, 0, 1, 1},  // 13
        {0, 1, 1, 1},  // 14
        {1, 1, 1, 1}   // 15
    };
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(pins.ICpin1[j], mux_channel[i][j]);
        }
        value_a[i] = analogRead(pins.Apin1);
        value[i] = (value_a[i] > threshold);
        sum += value[i];
    }
    for (int i = 16; i < 30; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(pins.ICpin2[j], mux_channel[i - 16][j]);
        }
        value_a[i] = analogRead(pins.Apin2);
        value[i] = (value_a[i] > threshold);
        sum += value[i];
    }
    //Serial.println(value_a[0]);
    return (sum > 0);
}

float cal_line_direction(int values[]){
    
}
//#endif
