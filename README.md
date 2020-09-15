<!-- Please do not change this html logo with link -->
<a href="https://www.microchip.com" rel="nofollow"><img src="images/microchip.png" alt="MCHP" width="300"/></a>

# Simple I<sup>2</sup>C I/O Expander with PIC16F15244 Family
This code example demonstrates the use of the PIC16F15244 family of parts to build a simple 8-bit I/O expander that is optimized for I<sup>2</sup>C control.

## Related Documentation

* <a href="https://onlinedocs.microchip.com/pr/GUID-632C0460-65B2-4F1E-8D1A-B760EB793F79-en-US-4/index.html">PIC16F15244 Family Datasheet</a>
* <a href="https://github.com/microchip-pic-avr-examples/pic16f15244-family-advanced-i2c-io-expander">Code Example: Advanced I/O Expander with the PIC16F15244 Family</a>

## Software Used

* <a href="http://www.microchip.com/mplab/mplab-x-ide?utm_source=github&utm_medium=text&utm_campaign=pic152xx&utm_content=MCU8_MMTCha_simpleI2C_IOExpander">MPLAB® IDE 5.40 or newer</a>
* <a href="https://www.microchip.com/mplab/compilers?utm_source=github&utm_medium=text&utm_campaign=pic152xx&utm_content=MCU8_MMTCha_simpleI2C_IOExpander">Microchip XC8 Compiler 2.20 or newer</a>
* <a href="https://packs.download.microchip.com/">PIC16F1xxxx_DFP v1.4.119</a>

## Hardware Used

### Common Components

* <a href="https://www.microchip.com/developmenttools/ProductDetails/PartNO/ADM00559?utm_source=github&utm_medium=text&utm_campaign=pic152xx&utm_content=MCU8_MMTCha_simpleI2C_IOExpander"> I<sup>2</sup>C Master Device (or for easy testing, an MCP2221A USB-UART/I<sup>2</sup>C breakout module, PN: ADM00559)</a>

### With the PIC16F15244 family part on a Curiosity LPC Board

* <a href="https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM164137?utm_source=github&utm_medium=text&utm_campaign=pic152xx&utm_content=MCU8_MMTCha_simpleI2C_IOExpander"> Microchip Low Pin Count Curiosity, PN: DM164137</a><br><br>

**Important:**<br>
Many of the RCx pins are used for other functions on the board. For proper operation on all pins, it is recommended to remove the small jumpers on the back of the board correlating to these features. Soldering pin headers to the nearby unpopulated area will allow 2x1 jumpers to re-enable features such as the LEDs, potentiometer, and touch button for future projects.

### With the Curiosity Nano (PIC16F15244)

* <a href="https://www.microchip.com/developmenttools/ProductDetails/PartNO/EV09Z19A?utm_source=github&utm_medium=text&utm_campaign=pic152xx&utm_content=MCU8_MMTCha_simpleI2C_IOExpander"> Microchip Curiosity Nano, PN: EV09Z19A</a>

**Important:** The debugger UART TX trace may interfere with the I/O Expander in some cases. Cutting this trace (located on the underside of the Nano) will resolve the issue.

### Solution Setup

1. Change the build configuration to the appropriate configuration ending with the part number on the board.<br>
  * If you have a license for a PRO compiler, select the PRO configuration to enable code optimizations.

<img src="images/configurationChange.PNG"><br>

2. Run Build and Clean on the project. (Hammer + Broom on the toolbar).  

## Wiring Setup

<img src="images/schematic.png" width="500px"><br>
*Figure 1 - Electrical Configuration of device. Shown with Address Lines tied to GND.*<br>

On the PIC16-152, the default positions for the pins and ports are:

| Function  | Pin or PORT
| --------- | ----
| SDA       | RB4
| SCL       | RB6
| INT       | RB4
| ADDR0     | RA0
| ADDR1     | RA1
| ADDR2     | RA2
| I/O       | PORTC

*Important! The addressing lines are only sampled on Power-up. If the address needs to be updated at run-time, then a RESET of the microcontroller is required.*

## I<sup>2</sup>C Default settings

**I<sup>2</sup>C base address:** 0x60<br>
**I<sup>2</sup>C speed:** 100kHz

*Note: The I<sup>2</sup>C base address can be changed in config.h*

## Operation
To reduce the amount of data sent, this example has in essence, 2 modes:

* A strong GND output
* A weak Vdd Input

### Strong GND Output

The strong GND output is created from the I/O driver on the device. **The limits of this driver are specified in the datasheet.** Generally, this mode can sink current in the range of mA.

### Weak Vdd Input

The weak Vdd input is created from the internal pull-up resistors on the device. These resistors cannot supply a lot of current, however they are used to define the logic level of each pin when it is open, preventing it from being a random value.

### Transitioning

When moving from GND to Vdd, the parasitic capacitance of the pin and connected circuits causes it to transition slowly.<br>

<img src="images/io_slow.png"><br>

To accelerate the process, the device briefly turns on the high-side I/O driver to help recharge the parasitic capacitance.<br>

<img src="images/io_fast.png"><br>

The number of wait loops that the I/O driver runs for is specified in *config.h*. For simplicity, the I<sup>2</sup>C interrupt is used to block during this time, as the delay is very brief. The constant `DELAY_COUNT` in *config.h* represents the number of iterations.

***For longer delays, it is recommended to use one of the timer peripherals on board to control the transitions.***

<img src="images/noBooster.PNG"><br>
*No Delay (natural rise time)*<br>
<img src="images/10cycle.PNG"><br>
*Delay constant of 10 - Note the change in time base from 20us to 40ns*<br>

For the images above, a 1nF capacitor was attached to the I/O pin. **This is an extreme case to demonstrate the differences in delay time. It is not recommended to directly drive a capacitor with the I/O pin.**

## Communication
The simplified nature of this example means that the I<sup>2</sup>C write and read operations represent the only 2 possible operations.

**In the case of writes**, the 1st data byte sent indicates the direction of each pin. Bytes sent after this are ACK'ed, but not used by the program.

  * For instance, writing the value 0b1000 0101 (0x85) would set Pin 7, 2, and 0 to be inputs with all other pins being outputs.

<img src="images/simpleI2C_write.png"><br>

**In the case of reads**, each byte read is the value in the I/O register PORTx. There is no restriction on the number of read requests.

* For instance, reading the value 0b1001 0010 (0x92) indicates a logic '1' on Pin 7, 4, and 1, while the other pins are at a logic '0'.

<img src="images/simpleI2C_read.png"><br>

### External Interrupts
If any pin changes I/O state, then the INT line is asserted to '0'. This only applies to pins that are assigned as an input. Any I<sup>2</sup>C activity with this device will clear the assertion on the STOP condition.

In *config.h*, the INT line can be configured as Open-Drain or as Push-Pull. In Push-Pull, logic 0 is still used as asserted, and logic 1 is used as not asserted.

## Summary
This code example demonstrates the utility of parts such as the PIC16-152 in ultra-low cost applications where flexibility is key.
