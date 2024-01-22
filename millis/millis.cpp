/*
  * millis.cpp
  *
  * Created: 19.01.2024 15:10:25
  *  Author: sWe
 */ 

#include "millis.h"

volatile unsigned long  milliseconds; // Value for store milliseconds

ISR(TIMER1_COMPA_vect) // interrupt service
{
  milliseconds++;	
}


 void init_timer(void)
{
	t_timer -> tccrb |= (1<<WGM12);  // Set up ctc mode
	t_timer -> tccrb |= (1<< CS11);  // Set up prescaler
    *timsk |= (1<<OCIE1A);           //Timer compare on match  interrupt enable
	*ocr_h = (OCR_VALUE>>8);         // Beginning write high byte
	*ocr_l = (uint8_t )OCR_VALUE;    // After write low byte!
	sei();                           // Enable global interrupt
}

void timer_off(void)
{
	t_timer -> tccrb &= ~(1<<WGM12);  // Turn off ctc mode
	t_timer -> tccrb &= ~(1<< CS11);  // Turn off prescaler
	*timsk &= ~(1<<OCIE1A);           //Timer compare on match  interrupt disable
	cli();                            // Disable global interrupt
}



unsigned long get_millis(void)
{
 unsigned long millis;

 ATOMIC_BLOCK(ATOMIC_FORCEON)
 {
   millis = milliseconds; 	
 }
  return millis;
}