#include "i2c_fsm.h"
#include "i2c_slave.h"
#include "io_control.h"
#include "interrupt.h"
#include "config.h"

#include <xc.h>

typedef union
{
    struct{
        unsigned STARTED : 1;
        unsigned STATE : 2;
    };
    uint8_t field;
} I2C_Status;

static volatile I2C_Status status;
static volatile uint8_t bCount;
static volatile uint8_t addr;

void init_I2C_FSM(void)
{
    status.field = 0x00;
    bCount = 0x00;
    addr = 0x00;
}


void handle_I2C_ISR(void)
{        
    //Normally ACK
    SSP1CON2bits.ACKDT = 0;
    
    if (SSP1STATbits.P)
    {        
        //Stop Interrupt
        bCount = 0;
        
        status.field = 0x00;

        //Release the Interrupt Line (if asserted)
        release_INT();
    }
    else if ((status.STATE == 1) && (!SSP1CON3bits.ACKTIM) && (!SSP1STATbits.RW))
    {
        //When being written to, ignore the 2nd interrupt after ACK
        status.STATE = 0;
    }
    else if ((SSP1STATbits.S) && (!SSP1STATbits.BF) && ((!SSP1CON3bits.ACKTIM) && (status.STATE == 0)))
    {
        //Start is enabled, no buffered data, no ACK/NACK yet, and no STOP
                
        //Start Condition
        status.STARTED = 1;
        bCount = 0;
    }
    else if (SSP1STATbits.BF)
    {
        status.STATE = 1;
        //Clear any former errors
        
        
        uint8_t rx = SSP1BUF;
        //Buffer Full
        if (bCount == 0)
        {            
            SSP1CON2bits.ACKDT = 0; //NACK if an error occurred 
        }
        else if (!SSP1STATbits.RW)
        {
            //Writes
            if (bCount == 1)
            {
                if (DELAY_COUNT != 0)
                {
                    //Sets the output of the I/O port into the delay functions
                    uint8_t count = 0;
                    //Apply I/O Port direction changes
                    if (DELAY_COUNT != 0)
                    {
                        //Sets the output of the I/O port
                        LATx = rx;

                        do
                        {
                        //Wait...
                            asm("NOP");
                            count++;
                        } while (count != DELAY_COUNT);
                    }
                } 
                //Apply I/O Port direction changes  
                TRISx = rx;
                LATx = 0x00;
            }
            SSP1CON2bits.ACKDT = 0;
        }
        
        //Release the clock
        SSP1CON1bits.CKP = 1;
        //Increment the Byte Count
        bCount += 1;
    }
    else if ((status.STARTED == 1) && (SSP1STATbits.RW) && (!SSP1CON2bits.ACKSTAT))
    {
        //Reads
        SSP1BUF = PORTx;
        
        SSP1CON1bits.CKP = 1;
        bCount += 1;
    }
}

uint8_t isActive_n(void)
{
    return SSP1STATbits.P;
}

void reset_I2C_addr(void)
{
    addr = 0x00;
}