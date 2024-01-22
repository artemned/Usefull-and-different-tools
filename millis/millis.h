#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>


#define  sei()  __asm__ __volatile__ ("sei" ::: "memory")
#define  cli()  __asm__ __volatile__ ("cli" ::: "memory")

#define OCR_VALUE 2000  // Data for compare on match OCRnA register 1/2000 = 0.0005 

ISR(TIMER1_COMPA_vect);

  uint8_t *const timsk = (uint8_t*) 0x6FU; // register TIMSK1
  uint8_t *const ocr_l = (uint8_t*) 0x88U; // Timer/Counter1 - Output Compare Register A Low Byte
  uint8_t *const ocr_h = (uint8_t*) 0x89U; // Timer/Counter1 - Output Compare Register A High Byte

//This struct used to timer/counter number 1
typedef struct 
{
	volatile uint8_t tccra;
	volatile uint8_t tccrb;
	volatile uint8_t tccrc;

}timer;

#define t_timer ((timer*) (0x80U)) // Address in table registers where is TIMER1

void init_timer(void);
void timer_off(void);
unsigned long get_millis(void);


