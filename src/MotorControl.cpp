#include <Arduino.h>
//#include <time.h>
//#include <Wire.h>

// void ControlRecieved(CMDS cmd, int16_t value)
// {
//     switch (cmd)
//     {
//     case Step:
//         for (int i = value; i >= 0; i--)
//         {
//             step();
//         }
//         break;
//     }
// }

typedef enum
{
    Step = 1,
    StepSpeed,
    GetPosition,
    SetPosition,
    GetDegPosition,
    SetDegPostition,
    PowerOff
} CMDS;

class motor
{
private:
    //bool Reverse;
    uint8_t StepDegree_Num;
    uint8_t StepDegree_Den;
    uint16_t DegPos;
    volatile int steps;
    unsigned int delay;
    volatile uint8_t *port;
    char position;
    //volatile unsigned long nextTime;
    volatile unsigned long lastTime;

public:
    motor(volatile uint8_t *Wport)
    {
        port = Wport;
        delay = 2500;
        lastTime = micros() + delay;
        position = 0;
        steps = 0;
        return;
        // Reverse = 0;
    }
    motor(volatile uint8_t *Wport, uint8_t stepDegree_Num, uint8_t stepDegree_Den) : motor(Wport)
    {
        StepDegree_Den = stepDegree_Den;
        StepDegree_Num = stepDegree_Num;
        DegPos = 0;
    }
    void update()
    {
        unsigned long t = micros();
        if (min(t, t - delay) > lastTime)
        {
            step();
            lastTime = t;
        }
    }
    void step()
    {
        //B1100 causes port 10 and 11
        if (steps)
        {
            position += steps < 0 ? -1 : 1;
            steps += steps < 0 ? 1 : -1;
            DegPos += (steps < 0 ? -1 : 1) * StepDegree_Num;
            DegPos += DegPos > 360 * StepDegree_Den ? DegPos - (360 * StepDegree_Den) : DegPos;
            switch (position & 3)
            {
            case 0:
                *port = B1001;
                break;
            case 1:
                *port = B1100;
                break;
            case 2:
                *port = B0110;
                break;
            case 3:
                *port = B0011;
            default:
                break;
            }
        }
        else
        {
            *port = 0;
        }
    }
    int CMD(uint8_t cmd, int16_t val)
    {
        switch (cmd)
        {
        case Step:
            steps = val;
            break;
        case StepSpeed:
            delay = val;
            break;
        case PowerOff:
            *port = 0;
            break;
        case SetPosition:
            steps = val - position;
            break;
        case GetPosition:
            return position;
        case GetDegPosition:
            //note that this must be divided by the denominator for actual postition
            //neevermind well just drop the decimal
            return DegPos / 2;
            break;
        case SetDegPostition:
            steps = (DegPos - (val * 2)) / 7;
            break;
        default:
            break;
        }
        return 0;
    }
};
