#include "Motor.hpp"
#include <Arduino.h>

Motor::Motor(int In1pin, int In2pin, int PWMpin, int STBYpin, int maxSpeed, bool GradualMode)
    : In1(In1pin), In2(In2pin), PWM(PWMpin), Standby(STBYpin), max_speed(maxSpeed), Isgradual(GradualMode)
{
    pinMode(In1, OUTPUT);
    pinMode(In2, OUTPUT);
    pinMode(PWM, OUTPUT);
    pinMode(Standby, OUTPUT);
}    

void Motor::drive(int* speed)
{
    digitalWrite(Standby, HIGH);
    Forward();
    analogWrite(PWM, 120);
    // if(Isgradual)
    // {
    //     if(*speed == max_speed)
    //     analogWrite(PWM, map(*speed, 0, max_speed, 0, 120));
    //     else{
    //         while(*speed != max_speed)
    //         {
    //             analogWrite(PWM, map(*speed, 0, max_speed, 0, 120));
    //             *speed += increment_decrement;
    //         }
    //     }
    // }
    // else
    //     analogWrite(PWM, 120);
    
}

void Motor::brake(int* speed)
{
    Forward();
    
    if(Isgradual)
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
    }
    else
        analogWrite(PWM, 0);
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