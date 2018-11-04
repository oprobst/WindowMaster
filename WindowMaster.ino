
#include "UI.h"
#include "KNX.h"
#include "WindowCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"

void initTimer(){
	  noInterrupts();
	  TCCR1A = 0;
	  TCCR1B = 0;
	  TCNT1 = 0;
	  TCCR1B |= (1<<CS12) | (1<<CS10);
	  TIMSK1 |= (1 << TOIE1);
	  interrupts();
}

ISR(TIMER1_OVF_vect)
{

  TCNT1 = 49911;
  timeUntilDisplayOff--;
  if (timeUntilDisplayOff == 0){
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
	if (checkAllContacts()) {
		displayUpdate();
	}
	delay(50);
	if (! digitalRead(USER_SWITCH)){
		digitalWrite(LED_DISPLAY, true);
		TCNT1 = 49911;
		timeUntilDisplayOff = TIME_UNTIL_DISPLAY_OFF;
		interrupts();
	}
}
