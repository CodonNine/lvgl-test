#include "stdint.h"
#include "uitilis.h"
#include "svpwm.h"
#include "math.h"
#include "stdio.h"

int8_t signT1_table[6][2] =
    {
        {1, -1},
        {1, 1},
        {0, 2},
        {-1, 1},
        {-1, -1},
        {0, -2}};

int8_t signT2_table[6][2] =
    {
        {0, 2},
        {-1, 1},
        {-1, -1},
        {0, -2},
        {1, -1},
        {1, 1}};

uint8_t Sector_Judge(float Valpha, float Vbeta)
{
    uint8_t a, b, c;
    uint8_t temp;

    if (Vbeta > 0)
        a = 0;
    else
        a = 1;

    if ((Vbeta - SQRT3 * Valpha) > 0)
        b = 0;
    else
        b = 1;

    if ((Vbeta + SQRT3 * Valpha) > 0)
        c = 0;
    else
        c = 1;

    temp = a * 4 + b * 2 + c;
    // printf("%d\n",temp);
    switch (temp)
    {
    case 0:
        return 2;
    case 1:
        return 3;
    case 2:
        return 1;
    case 3:
        return 0;
    case 4:
        return 0;
    case 5:
        return 4;
    case 6:
        return 6;
    case 7:
        return 5;
    }
}

void Cal_Time(Time *time_t, float Valpha, float Vbeta, uint8_t Sector)
{
    float a, b;

    a = SQRT3_2 * Valpha;
    b = 0.5 * Vbeta;

    time_t->t1 = a * signT1_table[Sector - 1][0] + b * signT1_table[Sector - 1][1];
    time_t->t2 = a * signT2_table[Sector - 1][0] + b * signT2_table[Sector - 1][1];
    time_t->t0 = 1 - time_t->t1 - time_t->t2;
}

void Time_balance(Time *time_t, Phase *phase_t, uint8_t Sector)
{
    float a, b, c;
    a = time_t->t1;
    b = time_t->t2;
    c = time_t->t0 / 2;

    switch (Sector)
    {
    case 1:
        phase_t->phaseA = c;
        phase_t->phaseB = a + c;
        phase_t->phaseC = a + b + c;
        break;

    case 2:
        phase_t->phaseA = b + c;
        phase_t->phaseB = c;
        phase_t->phaseC = a + b + c;
        break;

    case 3:
        phase_t->phaseA = a + b + c;
        phase_t->phaseB = c;
        phase_t->phaseC = a + c;
        break;

    case 4:
        phase_t->phaseA = a + b + c;
        phase_t->phaseB = b + c;
        phase_t->phaseC = c;
        break;
    case 5:
        phase_t->phaseA = a + c;
        phase_t->phaseB = a + b + c;
        phase_t->phaseC = c;
        break;
    case 6:
        phase_t->phaseA = c;
        phase_t->phaseB = a + b + c;
        phase_t->phaseC = b + c;
        break;
    }
}

void Inv_Park(float Ud, float Uq, Clark *clark_t, float theta)
{
    clark_t->Valpha = Ud * cos(theta) - Uq * sin(theta);
    clark_t->Vbeta = Ud * sin(theta) + Uq * cos(theta);
}