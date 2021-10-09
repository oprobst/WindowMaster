#include "KNX.h"
#include <KnxTpUart.h>
#include <SPI.h>
#include "WindowMasterConfig.h"

KnxTpUart knx(&Serial1, KNX_PA);

void initKnx() {
	Serial1.begin(19200, SERIAL_8E1);
	while (!Serial) {
		;
	}
	if (Serial1.available()) {
		knx.uartReset();
	}

}

void sendSingleWindowEvent(short no, short newType, short oldType) {

	eventsSinceReset++;

	if (newType == WINDOW_ERROR) {
		knx.groupWriteBool(contactGA[no], true);
		return;
	}
	if (oldType == WINDOW_ERROR) {
		knx.groupWriteBool(contactGA[no], false);
		delay(50);
	}
	if (newType == WINDOW_OPEN) {
		knx.groupWriteBool(functionGA[no], true);
	} else {
		knx.groupWriteBool(functionGA[no], false);
	}

}
	void sendCurrentUpdate (short no, float value){
		  knx.groupWrite2ByteFloat(currentGA[no], value * 1000.0);
		  delay(50);
	}

	void sendCisternUpdate (int value){
		  knx.groupWrite2ByteInt(CISTERN_GA, value);
		  delay(50);
	}


	void sendTemperatureUpdate (char * addr, float value){
		  knx.groupWrite2ByteFloat(addr, value);
		  delay(50);
	}
