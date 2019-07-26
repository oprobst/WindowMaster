#include "WindowCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"
#include "UI.h"
#include "KNX.h"

int currentSendKnx[] = { 0, 0, 0 };
int currentVisualized[] = { 0, 0, 0 };
int current[] = { 0, 0, 0 };

int sample(short analogInput, short samples) {
	int result = 0;
	for (short i = 0; i < samples; i++) {
		result += analogRead(analogInput);
	}
	return result / samples;
}

//in mVolt
uint16_t voltageCheck() {
	uint16_t result = sample(A3, 5);
	return 3300 / result * 1023;
}

int toMilliAmpere(short analogInput) {
	short mVperAmp = 66;
	int voltage = voltageCheck();
	int sensor = sample(analogInput, 15) * voltage / 1023;
	return sensor / mVperAmp * 1000;
}

int checkCurrent(short no) {
	if (no == 0) {
		return toMilliAmpere(A4);
	} else if (no == 1) {
		return toMilliAmpere(A5);
	} else if (no == 2) {
		return toMilliAmpere(A6);
	} else {
		return 0;
	}
}

short checkAllCurrents() {
	short result = FALSE;
	for (short i = 0; i < 3; i++) {
		current[i] = checkCurrent(i);
		if (currentVisualized[i] * 1.01 < current[i]
				|| currentVisualized[i] * 0.99 > current[i]) {
			result = TRUE;
			currentVisualized[i] = current[i];
		}
		if (currentSendKnx[i] * 1.05 < current[i]
				|| currentSendKnx[i] * 0.95 > current[i]) {
			sendCurrentUpdate(i, current[i]);
			currentSendKnx[i] = current[i];
		}
	}
	return result;

}
