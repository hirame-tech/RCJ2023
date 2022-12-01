#include <Arduino.h>
#include "motor.hpp"

#define MAX_SPEED 50

MOTOR::MOTOR(Stream *A,Stream *B,Stream *C,Stream *D):motorA(A),motorB(B),motorC(C),motorD(D){
}

MOTOR::~MOTOR(){
    motorA->write(127);
    motorB->write(127);
    motorC->write(127);
    motorD->write(127);
}

void MOTOR::free(){
    motorA->write(127);
    motorB->write(127);
    motorC->write(127);
    motorD->write(127);    
}

/**
 * @brief move motor
 * @param direction_r move direction(radian)
 * @param speed motor speed(0~255)
 * @param gyro jyro(0~255 senter:127)
 */
void MOTOR::move(float direction_r,int speed,int gyro){
    int p = 2;//比例定数
    gyro -= 127;//-127~+128に調整
    float motor_speed[4];//速度管理用
    int speed_sign[4];//正転・逆転管理用
    Stream*serial[] = {motorA,motorB,motorC,motorD};//for文で回したかった
    //進行方向設定
    motor_speed[0] = speed * cos(- (PI/4) + direction_r);
    motor_speed[1] = speed * cos((PI/4) + direction_r);
    motor_speed[2] = speed * cos(- (PI/4) + direction_r);
    motor_speed[3] = speed * cos((PI/4) + direction_r);
    
    //方向修正
    motor_speed[0] += p*gyro;
    motor_speed[1] += p*gyro;
    motor_speed[2] -= p*gyro;
    motor_speed[3] -= p*gyro;

    //速さの最大値の探索
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
    if(max > MAX_SPEED){
        for (int i = 0; i < 4; i++){
            motor_speed[i] = speed_sign[i] * map(motor_speed[i],0,max,0,MAX_SPEED);
        }
    }else{
        for (int i = 0; i < 4; i++){
            motor_speed[i] = speed_sign[i] * motor_speed[i];
        }
    }
    //正規化と送信
    for (int i = 0; i < 4; i++){
        serial[i]->write(int8_t(motor_speed[i])+127);
    } 
    //Serial.println(motor_speed[0]);
}