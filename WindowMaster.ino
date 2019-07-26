
#include "UI.h"
#include "KNX.h"
#include "WindowCheck.h"
#include "CurrentCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"
#define OFF 0
#define WINDOW 1
#define CURRENT 2

void initTimer() {
	noInterrupts();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	TCCR1B |= (1 << CS12) | (1 << CS10);
	TIMSK1 |= (1 << TOIE1);
	interrupts();
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
}


void loop() {

	short show = OFF;
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
			delay (500);
		} else if (show == WINDOW){
			show = CURRENT;
			digitalWrite(LED_DISPLAY, true);
			delay (500);
		} else {
			show = OFF;
			digitalWrite(LED_DISPLAY, false);
			delay (500);
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
