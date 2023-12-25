#include "AHT10.h"

//////////////// public method`s//////////////////////////////////////////////////

 AHT10::AHT10()
 {
	 //initial_I2C();
	 initialization();
 };
 
 AHT10::AHT10(const uint8_t& address) : device_address(address)
 {
   initialization();
 };
 

void AHT10::parsing_data(void)
{
  	measurement();
  	recieve_data();  
  	temperature = conversion_t(temperature_data);
    humidity = conversion_h(temperature_data);
	dew_point = conversion_d(temperature,humidity);
}

float AHT10::get_temperature(void)
{
	return temperature;
}
float AHT10::get_humidity(void)
{
	return humidity;
}
float AHT10::get_dew_point(void)
{
	return dew_point ;
		
}
void AHT10::reset(void)
{
	sensor_reset();
	initialization(); // try it again 
	
}

////////////////////// private method`s//////////////////////////////////////////////////

void AHT10::measurement(void)
{
	i2c_master.initial_start();
	for(uint8_t val = 0; val < lenght_data; val++)
	{
		i2c_master.send_data(measure_data[val]);
	}
	i2c_master.initial_stop();
	_delay_ms(100);
}

void AHT10::initialization(void)
{
	i2c_master.initial_start();
	for(uint8_t val = 0; val < lenght_data; val++)
	{
	     i2c_master.send_data(initial_data[val]);   
	}
	
	i2c_master.initial_stop();
	
	_delay_ms(50);
}

void AHT10::recieve_data(void)
{
	i2c_master.initial_start();
	i2c_master.send_data(address_read);// set up read mode
	for(uint8_t val = 0; val < lenght_sensor_data; val++)
	{
		sensor_data[val] = i2c_master.read_byte();
		if(val==5)sensor_data[val] = i2c_master.read_byte_NACK();
	}
	i2c_master.initial_stop();
	
	if(0x80 == ((sensor_data[0]<<7) & 0xF0))
	{
		//debug_blink();
		sensor_error_count++;
		if(sensor_error_count >= 3) reset();
	} 
	
	
	humidity_data = ((sensor_data[1] << 16) | (sensor_data[2] << 8 ) | sensor_data[3]) >> 4;
	temperature_data = (((sensor_data[3] & 0x0F) << 16) | (sensor_data[4] << 8) | sensor_data[5]);
	
}

void AHT10::sensor_reset(void)
{
	i2c_master.send_data(soft_reset[0]);
	i2c_master.send_data(soft_reset[1]);
	sensor_error_count=0;
	_delay_ms(30);
}
