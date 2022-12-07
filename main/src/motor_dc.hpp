#ifndef MOTOR_HPP_INCLUDE
#define MOTOR_HPP_INCLUDE

class MOTOR {
  public:
    MOTOR(Stream *A,Stream *B);//constructor
    ~MOTOR();//destructor

    void free();
    void move(float direction_r,int speed,int gyro);
  private:
    int a;
    Stream *motorA;
    Stream *motorB;
};

#endif