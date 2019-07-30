#include "WindowCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"
#include "UI.h"
#include "KNX.h"

float currentSendKnx[] = { 0, 0, 0 };
float currentVisualized[] = { 0, 0, 0 };


long sample(short analogInput, short samples) {
  //return analogRead(analogInput);
	long result = 0;
	for (short i = 0; i < samples; i++) {
		result += analogRead(analogInput);
	}
	return result / samples;
}

float voltageCheck() {
  return  3.26 / sample(A2, 3) * 1023.0 ;
}

float toAmpere(float sensor) {
  const float VperAmp = 0.133; //mV
  const float offset = 0.465; //mV
	
  sensor = sensor * uRef / 1023.0;
  sensor = sensor - offset;
  sensor = sensor / VperAmp;
  return sensor;
}

float checkCurrent(short no) {
	if (no == 0) {
		return toAmpere(lastReadCurrent[0]);
	} else if (no == 1) {
		return toAmpere(lastReadCurrent[1]);
	} else if (no == 2) {
		return toAmpere(lastReadCurrent[2]);
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
