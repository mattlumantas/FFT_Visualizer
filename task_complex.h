#ifndef TASK_H
#define TASK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include <math.h>
#include <stdlib.h>

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}

typedef struct _complex {
	short real;
	short imaginary;
} complex;

const char sine[192] = {
	0, 3, 6, 9, 12, 15, 18, 21,
	24, 28, 31, 34, 37, 40, 43, 46,
	48, 51, 54, 57, 60, 63, 65, 68,
	71, 73, 76, 78, 81, 83, 85, 88,
	90, 92, 94, 96, 98, 100, 102, 104,
	106, 108, 109, 111, 112, 114, 115, 117,
	118, 119, 120, 121, 122, 123, 124, 124,
	125, 126, 126, 127, 127, 127, 127, 127,

	127, 127, 127, 127, 127, 127, 126, 126,
	125, 124, 124, 123, 122, 121, 120, 119,
	118, 117, 115, 114, 112, 111, 109, 108,
	106, 104, 102, 100, 98, 96, 94, 92,
	90, 88, 85, 83, 81, 78, 76, 73,
	71, 68, 65, 63, 60, 57, 54, 51,
	48, 46, 43, 40, 37, 34, 31, 28,
	24, 21, 18, 15, 12, 9, 6, 3,

	0, -3, -6, -9, -12, -15, -18, -21,
	-24, -28, -31, -34, -37, -40, -43, -46,
	-48, -51, -54, -57, -60, -63, -65, -68,
	-71, -73, -76, -78, -81, -83, -85, -88,
	-90, -92, -94, -96, -98, -100, -102, -104,
	-106, -108, -109, -111, -112, -114, -115, -117,
	-118, -119, -120, -121, -122, -123, -124, -124,
	-125, -126, -126, -127, -127, -127, -127, -127,
};

void fft(unsigned short size, complex timeD[], unsigned short step, complex freqD[]) {
	complex twiddle;
	short tempTwiddle;
	short scale = 1;
	short i = 0;
	complex temp;
	if (size == 1) {
		*freqD = *timeD;
	}
	else {
		fft(size / 2, timeD, step * 2, freqD);
		fft(size / 2, (timeD + step), step * 2, (freqD + size / 2));
		for (i = 0; i < size / 2; i++) {
			temp = *(freqD + i);
			//tempTwiddle = ((float)i / size);
			//twiddle = ((tempTwiddle - floor(tempTwiddle)) * 128);
			twiddle.real = -((float)sine[i + 64] / 127) * (freqD + i + size / 2)->real - -((float)sine[i] / 127) * (freqD + i + size / 2)->imaginary;
			twiddle.imaginary = -((float)sine[i + 64] / 127) * (freqD + i + size / 2)->imaginary + -((float)sine[i] / 127) * (freqD + i + size / 2)->real;
			(freqD + i)->real = (temp.real + twiddle.real) / scale;
			(freqD + i)->imaginary = (temp.imaginary + twiddle.imaginary) / scale;
			(freqD + i + size / 2)->real = (temp.real - twiddle.real) / scale;
			(freqD + i + size / 2)->imaginary = (temp.imaginary - twiddle.imaginary) / scale;
		}
	}
	return;
}

/*
void fft(unsigned short size, complex timeD[], unsigned short step, complex freqD[]) {
	complex twiddle;
	short tempTwiddle;
	short i = 0;
	complex temp;
	if (size == 1) {
		*freqD = *timeD;
	}
	else {
		fft(size / 2, timeD, step * 2, freqD);
		fft(size / 2, (timeD + step), step * 2, (freqD + size / 2));
		for (i = 0; i < size / 2; i++) {
			temp = *(freqD + i);
			tempTwiddle = ((float)i / size);
			//twiddle = ((tempTwiddle - floor(tempTwiddle)) * 128);
			twiddle.real = ((float)sine[tempTwiddle + 64] / 127) * (freqD + i + size / 2)->real - ((float)sine[tempTwiddle] / 127) * (freqD + i + size / 2)->imaginary;
			twiddle.imaginary = ((float)sine[tempTwiddle + 64] / 127) * (freqD + i + size / 2)->imaginary + ((float)sine[tempTwiddle] / 127) * (freqD + i + size / 2)->real;
			(freqD + i)->real = temp.real + twiddle.real;
			(freqD + i)->imaginary = temp.imaginary + twiddle.imaginary;
			(freqD + i + size / 2)->real = temp.real - twiddle.real;
			(freqD + i + size / 2)->imaginary = temp.imaginary - twiddle.imaginary;
		}
	}
	return;
}
*/

// void fft(unsigned short size, short timeD[], unsigned short step, short freqD[]) {
// 	char twiddle[4] = { 1, -1, -1, 1};
// 	short i, temp = 0;
// 	if (size == 1) {
// 		*freqD = *timeD;
// 	}
// 	else {
// 		fft(size / 2, timeD, step * 2, freqD);
// 		fft(size / 2, (timeD + step), step * 2, (freqD + size / 2));
// 		for (i = 0; i < size / 2; i++) {
// 			temp = *(freqD + i);
// 			*(freqD + i) = temp + twiddle[i % 4] * (*(freqD + i + size / 2));
// 			*(freqD + i + size / 2) = temp - twiddle[i % 4] * (*(freqD + i + size / 2));
// 		}
// 	}
// 	return;
//}

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

complex timeD[256];
complex freqD[256];
char magnitude[16];
unsigned char i, j, adc_done, fft_ready,lcd_ready = 0;
unsigned char fft_done, lcd_done = 1;
unsigned short adc_cnt = 0;

enum ADC_IN_states {ADC_IN_start, ADC_IN_pause, ADC_IN_poll};
enum FFT_states {FFT_start, FFT_pause, FFT_transform};
enum LCD_states {LCD_start, LCD_pause, LCD_process, LCD_display};
	
int ADC_IN_SM(int state) {

	
	switch (state) {
		case ADC_IN_start:
			state = ADC_IN_pause;
			break;
			
		case ADC_IN_pause:
			if (!adc_done) {
				state = ADC_IN_poll;
			}
			else {
				state = ADC_IN_pause;
			}
			break;
				
			
		case ADC_IN_poll:
			if (adc_cnt >= 256) {
				state = ADC_IN_pause;
				adc_cnt = 0;
				adc_done = 1;
				fft_done = 0;
			}
			else {state = ADC_IN_poll;
			}
			//state = ADC_IN_poll;
			break;
			
		default:
			state = ADC_IN_start;
			break;
		
	}
	
	switch (state) {
		case ADC_IN_start:
			break;
			
		case ADC_IN_pause:
			break;
			
		case ADC_IN_poll:
		//for( adc_cnt = 0; adc_cnt < 256; adc_cnt++) {
			timeD[adc_cnt].real = ADCH >> 1;
			//timeD[adc_cnt] = 500;
			timeD[adc_cnt].imaginary = 0;
			adc_cnt++;
		//}
			//adc_done = 1;
			//fft_done = 0;
			break;
		
		default:
			break;
	}
	return state;
}

int FFT_SM(int state) {
	switch (state) {
		case FFT_start:
			state = FFT_pause;
			break;
		
		case FFT_pause:
			if (!fft_done && lcd_done) {
				state = FFT_transform;
			}
			else {
				state = FFT_pause;
			}
			break;
		
		case FFT_transform:
			state = FFT_pause;
			break;
		
		default:
			state = FFT_start;
			break;
	}
	
	switch (state) {
		case FFT_start:
			break;
			
		case FFT_pause:
			break;
			
		case FFT_transform:
			fft(256, timeD, 1, freqD);
			adc_done = 0;
			fft_done = 1;
			lcd_done = 0;
			break;
			
		default:
			break;
	}
	return state;
}

int LCD_SM(int state) {
	long tempMag;
	short tempDisplay;
	
	switch (state) {
		case LCD_start:
			state = LCD_pause;
			break;
		
		case LCD_pause:
			 if (!lcd_done) {
				 state = LCD_process;
			 }
			 else {
				 state = LCD_pause;
			 }
			 break;

		case LCD_process:
			state = LCD_display;
			break;
			
		case LCD_display:
			state = LCD_pause;
			break;
			
		default:
			state = LCD_start;
			break;
	}

	switch (state) {
		case LCD_start:
			break;
			
		case LCD_pause:
			break;
		
		case LCD_process:
			for (i = 0; i < 16; i++) {
				tempMag = 0;
				for (j = 0; j < 16; j += 1) {
					tempMag += sqrt(((long)freqD[(i * 16) + j].real * (long)freqD[(i * 16) + j].real) + ((long)freqD[(i * 16) + j].imaginary * (long)freqD[(i * 16) + j].imaginary));
					//tempMag += abs(freqD[(i * 4) + j].real);
					
				}
				magnitude[i] = (short)(tempMag / 16);
			}
// 			for (i = 0; i < 16; i++) {
// 				magnitude[i] = freqD[i];
// 			}
 			lcd_done = 1;
			break;
			
		case LCD_display:
			for (i = 1; i <= 16; i++) {
				tempDisplay = magnitude[i - 1] / 28;
				if (tempDisplay < 8) {
					LCD_Cursor(i + 16);
					LCD_WriteData(tempDisplay);
					LCD_Cursor(i);
					LCD_WriteData(0x20);
				}
				else if (tempDisplay > 16) {
					LCD_Cursor(i + 16);
					LCD_WriteData(7);
					LCD_Cursor(i);
					LCD_WriteData(tempDisplay - 8);
				}
				else {
					LCD_Cursor(i + 16);
					LCD_WriteData(7);
					LCD_Cursor(i);
					LCD_WriteData(7);
				}
			}
			break;
		
		default:
			break;
	}
	return state;
}

#endif