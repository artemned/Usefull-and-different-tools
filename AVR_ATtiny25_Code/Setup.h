#pragma once

#include "USI.h"
#include "BitManip.h"
#include <avr/io.h>
//#include <util/delay.h>

#define NON_LINEARITY_COEFFICIENT 0.992
#define SIGNAL_PIN PB3

#define DIRECTION_PORTS_B DDRB
#define READ_PIN_B PINB
#define CONFIGURATION_PORTS_B PORTB

uint8_t readPIN(uint8_t pin)
{
	return !!(PINB & _BV(pin));
}

inline void digitalOFF(uint8_t& pin)
{
	CONFIGURATION_PORTS_B &= ~_BV(pin);
}

inline void digitalON(uint8_t& pin)
{
	CONFIGURATION_PORTS_B |= _BV(pin);
}


inline void dischargeSignal(void)
{
	DIRECTION_PORTS_B |= _BV(SIGNAL_PIN); // PIN as OUT
	CONFIGURATION_PORTS_B |= _BV(SIGNAL_PIN); // PIN height state
}

inline void disable_ADC(void)
{
  ADCSRA &= ~_BV(ADEN);// Turn off ADC
  ACSR |= _BV(ACD);// Turn off ADC Comparator
  //PRR |= _BV(ADC); //Turn off ADC core
  PRR |= (_BV(PRADC) | _BV(PRTIM1)|_BV(PRTIM0));// Power reduction ADC,TIMER1,TIMER0
}


class MAX6675
{
	public:
	MAX6675()
	{
	  DIRECTION_PORTS_B &=(~_BV(_SO_pin));// _SO_pin as input
	  DIRECTION_PORTS_B |=(_BV(_CS_pin)| _BV(_SCK_pin));// _SCK_pin and _CS_pin setup as out
	  digitalON(_CS_pin);		
	};
	float read_temp();	
	private:
	uint8_t _CS_pin=4;
	uint8_t _SO_pin=1;
	uint8_t _SCK_pin=2;
	uint8_t chip_read(uint8_t CS_pin, uint8_t &error_tc);
	
};//class	

float MAX6675::read_temp()
{
    uint16_t value = 0;
    uint8_t error_tc = 0;
    float temp = 0.0;

    /*
      Initiate a temperature conversion. According to MAX's tech notes FAQ's
      for the chip, Line going high initiates a conversion, which means, we
      need to clock the chip low to high to initiate the conversion, then wait
      for the conversion to be complete before trying to read the data from
      the chip.
    */
    //digitalOFF(_CS_pin);
    //_delay_ms(2);
    //digitalON(_CS_pin);
    //_delay_ms(220);
	//
    /* Read the chip and return the raw temperature value */

    /*
      Bring CS pin low to allow us to read the data from
      the conversion process
    */
    digitalOFF(_CS_pin);
	_delay_ms(0.1);

    /* Cycle the clock for dummy bit 15 */
    digitalON(_SCK_pin);
    _delay_ms(1);
    digitalOFF(_SCK_pin);

     /*
      Read bits 14-3 from MAX6675 for the Temp. Loop for each bit reading
      the value and storing the final value in 'temp'
    */
    for (int i=11; i>=0; i--) {
        digitalON(_SCK_pin);
        value += readPIN(_SO_pin) << i;
        digitalOFF(_SCK_pin);
    }

    /* Read the TC Input inp to check for TC Errors */
    digitalON(_SCK_pin);
    error_tc = readPIN(_SO_pin);
    digitalOFF(_SCK_pin);

    /*
      Read the last two bits from the chip, faliure to do so will result
      in erratic readings from the chip.
    */
    for (int i=1; i>=0; i--) {
        digitalON(_SCK_pin);
        _delay_ms(1);
        digitalOFF(_SCK_pin);
    }

    // Disable Device
    digitalON(_CS_pin);

    
    temp = (value*0.25);
	if(temp<=100.00)
	{
	  temp = (temp * NON_LINEARITY_COEFFICIENT);	
	}
	
	
	

    /* Output negative of CS_pin if there is a TC error, otherwise return 'temp' */
    if(error_tc != 0) {
        return -_CS_pin;
    } else {
        return temp;
    }
}





