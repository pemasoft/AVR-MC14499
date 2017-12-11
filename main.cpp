/*
* 7-Segment display driver MC14499P connect to 8 bit AVR MCU (ATMEGA328)
* Created: 11.12.2017
* Author : Petr Matejovsky
* License: GNU PUBLIC
*/

// define CPU fervencies 16 MHz
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define setPORT DDRB	// define port register output - PORT B
#define PORT PORTB		// define port register - PORT B

#define enablePin PB0	// enable MC14499P pin 12
#define clockPin PB4	// clock MC14499P pin 13
#define dataPin PB3		// data MC14499P pin 5

#define ledPin PB5		// LED DIODE

#define OUTPUT_PINS() setPORT |= ((1 << enablePin) | (1 << clockPin) | (1 << dataPin) | (1 << ledPin))  // define pins as output

#define ENABLE_HIGH() PORT |= (1 << enablePin)	// set enable pin to log 1
#define ENABLE_LOW() PORT &= ~(1 << enablePin)	// set enable pin to log 0

#define CLOCK_HIGH() PORT |= (1 << clockPin)	// set clock pin to log 1
#define CLOCK_LOW() PORT &= ~(1 << clockPin)	// set clock pin to log 0

#define DATA_HIGH() PORT |= (1 << dataPin)		// data pin log 1
#define DATA_LOW() PORT &= ~(1 << dataPin)		// data pin log 0

#define LED_ON() PORT |= (1 << ledPin)			// LED ON
#define LED_OFF() PORT &= ~(1 << ledPin)		// LED OFF



void ShiftOut(uint8_t byte)
{
	// 4 bits in byte send to data pin
	for (int i = 0; i < 4; i++) {
		(((1 << (3 - i)) & byte) == 0) ? DATA_LOW() : DATA_HIGH();
		CLOCK_HIGH();	// clock impulse
		_delay_us(5);
		CLOCK_LOW();
	}
}

void Digits(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
{
	ENABLE_LOW();	// enable chip
	_delay_us(5);
	
	ShiftOut(0b00000010);	// decimal point
	ShiftOut(digit4);		// digit
	ShiftOut(digit3);
	ShiftOut(digit2);
	ShiftOut(digit1);
	
	_delay_us(5);
	ENABLE_HIGH();	// disable chip
}

void Display(uint8_t hours, uint8_t minutes)
{
	uint8_t m2 = (uint8_t)(minutes - (minutes / 10) * 10);
	uint8_t m1 = (uint8_t)(minutes / 10);
	uint8_t h2 = (uint8_t)(hours - (hours / 10) * 10);
	uint8_t h1 = (uint8_t)(hours / 10);
	
	if (h1 == 0) {
		h1 = 0b00001111;	// disable first digit
	}
	
	Digits(h1, h2, m1, m2);
}

int main(void)
{
	// setup value
	uint8_t hours = 13;
	uint8_t minutes = 22;
	uint8_t second = 0;
	
	// set port pins
	OUTPUT_PINS();
	
	while (1) {
		Display(hours, minutes);
		
		_delay_ms(500);
		LED_OFF();
		_delay_ms(500);
		LED_ON();
		
		second ++;
		
		if (second == 60) {
			second = 0;
			minutes ++;
		}
		if (minutes == 60) {
			minutes = 0;
			hours ++;
		}
		if (hours == 24) {
			hours = 0;
		}
	}
}
