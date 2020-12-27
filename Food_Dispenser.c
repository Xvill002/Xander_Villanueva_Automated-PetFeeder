include <avr/interrupt.h>
#include<avr/io.h>
#include<util/delay.h>


//
//
//
//
//
// THIS FILE CONTAINS THE CODE IN CHARGE OF MAKING THE DEVICE DISPENSE FOOD WHEN THE BOWL ISNT FULL ENOUGH
//
//
//
//

// TIMER INITIALIZATIONS 

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks.

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B = 0x1B;	// bit3 = 0: CTC mode (clear timer on compare)
	TCCR1B |= 1<<CS10;
	ICR1=4999;
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
}

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//       analog to digital conversions.
}

void Wait()
{
	uint8_t i;
	for(i=0;i<10;i++)
	{
		
		_delay_loop_2(0); //waits so the motor finishes reaching its position before it can receive another signal
	}

}


void Spin_Motor(int is_in_open_position) // Spins the Servo Motor to open and close position
{
	if(is_in_open_position == 0) //Open dispenser if value is 0
	{
		OCR1A =	170;
		Wait();
	}
	
	else if(is_in_open_position == 1) //Closes dispenser if value is 1
	{
		OCR1A = 75;
		Wait();
	}
	
	return;
	
}


int main(void)
{
	unsigned short adc_value; // This will be holding the value of the force sensor resisitor
	DDRA = 0x00; PORTA = 0xFF; // Port Connected to Force sensor resistor
	DDRD = 0xFF; PORTD = 0x00; // Port connected to the servo motor
	A2D_init();
	TimerOn();
	TimerSet(1);
	
	while(1)
	{
		adc_value = ADC; // Representative value of weight on the pressure sensor
		if(adc_value  < 600) // rational value that matched a full bowl of food
		{
			Spin_Motor(0); // spin motor to open position
		}
		
		else
		{
			Spin_Motor(1); // spint motor to closed position
		}
		
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		
		
	}
}

