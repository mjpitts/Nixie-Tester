# Nixie-Tester
In this repo there is the C that runs on the ATmega as well as the schematic and gerber files for the PCB. The primary functions of the tester are as follows: Iterate through the digits of the nixie at various speeds. Manually scroll through the digits of the tube with a BCD thumbwheel. 


# Software Requirements:

### AVR-GCC
### AVRDUDE
### MAKE 

# Hardware Requirements:

### ISP of your choosing
### AVR Microcontroller
### BCD Thumbwheel
### IN-2 Nixie Tube (best found on ebay)
### Indicator Nixie
### High voltage power supply
### L7805CV Voltage Regulator
### K155ID1 Nixie driver
### HV MOSFET
### SPST Switch
### 3 10k ohm resistors and 1 22k ohm resistor
### 2 pin 5mm pin pitch screw terminal

Note I used an ATmega 328 because that is what I had on hand. There are 6 digital inputs and 5 digital outputs for this system.
Any microcontoller that can accommodate this would more than likely work. 
