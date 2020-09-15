#include "io_control.h"
#include "interrupt.h"
#include "config.h"

#include <xc.h>

void initIO(void)
{
    resetIO();
    
    //Digital I/O
    ANSELx = 0x00;
        
    //Init the Interrupt Pin
    
#ifdef ENABLE_OPEN_DRAIN
    //Open Drain, Output
    INT_ODCON = 1;
#endif
    
    //INT pin is always active low (even when Open Drain is disabled)
    INT_TRIS = 0;
    INT_LAT = 1;
}

void resetIO(void)
{
    //Disable the IOC Interrupts
    resetIOC_pins();
    
    //Load Defaults for PORT
    LATx = DEFAULT_LATx;
    TRISx = DEFAULT_TRISx;
    WPUx = DEFAULT_WPUx;
    SLRCONx = DEFAULT_SLRCONx;
    ODCONx = DEFAULT_ODCONx;
    INLVLx = DEFAULT_INLVLx;
}