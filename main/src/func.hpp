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
float degree_to_rad(int degree){
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


    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(pins.ICpin1[j], mux_channel[i][j]);
        }
        value[13 - i] = analogRead(pins.Apin1) > threshold;
        //value[i] = analogRead(pins.Apin1) > threshold;
        sum += value[13 - i];
        //sum += value[i];
    }
    for (int i = 14; i < 30; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(pins.ICpin2[j], mux_channel[i - 16][j]);
        }
        value[43-i] = analogRead(pins.Apin2) > threshold;
        //sum += value[i];
        int tmp;
        /*tmp = value[29];
        value[29] = value[14];
        value[14] = tmp;*/
        sum += value[43 - i];
        
    }
    return (sum > 0);
}
//反応しているセンサがある前提
void cal_line_direction(int data[], float *angle, float *distance) {
    //座標の指定
    double x[30];
    double y[30];

    int i, k;
    int s = 0;
    int tmp;
    int count1, count2;
    int lightcount = 0;
    int state[30];
    double a, b, c;
    // x[0]y[0]（各センサの座標をとる）
    // x[0]y[0] を PT1 に対応させている。

    // 2022-12-06:基準となるx軸を原点とPT1のセンサがなす直線にした。
    for (i = 0; i < 30; i++) {
        x[i] = 47 * cos(degree_to_rad(12 * i));
        y[i] = 47 * sin(degree_to_rad(12 * i));
    }

    //しきい値を満たすセンサの取得
    for (i = 0; i < 30; i++) {
        if (data[i]) {
            state[i] = 1;
            lightcount++;
        } else {
            state[i] = 0;
        }
        // Serial.print(lightcount);
        // Serial.print(',');
        // Serial.println(state[i]);
    }

    //labeling
    /*int labelnum = 0;
    if(state[0] == 1){
        labelnum = 1;
    }
    state[0] *= labelnum;
    for (int i = 1; i < 30; i++){
        if((state[i] - state[i-1]) > 0){//微分
            labelnum++;
        }
        state[i] *= labelnum;
    }
    if((state[0] * state[29]) > 0){//0と29が両方反応している
        for(int i = 29; i > -1; i--){
            if(state[i] != 0){
                state[i] = 1;
            }else{
                labelnum --;
                break;
            }
        }
    }
    
    Serial.println(labelnum);
    */
    //反応したブロックの数　
    

    if(lightcount > 1){
        for (i = 0; i < 30; i++) {
            if (state[i] == 1) {
               count1 = i;
               break;
            }
        }

        for (i = 29; i > -1; i--) {
            if ((state[i] == 1) && (i != count1)) {
               count2 = i;
               break;
            }
        }

        // y = ax + b -> a -1 b
        // y = -1/a * x   tan(theta) = -1/a c = -1/a
        // theta = arctan(c);
        //Serial.print(count1);
        //Serial.print(",");
        //Serial.print(count2);
        a = (y[count1] - y[count2]) / (x[count1] - x[count2]);
        b = y[count1] - a * x[count1];
        *distance = (fabs(b) / sqrt(a * a + 1));
        *angle = (float)atan2((x[count1] + x[count2])/2,(y[count1] + y[count2])/2);
        //Serial.print(",");
        
    }else if(lightcount == 1){//ここｱ正常
        for (int i = 0; i < 30; i++){
            if(state[i]){
                count1 = i;
                break;
            }
        }

        *distance = 47;
        *angle = (float)atan2(x[count1],y[count1]);
    }else{
        return;
    }

    *angle += PI;
    *angle -= PI/2;
    if(*angle < 0){
        *angle = 2*PI - *angle;
    }
    if(*angle > 2*PI){
        *angle -= 2*PI;
    }
    *angle = 2*PI -*angle;
}
//#endif
