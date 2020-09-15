#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

    
//Number of iterations to run the full-strength I/O driver for (when switching from 0 to 1).
//This number is an approximate value due to the overhead associated with loops.
//Valid number range: 0 to 255
#define DELAY_COUNT 10

//I2C Parameters    
#define I2C_BASE_ADDRESS 0x60
#define _I2C_BASE_ADDR_VALUE (I2C_BASE_ADDRESS << 1)
    
//Comment out this define to disable address select lines for I2C    
#define ENABLE_ADDR_LINES
    
#ifdef ENABLE_ADDR_LINES
    
#define I2C_ADDR_PIN0_ANSEL     ANSELAbits.ANSA0
#define I2C_ADDR_PIN1_ANSEL     ANSELAbits.ANSA1
#define I2C_ADDR_PIN2_ANSEL     ANSELAbits.ANSA2
    
#define I2C_ADDR_PIN0_DIR       TRISAbits.TRISA0
#define I2C_ADDR_PIN1_DIR       TRISAbits.TRISA1
#define I2C_ADDR_PIN2_DIR       TRISAbits.TRISA2
    
#define I2C_ADDR_PIN0           PORTAbits.RA0
#define I2C_ADDR_PIN1           PORTAbits.RA1
#define I2C_ADDR_PIN2           PORTAbits.RA2
#endif

//Controlled PORT
    
//PORT 1
#define PORTx           PORTC
#define LATx            LATC
#define TRISx           TRISC
#define ANSELx          ANSELC
#define ODCONx          ODCONC
#define SLRCONx         SLRCONC
#define INLVLx          INLVLC
#define WPUx            WPUC
    
//PORT 1 IOC Parameters
#define IOCxF           IOCCF
#define IOCxP           IOCCP
#define IOCxN           IOCCN
    
//Default Settings for PORT 1
//Per this example - LAT should always be 0, SLRCON should be 0 (max slew)
//And WPU should be enabled. (Turned off when in output)
#define DEFAULT_TRISx       0xFF
#define DEFAULT_LATx        0x00
#define DEFAULT_WPUx        0xFF
#define DEFAULT_INLVLx      0x00
#define DEFAULT_ODCONx      0x00
#define DEFAULT_SLRCONx     0x00
    
//INT Pin Settings
    
//Comment out this line to disable OPEN DRAIN for INT PIN
#define ENABLE_OPEN_DRAIN
    
#define INT_LAT     LATA4
#define INT_TRIS    TRISA4
#define INT_ODCON   ODCA4


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIGURATION_H */

