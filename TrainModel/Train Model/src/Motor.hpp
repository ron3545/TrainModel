#pragma once

class Motor
{
  public:
    enum class DriveMode_
    {
      DriveMode_GradualSpeedIncrease,
      DriveMode_InstantSpeedIncrease
    };

    enum class BreakMode_
    {
      BreakMode_GradualBreak,
      BreakMode_InstantBreak
    };

    Motor(int In1pin, int In2pin, int PWMpin, int STBYpin, int maxSpeed = 40);  
    void drive(int* speed, DriveMode_ mode = DriveMode_::DriveMode_GradualSpeedIncrease);
    void brake(int* speed, BreakMode_ mode = BreakMode_::BreakMode_GradualBreak);
    void standby();
  private:
    int In1, In2, PWM,Standby, max_speed;
    const int increment_decrement = 10;

    void Forward();
};