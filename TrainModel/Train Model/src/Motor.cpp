#include "Motor.hpp"
#include <Arduino.h>

Motor::Motor(int In1pin, int In2pin, int PWMpin, int STBYpin, int maxSpeed = 40)
    : In1(In1pin), In2(In2pin), PWM(PWMpin), Standby(STBYpin), max_speed(maxSpeed)
{
    pinMode(In1, OUTPUT);
    pinMode(In2, OUTPUT);
    pinMode(PWM, OUTPUT);
    pinMode(Standby, OUTPUT);
}    

void Motor::drive(int* speed, DriveMode_ mode = DriveMode_::DriveMode_GradualSpeedIncrease)
{
    digitalWrite(Standby, HIGH);
    Forward();

    switch(mode)
    {
    case DriveMode_::DriveMode_GradualSpeedIncrease:
    {
        if(speed == nullptr)
        return;
        
        if(*speed == max_speed)
        analogWrite(PWM, map(*speed, 0, max_speed, 0, 255));
        else{
        while(*speed != max_speed)
        {
            analogWrite(PWM, map(*speed, 0, max_speed, 0, 255));
            *speed += increment_decrement;
        }
        }
    }break;
    
    case DriveMode_::DriveMode_InstantSpeedIncrease:
    {
        analogWrite(PWM, 255);
    }break;

    default:
    analogWrite(PWM, 0);
    };
}

void Motor::brake(int* speed, BreakMode_ mode = BreakMode_::BreakMode_GradualBreak)
{
    Forward();
    switch(mode)
    {
    case BreakMode_::BreakMode_GradualBreak:
    {
        if(speed == nullptr)
        return;
        
        while(*speed != 0 || *speed > 0)
        {
        if(*speed < 0) //prevents negative value going to the PWM
            break;

        analogWrite(PWM, map(*speed, 0, max_speed, 0, 255));
        *speed -= increment_decrement;
        }
    }break;
    
    case BreakMode_::BreakMode_InstantBreak:
    analogWrite(PWM, 0);
    };
}

void Motor::standby()
{
    digitalWrite(Standby, LOW);
}

void Motor::Forward()
{
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
}