#include "KNX.h"
#include <KnxTpUart.h>
#include <SPI.h>
#include "WindowMasterConfig.h"

KnxTpUart knx(&Serial, KNX_PA);

void initKnx() {
	Serial.begin(19200, SERIAL_8E1);
	while (!Serial) {
		;
	}
	if (Serial.available()) {
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

