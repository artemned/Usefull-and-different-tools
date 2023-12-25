#include "I2C_Master.h"

// enum error_status
//{
//	START = 0x08,
//	SLA_W = 0x18,
//	BYTE_WRITE = 0x28,
//	SLA_R = 0x40,
//	BYTE_READ = 0x50
//
//}err = START;

I2C_Master::I2C_Master() //initial I2C interface	
{
  set_pins();//?need or not
  TWBR = BIT_RATE_VALUE;
  CONTROL_INTERFACE |= _BV(TWEN);
  power_reduce(); //This is method does not work on platform Arduino
} //I2C_Master

void I2C_Master::set_pins(void)
{
  clear_bit(DIRECTION_PORT_C,SDA); // Set up as input
  set_bit(DATA_REGISTER,SDA);    // Switch on pull up resistor
  clear_bit(DIRECTION_PORT_C,SCL); // Set up as input
  set_bit(DATA_REGISTER,SCL);    // Switch on pull up resistor
}

void I2C_Master::power_reduce(void)
{
 //set_bit(ANALOG_COMPARE,ADC); //Switch off analog comparator
 ANALOG_COMPARE |= _BV(ADC);
 POWER_REDUCTION |= (_BV(ADC) | _BV(PRTIM0)
 | _BV(PRTIM1)| _BV(PRTIM2)
 | _BV(PRSPI) ) ; //disable core part`s

}

void I2C_Master::wait_for_complet(void)
{
  loop_until_bit_is_set(CONTROL_INTERFACE,TWINT);
}

// default destructor
I2C_Master::~I2C_Master()
{
	CONTROL_INTERFACE &= _BV(TWEN); //disable TWI operation 
} //~I2C_Master


////////////////public method`s//////////////////////////////////////////////

void I2C_Master::initial_start(void)
{

  //CONTROL_INTERFACE = (_BV(TWINT) | _BV(TWSTA) | _BV(TWEN));
  set_bit(CONTROL_INTERFACE,TWINT); //set interrupt bit
  set_bit(CONTROL_INTERFACE,TWSTA); //set start bit
  set_bit(CONTROL_INTERFACE,TWEN);  //set enable bit
  wait_for_complet();
// if((TWSR & 0xF8) != (uint8_t)err) {};
//	  err = error_status::SLA_W;

}

void I2C_Master::_restart(void)
{
  initial_start();
}

void I2C_Master::initial_stop(void)
{
 CONTROL_INTERFACE = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
}

uint8_t I2C_Master::read_byte(void)
{
  CONTROL_INTERFACE = (_BV(TWINT)|_BV(TWEA)|_BV(TWEN));
  //set_bit(CONTROL_INTERFACE,TWINT);
  //set_bit(CONTROL_INTERFACE,TWEA);
  //set_bit(CONTROL_INTERFACE,TWEN);
  //i2c_wait();
  wait_for_complet();
  return (DATA_REGISTER);
}

uint8_t I2C_Master::read_byte_NACK(void)
{
  CONTROL_INTERFACE = (_BV(TWINT)| _BV(TWEN));
  //set_bit(CONTROL_INTERFACE,TWINT);
  //set_bit(CONTROL_INTERFACE,TWEN);
  wait_for_complet();
  return (DATA_REGISTER);  
}

void I2C_Master::send_data(const uint8_t& data)
{
  DATA_REGISTER = data;
  CONTROL_INTERFACE = (_BV(TWEN) | _BV(TWINT));
  wait_for_complet();
}
