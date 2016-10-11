#ifndef ADC_H
#define ADC_H

#include <avr/interrupt.h>

void ADC_init() {
	
	ADMUX |= (1 << ADLAR);
	//ADLAR: left shifts result so that 8 MSBs are in ADCH
	
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS0);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
	// ADPS2 & ADPS1: setting these bits sets the ADC clock prescaler to 32
	
}




#endif