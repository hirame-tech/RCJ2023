#ifndef LINE_HPP_INCLUDE
#define LINE_HPP_INCLUDE

class LINE {
  public:
    LINE();//constructor
    ~LINE();//destructor

    void set_pin(int _ICpin1[], int _Apin1, int _ICpin2[],int _Apin2);
    bool get_line(int value[],int threshold);
    void cal_line_direction(int data[], float *angle, float *distance);
  private:
    float degree_to_rad(int degree);
    int rad_to_degree(float rad);
    
    int ICpin1[4];
    int ICpin2[4];
    int Apin1;
    int Apin2;
};

#endif