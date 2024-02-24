#include "main.h"
#include "sys.h"
void delay_us(uint16_t nus)
{

    SysTick->LOAD=nus*168;
    SysTick->VAL=0x00;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
    while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(u16 nms)
{
    u32 i;
    for(i=0;i<nms;i++) delay_us(1000);
}
