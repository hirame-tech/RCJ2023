#ifndef MOTOR_HPP_INCLUDE
#define MOTOR_HPP_INCLUDE

class MOTOR {
  public:
    MOTOR(Stream *A,Stream *B,Stream *C,Stream *D);//constructor
    ~MOTOR();//destructor

    void move(float direction_r,int speed,int gyro);
  private:
    int a;
    Stream *motorA;
    Stream *motorB;
    Stream *motorC;
    Stream *motorD;
};

#endif