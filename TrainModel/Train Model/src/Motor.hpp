#pragma once

class Motor
{
  public:
    
    Motor(int In1pin, int In2pin, int PWMpin, int STBYpin, int maxSpeed = 40, bool GradualMode = false);  
    void drive(int* speed);
    void brake(int* speed);
    void standby();
  private:
    int In1, In2, PWM,Standby, max_speed;
    const int increment_decrement = 10;
    bool Isgradual;

    void Forward();
};