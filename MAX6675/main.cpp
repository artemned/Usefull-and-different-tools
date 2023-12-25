
//#include <stdint.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "USI.h"
#include "ftoa.h"
#include "State_FSM_SPI.h"
//#include "Setup.h"
//#include <stdlib.h>

int main(void)
{
	tx_init();	
	disable_ADC();
	MAX6675_SPI temp;
	//MAX6675 temp;
	//dischargeSignal();
	char postfix[]= ";";
	char sourceBuffer[]="";
	while(1)
	{
	   ftoa(temp.read_temp(),sourceBuffer,2);//need delay before send data	
		_delay_ms(1000); 		
		out_device(sourceBuffer);
		out_device(postfix);
		_delay_ms(1000);
	}
	return 0;
}


