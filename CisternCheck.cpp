#include "CisternCheck.h"
#include "WindowMasterConfig.h"
#include "UI.h"
#include "KNX.h"
#include <Arduino.h>

long lastSend = 0;

void updateCistern(int value) {
	cisternEventsSinceReset += 1;
	if (cisternEventsSinceReset > 2147483640)
		cisternEventsSinceReset = 0;
	cisternValue = value;
	if (millis() > lastSend + CISTERN_SEND_INTERVALL) {
		sendCisternUpdate(value);
		lastSend = millis();
	}
}

void initCisternCheck() {
	Serial2.begin(300);
	while (!Serial2) {
		;
	}
}
