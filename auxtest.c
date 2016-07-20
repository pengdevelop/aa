#include <stdio.h>

int auxClockTicks;

int aux_tick_get()
{  
    return auxClockTicks;
}

void aux_counter()
{
    auxClockTicks ++;
    auxClockTicks %= 0xfffffffd;  
}

int testAuxclk()
{   
    if (sysAuxClkConnect (aux_counter, (int) 0) == ERROR)
    {
        printf( "sysAuxClkConect() failed");
        return 0;
    }
    auxClockTicks = 0;
    sysAuxClkEnable();

}