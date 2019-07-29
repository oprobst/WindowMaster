#include "WindowCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"
#include "UI.h"
#include "KNX.h"

int currentSendKnx[] = { 0, 0, 0 };
int currentVisualized[] = { 0, 0, 0 };


long sample(short analogInput, short samples) {
  //return analogRead(analogInput);
	long result = 0;
	for (short i = 0; i < samples; i++) {
		result += analogRead(analogInput);
	}
	return result / samples;
}

long voltageCheck() {
  return  3.26 / sample(A3, 2) * 1023 *1000;
}

long toMilliAmpere(int sensor) {
  const short mVperAmp = 133; //mV
  const int offset = 465; //mV
	//long sensor = sample(analogInput, 5);
  sensor = sensor * uRef / 1023;
  sensor = sensor - offset;
  //sensor = sensor * 1000 / mVperAmp;
  return sensor / mVperAmp * 1000;
  //return sensor;
}

int checkCurrent(short no) {
	if (no == 0) {
		return toMilliAmpere(lastReadCurrent[0]);
	} else if (no == 1) {
		return toMilliAmpere(lastReadCurrent[1]);
	} else if (no == 2) {
		return toMilliAmpere(lastReadCurrent[2]);
	} else {
		return 0;
	}
}

short checkAllCurrents() {
	short result = FALSE;
  uRef = voltageCheck();
	for (short i = 0; i < 3; i++) {
		current[i] = checkCurrent(i);
		if (currentVisualized[i] * 1.01 < current[i]
				|| currentVisualized[i] * 0.99 > current[i]) {
			result = TRUE;
			currentVisualized[i] = current[i];      
		}
		if (currentSendKnx[i] * 1.05 < current[i]
				|| currentSendKnx[i] * 0.95 > current[i]) {
			//sendCurrentUpdate(i, current[i]);
			currentSendKnx[i] = current[i];
		}
	}
	return result;

}
