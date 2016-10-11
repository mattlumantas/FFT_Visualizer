/*
 * Audio_Visualizer.c
 *
 * Created: 5/19/2016 10:47:30 AM
 *  Author: Matt
 */ 

#include <avr/io.h>
#include "timer.h"
#include "io.c"
#include "adc.h"
#include "task_complex.h"




int main(void)
{

	MCUCR |= (1u << PUD);
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	ADC_init();
	LCD_init();
	LCD_CharInit();
	
	unsigned long int ADC_IN_calc = 1;
	unsigned long int FFT_SM_calc = 50;
	unsigned long int LCD_SM_calc = 50;
	
	unsigned long int tempGCD = 1;
	tempGCD = findGCD(ADC_IN_calc, FFT_SM_calc);
	tempGCD = findGCD(tempGCD, LCD_SM_calc);
	
	unsigned long int GCD = tempGCD;
	
	unsigned long int ADC_IN_period = ADC_IN_calc / GCD;
	unsigned long int FFT_SM_period = FFT_SM_calc / GCD;
	unsigned long int LCD_SM_period = LCD_SM_calc / GCD;
	
	static task ADC_TASK, FFT_TASK, LCD_TASK;
	task * tasks[] = { &ADC_TASK, &FFT_TASK, &LCD_TASK};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	
	ADC_TASK.state = ADC_IN_start;
	ADC_TASK.period = ADC_IN_period;
	ADC_TASK.elapsedTime = ADC_IN_period;
	ADC_TASK.TickFct = &ADC_IN_SM;
	
	FFT_TASK.state = FFT_start;
	FFT_TASK.period = FFT_SM_period;
	FFT_TASK.elapsedTime = FFT_SM_period;
	FFT_TASK.TickFct = &FFT_SM;
	
	LCD_TASK.state = LCD_start;
	LCD_TASK.period = LCD_SM_period;
	LCD_TASK.elapsedTime = LCD_SM_period;
	LCD_TASK.TickFct = &LCD_SM;

	TimerOn();
	TimerSet(GCD);
	
	unsigned short k;
    while(1)
    {
		for (k = 0; k < numTasks; k++) {
			if (tasks[k]->elapsedTime == tasks[k]->period) {
				tasks[k]->state = tasks[k]->TickFct(tasks[k]->state);
				tasks[k]->elapsedTime = 0;
			}
			tasks[k]->elapsedTime += 1;
		}
		while(!TimerFlag){};
		TimerFlag = 0;
	}
}