#ifndef LINE_HPP_INCLUDE
#define LINE_HPP_INCLUDE

class LINE {
  public:
    LINE();//constructor
    ~LINE();//destructor

    void set_pin(int _ICpin1[], int _Apin1, int _ICpin2[],int _Apin2);
    
  private:
    float degree_to_rad(int degree);
    
    int ICpin1[4];
    int ICpin2[4];
    int Apin1;
    int Apin2;
};

#endif