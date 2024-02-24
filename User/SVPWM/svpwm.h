#include "stdint.h"
#include "uitilis.h"


typedef struct
{
    float t1;
    float t2;
    float t0;
}Time;

typedef struct
{
    float phaseA;
    float phaseB;
    float phaseC;
}Phase;

typedef struct
{
    float Valpha;
    float Vbeta;
}Clark;



uint8_t Sector_Judge(float Valpha, float Vbeta);
void Cal_Time(Time *time_t, float Valpha, float Vbeta, uint8_t Sector);
void Time_balance(Time *time_t, Phase *phase_t, uint8_t Sector);
void Inv_Park(float Ud,float Uq,Clark* clark_t,float theta);