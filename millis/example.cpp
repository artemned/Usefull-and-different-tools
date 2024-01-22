
#include "millis.h"
#define LED_PIN 5


unsigned long millis;
unsigned long prev_millis=0;

typedef struct
{
	volatile uint8_t input_pin_address ; //PINB
	volatile uint8_t data_direction ;    //DDRB
	volatile uint8_t data;              //PORTB

}GPIO;

#define GPIOO ((GPIO*)(0x23U))

inline void setPIN(void)
{
	GPIOO->data_direction |= _BV(LED_PIN); //Pin as out
}




int main(void)
{
	
	init_timer();
    setPIN();	
	 
    while (1) 
    {
		if(get_millis() - prev_millis > 500)
		{
			GPIOO->data ^= _BV(LED_PIN);
			
			prev_millis = get_millis();
		}
		
		
		
		
    }
}

