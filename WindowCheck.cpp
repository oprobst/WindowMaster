#include "WindowCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"
#include "UI.h"
#include "KNX.h"


void initWindowCheck() {
	for (short i = 0; i < ARRAYSIZE; i++) {
		pinMode(functionPin[i], INPUT_PULLUP);
		pinMode(statePin[i], INPUT_PULLUP);
		if (i == 1)
			break; //TODO REMOVE me for arduino mega
	}
}

short checkAllContacts() {
	short updateUiNecessary = FALSE;
	for (short i = 0; i < ARRAYSIZE; i++) {
		if (checkContact(i)) {
			updateUiNecessary = TRUE;
		}
	}

	return updateUiNecessary;
}

short checkContact(short no) {
	if (no > 1) { //TODO Remove this if for arduino mega
		return FALSE;
	}
	short state = digitalRead(statePin[no]);
	short function = digitalRead(functionPin[no]);
	short result = 0;

	if (state == HIGH) {
		//Window open
		result = WINDOW_OPEN;
	} else {
		//Window closed
		result = WINDOW_CLOSE;
	}

	if (function == HIGH) {
		//Contact broken!
		result = WINDOW_ERROR;
	}

	if (result != windowState[no]) {
		sendSingleWindowEvent(no, result, windowState[no]);
		windowState[no] = result;
		lastEvent = no;
		return TRUE;
	}

	return FALSE;
}


