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


#define  nop()  __asm__ __volatile__ ("nop")



uint8_t readPIN(uint8_t pin)
{
	return !!(READ_PIN_B & _BV(pin));
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
	PRR |= (_BV(PRADC) | _BV(PRTIM1)|_BV(PRTIM0));// Power reduction ADC,TIMER1,TIMER0
}


class MAX6675_SPI
{
	public:
	MAX6675_SPI()
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
	
	struct MAX6675_Frame
	{
		int8_t device_size = 1; //count cycle for DEVICE state
		int8_t data_size = 11; //count cycle for DATA_BITS state
		
		enum State // state for reading with SPI from MAX6675 device
		{
			DUMMY_BIT = 0,
			DATA_BITS,
			THERMO_IN, // for error state
			DEVICE_ID,
		}state_frame = DUMMY_BIT; // using 15 bits
		
	}Frame;
	
};//class
	
	
 float MAX6675_SPI::read_temp(void)
{
		 uint16_t value = 0;
		 uint8_t error_tc = 0;
		 float temp = 0.0;
    /*
      Bring CS pin low to allow us to read the data from
      the conversion process
    */
    Frame.state_frame = Frame.State::DUMMY_BIT;
	digitalOFF(_CS_pin); //initial spi 
   
	//asm("nop");
    
		switch(Frame.state_frame)
		{
		 case Frame.State::DUMMY_BIT:
		 {
			 Frame.state_frame = Frame.State::DATA_BITS;
			 // Cycle the clock for dummy bit 15 
			 digitalON(_SCK_pin);
			  nop();//delay about 0.1
			  nop();
			  nop();
	 		// _delay_ms(1);
			 digitalOFF(_SCK_pin);			 
		 }
		 
		case Frame.State::DATA_BITS:
		{
		  Frame.state_frame = Frame.State::THERMO_IN;
		  /*
             Read bits 14-3 from MAX6675 for the Temp. Loop for each bit reading
             the value and storing the final value in 'temp'*/                                                                                  
           for(int i = Frame.data_size; i >= 0; i --)
		   {
               digitalON(_SCK_pin);
               value += readPIN(_SO_pin) << i;
               digitalOFF(_SCK_pin);
           }
		 }
		
		case Frame.State::THERMO_IN:
		{
		 Frame.state_frame = Frame.State::DEVICE_ID;
		 /* Read the TC Input inp to check for TC Errors */
		 digitalON(_SCK_pin);
		 error_tc = readPIN(_SO_pin);
		 digitalOFF(_SCK_pin);		 
		}
		
		case Frame.State::DEVICE_ID:
		{
         /*
          Read the last two bits from the chip, faliure to do so will result
          in erratic readings from the chip.*/
        
          for (int i=Frame.data_size; i>=0; i--)
		  {
              digitalON(_SCK_pin);
               nop();
			   nop();
			   nop();
			   nop();
              digitalOFF(_SCK_pin);
          }
		}
		default: break;
		
	}//switch
		
		// Disable Device
		nop();
		digitalON(_CS_pin);
		
		temp = (value*0.25);
		
		if(temp<=100.00)
		{
			temp = (temp * NON_LINEARITY_COEFFICIENT);
		}

		// Output negative of CS_pin if there is a TC error, otherwise return 'temp' 
		if(error_tc != 0) {
			return -_CS_pin;
			} else {
			return temp;
		}
				
}

	


