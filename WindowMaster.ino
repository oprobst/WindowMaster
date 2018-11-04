
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







////////////////////////////////////////////////////////////////////KNX
void setKNX() {

}/*
 knx.addListenGroupAddress(KNX_GA_CO2);                                                                                 //Gruppenadressen registrieren
 knx.addListenGroupAddress(KNX_GA_TVOC);
 knx.addListenGroupAddress(KNX_GA_TEMP);
 knx.addListenGroupAddress(KNX_GA_HUM);
 if (KNX_receive) {knx.addListenGroupAddress(KNX_GA_VALVE); knx.addListenGroupAddress(KNX_GA_SOLL);
 knx.addListenGroupAddress(KNX_GA_BA);}                                                            //Bei KNX-Receive
 }

 /**
 ///////////////////////////////////////////////////////KNX Bus Readanfrage beantworten
 void serialEvent() {
 KnxTpUartSerialEventType eType = knx.serialEvent();
 if (eType == KNX_TELEGRAM) {

 KnxTelegram* telegram = knx.getReceivedTelegram();

 String target =
 String(0 + telegram->getTargetMainGroup())   + "/" +
 String(0 + telegram->getTargetMiddleGroup()) + "/" +
 String(0 + telegram->getTargetSubGroup());

 if (telegram->getCommand() == KNX_COMMAND_READ) {
 readT=true;
 if (strcmp(target.c_str(),KNX_GA_TEMP)==0) {
 knx.groupAnswer2ByteFloat(KNX_GA_TEMP, temp);
 //result = knx.groupWrite2ByteFloat(KNX_GA_TEMP, temp);
 result = knx.groupWrite2ByteFloat(KNX_GA_HUM, hum);
 result = knx.groupWrite2ByteInt(KNX_GA_CO2, airQuality);
 result = knx.groupWrite2ByteInt(KNX_GA_TVOC, airTvoc);
 if (KNX_receive) {
 result = knx.groupWrite2ByteFloat(KNX_GA_SOLL, Sollwert);
 result = knx.groupWrite1ByteInt(KNX_GA_VALVE, (y_res*2.55));
 result = knx.groupWrite1ByteInt(KNX_GA_BA, BA);
 }
 }
 //     if (strcmp(target.c_str(),KNX_GA_HUM)==0) {
 //          knx.groupAnswer2ByteFloat(KNX_GA_HUM, hum);
 //     }
 //     if (strcmp(target.c_str(),KNX_GA_CO2)==0) {
 //          knx.groupAnswer2ByteInt(KNX_GA_CO2, airQuality);
 //     }
 //     if (strcmp(target.c_str(),KNX_GA_TVOC)==0) {
 //          knx.groupAnswer2ByteInt(KNX_GA_TVOC, airTvoc);
 //     }
 //     if (strcmp(target.c_str(),KNX_GA_BA)==0) {
 //          knx.groupAnswer1ByteInt(KNX_GA_BA, BA);
 //     }
 //     if (strcmp(target.c_str(),KNX_GA_SOLL)==0) {
 //          knx.groupAnswer2ByteFloat(KNX_GA_SOLL, Sollwert);
 //     }
 readT=false;
 }
 if ((telegram->getCommand() == KNX_COMMAND_WRITE)&&(KNX_receive)) {
 readT=true;
 if (strcmp(target.c_str(),KNX_GA_SOLL)==0) {
 Sollwert = telegram->get2ByteFloatValue();
 }
 if (strcmp(target.c_str(),KNX_GA_BA)==0) {
 BA = telegram->get1ByteIntValue();
 }
 if (strcmp(target.c_str(),KNX_GA_VALVE)==0) {
 y_res = telegram->get1ByteIntValue();
 y_res = y_res/2.55;
 }
 readT=false;
 }
 }
 }   */

