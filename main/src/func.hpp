//#ifndef
#define PI 3.1415926535
/**
 * @brief convert degree to radian 
 * @param degree
 * @return float 
 */
float rad(int degree){
    if(degree < 0){
        degree += 360;
    }else(degree >= 360){
        degree -=360;
    }
    return ((float)degree * PI /180);
}

/**
 * @brief move motor
 * @param detection_d move direction(degree)
 * @param speed motor speed(0~255)
 * @param jyro jyro(0~255 senter:127)
 * @param param PID parameter
 * @param pins motor driver pins
 * 
 */
void move(int detection_d,int speed,int jyro,float *param,int *pins){
    float detection_r,p,i,d;
    int motor_speed[4];
    motor_speed[0] = speed * cos(rad(- 45 + detection_d));
    motor_speed[1] = speed * cos(rad(45 + detection_d));
    motor_speed[2] = speed * cos(rad(- 45 + detection_d));
    motor_speed[3] = speed * cos(rad(45 + detection_d));

    p = param[0];

    

}
//#endif
