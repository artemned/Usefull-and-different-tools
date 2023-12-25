#pragma once
/*
Rp Value of Pull-up resistor if fSCL ? 100 kHz = VCC – 0,4V/3mA 
 1000ns / Cb ?
 
Rp Value of Pull-up resistor if fSCL > 100 kHz = VCC – 0,4V/3mA
300ns / Cb ?

 Cb = capacitance of one bus line in pF
 Rp = Pull-up resistor
 SCL frequency = CPU Clock frequency / 16 + 2(TWBR)*(prescaler).
 TWBR = 72 or 71 for 16MHz = 100 kHz
*/

#include "BitManip.h"
#include <avr/io.h>
#include <avr/delay.h>

#define DIRECTION_PORTS_B DDRB // need for debug
#define DATA_REGISTER_B PORTB
#define DIRECTION_PORT_C DDRC
#define DATA_REGISTER_C PORTC
#define INPUT_PIN PINC
#define POWER_REDUCTION PRR
#define ANALOG_COMPARE ACSR
#define CONTROL_INTERFACE TWCR
#define DATA_REGISTER TWDR
#define STATUS_REGISTER TWSR
#define BIT_RATE_VALUE 80






class I2C_Master
{

public:
	I2C_Master();
	~I2C_Master();
	void initial_start(void);
	void _restart(void);
	void initial_stop(void);
	uint8_t read_byte(void);
	uint8_t read_byte_NACK(void);// This is method for reading last byte from slave device and set line up NACK signal
	void send_data(const uint8_t& data);
private:
	I2C_Master( const I2C_Master &c ) = delete;
	I2C_Master& operator=( const I2C_Master &c ) = delete;
    
    const uint8_t SCL = PC5; // PC5
    const uint8_t SDA = PC4; //PC4
    
    void set_pins(void);
    void power_reduce(void);
    void wait_for_complet(void);
 
}; //I2C_Master

