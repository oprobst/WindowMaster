
#include "UI.h"
#include "KNX.h"
#include "WindowCheck.h"
#include "CurrentCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"
#define OFF 0
#define WINDOW 1
#define CURRENT 2

short show = OFF;
  
void initTimer() {
	noInterrupts();
	// Timer 0 default for Arduino lib
	// Timer 1 disable display
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	TCCR1B |= (1 << CS12) | (1 << CS10);
	TIMSK1 |= (1 << TOIE1);
	//Timer 2 for LED pin 9 (Ardunio Mega)
	//Timer 3 for measure current // 100kHz

	TCCR3A = 0;
	TCCR3B = 0;
	TCNT3  = 0;
	OCR3A = 159;
	TCCR3B |= (1 << WGM21);
	TCCR3B |= (0 << CS22) | (0 << CS21) | (1 << CS20);
	// enable timer compare interrupt
	TIMSK3 |= (1 << OCIE3A);
	interrupts();
}

const long samplesPerSec = 100000;
long currentSampleCount = 0;
long currentRead [3] = {0,0,0};

ISR(TIMER3_COMPA_vect){
   currentSampleCount ++;
   currentRead [0] = (currentRead[0] * 1000 / currentSampleCount + analogRead (A5)* 1000/currentSampleCount)*currentSampleCount* 1000;
   currentRead [1] = (currentRead[1] * 1000/ currentSampleCount + analogRead (A3)* 1000/currentSampleCount)*currentSampleCount* 1000;
   currentRead [2] = (currentRead[2] * 1000/ currentSampleCount + analogRead (A4)* 1000/currentSampleCount)*currentSampleCount* 1000;
   if (currentSampleCount >= samplesPerSec){
	   lastReadCurrent [0] = currentRead[0]/1000;
	   lastReadCurrent [1] = currentRead[1]/1000;
	   lastReadCurrent [2] = currentRead[2]/1000;
	   currentSampleCount = 0;
	   currentRead [0] = 0;
	   currentRead [1] = 0;
	   currentRead [2] = 0;
   }
}

ISR(TIMER1_OVF_vect) {

	TCNT1 = 49911;
	timeUntilDisplayOff--;
	if (timeUntilDisplayOff == 0) {
		digitalWrite(LED_DISPLAY, false);
		noInterrupts();
	}
}

void setup() {
	pinMode(LED_DISPLAY, OUTPUT);
	pinMode(USER_SWITCH, INPUT_PULLUP);
	digitalWrite(LED_DISPLAY, true);
	displayInit();
	initKnx();
	delay(1000);
	initWindowCheck();
	initTimer();
	displayUpdate();
}


void loop() {

	if (checkAllContacts() && show == WINDOW) {
		displayUpdate();
	}
	if (checkAllCurrents() && show == CURRENT){
		displayCurrentUpdate();
	}

	delay(50);
	if (!digitalRead(USER_SWITCH)) {
		if (show == OFF) {
			show = WINDOW;
			digitalWrite(LED_DISPLAY, true);
      displayUpdate();
			delay (300);
		} else if (show == WINDOW){
			show = CURRENT;
			digitalWrite(LED_DISPLAY, true);
      displayCurrentUpdate();
			delay (300);
		} else {
			show = OFF;
			digitalWrite(LED_DISPLAY, false);
			delay (300);
		}

		TCNT1 = 49911;
		timeUntilDisplayOff = TIME_UNTIL_DISPLAY_OFF;
		interrupts();
	}
	for (short i = 0; i < ARRAYSIZE; i++) {
		if (windowState[i] == WINDOW_ERROR) {
			errorLedOn();
			break;
		}
		statusLedOn();
	}
}
