//#include <avr/io.h>
//#include <avr/delay.h>
#include "AHT10.h"



int main(void)
{
    
	
	float temperatura=0;
	float humidity=0;
	float dew_point = 0;
	
	AHT10 sensor;//uncomment either it column or bottom  
  //AHT10 sensor(0x38); //create object and initialization him interface	
 

		while(1){
        sensor.parsing_data();
		temperatura = sensor.get_temperature();
		humidity = sensor.get_humidity();
	    dew_point = sensor.get_dew_point();
		 
		_delay_ms(5000);
 		
		
		}
	//}
		
	
}








// sensor_data[0]=read_byte(); //information byte from slave device
// sensor_data[1]=read_byte(); //hum
// sensor_data[2]=read_byte(); //hum
// sensor_data[3]=read_byte(); //hum and temp
// sensor_data[4]=read_byte(); //temp
// sensor_data[5]=read_byte_NACK(); //last byte temp