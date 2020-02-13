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
    Step,
    StepSpeed,
    GetPosition,
    SetPosition,
    PowerOff
} CMDS;

class motor
{
private:
    //bool Reverse;
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
        // Reverse = 0;
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
    char CMD(uint8_t cmd, uint16_t val)
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
            PORTB = 0;
            break;
        case SetPosition:
            steps = val - position;
            break;
        case GetPosition:
            return position;
        default:
            break;
        }
        return 0;
    }
};
