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
/**
 * @brief move motor
 * @param direction_r move direction(radian)
 * @param speed motor speed(0~255)
 * @param gyro jyro(0~255 senter:127)
 * @param pins motor driver pins
 */
void move(float direction_r,int speed,int gyro,Stream *serial[]){
    int p = 2;//比例定数
    gyro -= 127;
    float motor_speed[4];
    int speed_sign[4];
    motor_speed[0] = speed * cos(- (PI/4) + direction_r);
    motor_speed[1] = speed * cos((PI/4) + direction_r);
    motor_speed[2] = speed * cos(- (PI/4) + direction_r);
    motor_speed[3] = speed * cos((PI/4) + direction_r);
    
    
    motor_speed[0] += p*gyro;
    motor_speed[1] += p*gyro;
    motor_speed[2] -= p*gyro;
    motor_speed[3] -= p*gyro;

    //normalize and Serial send
    int max = 0;
    for(int i = 0; i < 4; i++){
        if( 0 <= motor_speed[i]){
            speed_sign[i] = 1;
        }else{
            speed_sign[i] = -1;
            motor_speed[i] = abs(motor_speed[i]);
        }
        if(max < motor_speed[i]){
            max = motor_speed[i];
        }
    }
    for (int i = 0; i < 4; i++){
        motor_speed[i] = speed_sign[i] * map(motor_speed[i],0,max,0,100);
        serial[i]->write(int8_t(motor_speed[i]));
    } 
}

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
    }
    *detection_r = (data & 0x0F) *PI/8;
    *distance = (data & 0XF0) >> 4;
}

/*
* @brief get gyro value
* @param *serial Serial class which using gyro
* @return gyro value(0-255)
*/
int get_gyro(Stream *serial){
    if(serial->available()){
        return serial->read();
    }    
}

bool get_line(LINEPIN pins,int value[],int threshold){
    int sum = 0;
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
        value[i] = (analogRead(pins.Apin1) < threshold);
        sum += value[i];
    }
    for (int i = 16; i < 30; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(pins.ICpin2[j], mux_channel[i - 16][j]);
        }
        value[i] = (analogRead(pins.Apin2) < threshold);
        sum += value[i];
    }
    return (sum > 0);
}

float cal_line_direction(int values[]){
    
}
//#endif
