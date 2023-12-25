#pragma once
/*
 Conversion for AHT10_sensor
 humidity    = (Srh * 100%)/2^20 
 temperatura = (St * 200)/2^20  - 50
 pointD      = T — ((100 — RH) / 5) 
*/ 
#include "I2C_Master.h"
 


inline float conversion_h(unsigned long& sum_humidity_byte)
{
	return (float) (sum_humidity_byte * 100) / 1048576;
}

inline float conversion_t(unsigned long& sum_temperature_byte)
{
	return (float)(sum_temperature_byte * 200) / 1048576 - 50;
}

inline float conversion_d(float& temperature,float& humidity)
{
	return (100 - humidity) / 5 - temperature;
}



class AHT10
{
 
 public:
 
 AHT10(); //= default;
AHT10(const uint8_t& address);
 
 void parsing_data(void); 
 
 float get_temperature(void);
 
 float get_humidity(void);
 
 float get_dew_point(void);
 
 void reset(void);
 
private:

AHT10( const AHT10 &c ) = delete;
AHT10& operator=( const AHT10 &c ) = delete;

I2C_Master i2c_master;

const uint8_t device_address=0x38;

const uint8_t initial_data[4] = {(device_address<<1),0xE1,0x08,0x00};
const uint8_t measure_data[4] = {(device_address<<1),0xAC,0x33,0x00};
const uint8_t normal_cmd  [4] = {(device_address<<1),0xA8,0x00,0x00};
const uint8_t soft_reset  [2] = {(device_address<<1),0xBA};

const uint8_t address_read = (device_address<<1) | 1;
 
unsigned long sensor_data[6]; // buffer for data from sensor

int8_t lenght_data = sizeof(initial_data) / sizeof(initial_data[0]);
int8_t lenght_sensor_data = sizeof(sensor_data) / sizeof(sensor_data[0]);

unsigned long temperature_data,humidity_data;

float temperature,humidity,dew_point;

uint8_t sensor_error_count = 0;
 
 void measurement(void);
 
 void initialization(void);
 
 void recieve_data(void);

 void sensor_reset(void);
  
};



