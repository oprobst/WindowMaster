#include <TFT.h>
#include <SPI.h>
#include "UI.h"
#include "WindowMasterConfig.h"

short inErrorState = FALSE;
short isOnlyCurrentUpdate = FALSE;


/*

 * Iterate through the ds18b20_sensors array and compare hard coded sensor addresses with the one read from the sensor.
 * Return an const char * with the name stored in the array.

const char* getNameForSensorAddress(byte *addr) {
	for (uint8_t j = 0; j < amountKnownSensors; j++) {
		if (array_cmp(addr, ds18b20_sensors[j].address, BYTE_SIZE_ADDRESS)
				== true) {
			return ds18b20_sensors[j].name;
		}
	}
	return "unknown";
}
*/

/*
 * Read the byte address and translate it into a human readable string containing
 * hex values.
 * buf: buffer to write the created string into. Must be of size 24!
 * addr: The address to parse.
 */

void getStringForSensorAddress(char *buf, short *addr) {
	short *pin = addr;
	const char *hex = "0123456789ABCDEF";
	char *pout = buf;
	for (; pin < addr + BYTE_SIZE_ADDRESS; pout += 2, pin++) {
		pout[0] = hex[(*pin >> 4) & 0xF];
		pout[1] = hex[*pin & 0xF];
		//pout[2] = ':';
	}
	pout[-1] = 0;
}




Adafruit_ST7735 tft = Adafruit_ST7735(TFT_PIN_CS, TFT_PIN_DC, TFT_PIN_RST); // Display-Bibliothek Setup
//////////////////////////////////Setup
void displayInit() {
	//setKNX();
	tft.initR(INITR_BLACKTAB);   // ST7735-Chip initialisieren
	tft.setRotation(2);
	tft.setTextWrap(true);
	tft.fillScreen(ST7735_BLACK);
	tft.setTextSize(2);
	tft.setCursor(2, 60);
	tft.setTextColor(ST7735_RED);
	tft.print("Window-");
	tft.setCursor(10, 80);
	tft.print("Master");
	tft.setTextSize(1);
	tft.setCursor(30, 145);
	tft.setTextColor(ST7735_WHITE);
	tft.print("by Oli 2018-21");

	pinMode(LED, OUTPUT);

	//statusLedOn();
}

void displayCancelError() {
	inErrorState = FALSE;
	tft.fillScreen(ST7735_BLACK);
}

void displayError(char *reason) {
	inErrorState = TRUE;
	tft.fillScreen(ST7735_BLACK);
	tft.setTextSize(2);
	tft.setCursor(2, 2);
	tft.setTextColor(ST7735_BLUE);
	tft.print("ERROR");
	tft.setCursor(3, 30);
	tft.setTextColor(ST7735_WHITE);
	tft.print(reason);
}

short determineWindowState(short reqFloor, short state) {
	short count = 0;
	for (short i = 0; i < ARRAYSIZE; i++) {
		if (isOnfloor[i] == reqFloor && windowState[i] == state) {
			count++;
		}
	}
	return count;
}

void paintOpenWindow(short x, short y) {
	tft.fillRect(x, y, 8, 8, ST7735_CYAN);
	tft.drawRect(x, y, 8, 8, ST7735_RED);
	for (short i = 0; i < 7; i++) {
		tft.drawLine(x + 7, y + i, x + 3, y + i + 3, ST7735_RED);
	}
	tft.drawLine(x + 7, y + 6, x + 3, y + 10, ST7735_WHITE);
	tft.drawLine(x + 3, y + 2, x + 3, y + 10, ST7735_WHITE);
	tft.drawLine(x + 7, y, x + 3, y + 3, ST7735_WHITE);
	tft.drawLine(x + 7, y + 6, x + 7, y, ST7735_WHITE);

}
void paintClosedWindow(short x, short y) {
	tft.fillRect(x, y, 10, 8, ST7735_RED);
	tft.drawRect(x, y, 10, 8, ST7735_WHITE);
	tft.drawLine(x, y + 4, x + 2, y + 4, ST7735_WHITE);
	tft.drawLine(x + 2, y + 4, x + 2, y + 5, ST7735_WHITE);
}
void paintContactFail(short x, short y) {
	tft.fillTriangle(x + 4, y, x, y + 8, x + 8, y + 8, ST7735_BLUE);
	tft.drawTriangle(x + 4, y, x, y + 8, x + 8, y + 8, ST7735_CYAN);
	tft.drawPixel(x + 4, y + 7, ST7735_CYAN);
	tft.drawLine(x + 4, y + 2, x + 4, y + 5, ST7735_CYAN);
}

void printFloor(char *name, short open, short closed, short malfunction,
		short pxOffset) {
	tft.setTextSize(1);
	//draw icons:
	paintOpenWindow(25, pxOffset);
	paintClosedWindow(60, pxOffset);
	paintContactFail(95, pxOffset);

	//write Floor:
	tft.setCursor(8, pxOffset);
	tft.setTextSize(1);
	tft.print(name);
	//provide numbers:
	tft.setCursor(40, pxOffset);
	tft.print(open);
	tft.setCursor(75, pxOffset);
	tft.print(closed);
	tft.setCursor(110, pxOffset);
	tft.print(malfunction);
}
void printCurrent(char *name, float valueCurrent, short pxOffset) {
	tft.setTextSize(1);

	tft.setCursor(18, pxOffset);
	tft.setTextSize(1);
	tft.print(name);
	tft.fillRect(86 - 1, pxOffset - 1, 35, 10, ST7735_BLUE);
	tft.setCursor(86, pxOffset);
	tft.print(valueCurrent);
}

void displayUpdate() {
	if (inErrorState) { // don't show windows in error state
		return;
	}
	if (isOnlyCurrentUpdate) {
		isOnlyCurrentUpdate = FALSE;
	}
	tft.fillScreen(ST7735_BLACK);
	tft.setCursor(1, 1);
	tft.setTextSize(2);
	tft.setTextColor(ST7735_RED);
	tft.print("Fenster");
	tft.setTextColor(ST7735_WHITE);
	tft.drawRect(2, 20, 125, 90, ST7735_GREEN);
	printFloor("UG", determineWindowState(0, WINDOW_OPEN),
			determineWindowState(0, WINDOW_CLOSE),
			determineWindowState(0, WINDOW_ERROR), 30);
	printFloor("EG", determineWindowState(1, WINDOW_OPEN),
			determineWindowState(1, WINDOW_CLOSE),
			determineWindowState(1, WINDOW_ERROR), 60);
	printFloor("OG", determineWindowState(2, WINDOW_OPEN),
			determineWindowState(2, WINDOW_CLOSE),
			determineWindowState(2, WINDOW_ERROR), 90);

	tft.setCursor(85, 10);
	tft.setTextSize(1);
	tft.print(KNX_PA);
	tft.setCursor(2, 115);
	tft.print("Events seit Reset:");
	tft.setCursor(2, 125);
	tft.print(eventsSinceReset);

	if (lastEvent != 127) {
		tft.setCursor(2, 140);
		tft.print("Letztes Event: ");
		short state = windowState[lastEvent];

		tft.setTextColor(ST7735_YELLOW);

		if (state == WINDOW_OPEN) {
			paintOpenWindow(2, 150);
			tft.print(functionGA[lastEvent]);
		} else if (state == WINDOW_CLOSE) {
			paintClosedWindow(2, 150);
			tft.print(functionGA[lastEvent]);
		} else {
			paintContactFail(2, 150);
			tft.print(contactGA[lastEvent]);
		}
		tft.setCursor(15, 150);
		tft.setTextColor(ST7735_WHITE);
		tft.print(windowName[lastEvent]);
	}

}

void displayCurrentUpdate() {
	if (inErrorState) { // don't show windows in error state
		return;
	}
	if (!isOnlyCurrentUpdate) {
		tft.fillScreen(ST7735_BLACK);
		tft.setCursor(1, 1);
		tft.setTextSize(2);
		tft.setTextColor(ST7735_RED);
		tft.print("Strom");
		tft.setTextColor(ST7735_WHITE);
		tft.drawRect(2, 20, 125, 90, ST7735_GREEN);
		isOnlyCurrentUpdate = TRUE;
	}
	printCurrent("Netzteil 1", current[0], 30);
	printCurrent("Netzteil 2", current[1], 60);
	printCurrent("Netzteil 3", current[2], 90);
	printCurrent("Ref. Volt", uRef, 120);

}

void displayCisternUpdate() {
	if (inErrorState) { // don't show windows in error state
		return;
	}
	tft.fillScreen(ST7735_BLACK);
	tft.setCursor(1, 1);
	tft.setTextSize(2);
	tft.setTextColor(ST7735_RED);
	tft.print("Zisterne ");
	tft.setTextColor(ST7735_WHITE);
    tft.setCursor(2, 40);
	tft.print(cisternValue);
	tft.setCursor(2, 115);
	
	tft.setTextSize(1);
	tft.print("Events seit Reset:");
	tft.setCursor(2, 125);
	tft.print(cisternEventsSinceReset);

}


void displayTempSensors(short page) {
	if (inErrorState) { // don't show windows in error state
		return;
	}

	tft.fillScreen(ST7735_BLACK);
	tft.setCursor(1, 1);
	tft.setTextSize(2);
	tft.setTextColor(ST7735_RED);
	tft.print("Temp. ");
  tft.print(page);
	tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
	short i = page * 8;
	int posY = 18;
	while (i < (page + 1) *8){ // for the next 8 entries


		tft.setCursor(1, posY);
		char  name [24];
		getStringForSensorAddress(name, ds18b20_sensors[i].address);
		tft.print(name);
		tft.setCursor(50, posY +8 );
		tft.print(ds18b20_sensors[i].name);
		tft.setCursor(1, posY + 8);
		tft.print(ds18b20_sensors[i].temperatureGA);
		tft.setCursor(95, posY );
    if (ds18b20_sensors[i].lastTemperatur > -50){
		  tft.print(ds18b20_sensors[i].lastTemperatur);
    } else {
      tft.print("N/A");
    }
		posY += 18;
		i++;
	}

}


void displayUnknownTempSensors() {

	if (inErrorState) { // don't show windows in error state
		return;
	}
	int posY = 16;
	tft.fillScreen(ST7735_BLACK);
		tft.setCursor(1, 1);
		tft.setTextSize(2);
		tft.setTextColor(ST7735_RED);
		tft.print("Neu Temp.");
		tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
       for (int i = 0; i < 16; i++){
    	   tft.setCursor( 1, posY);
    	   char  name [24];
    	   getStringForSensorAddress(name, unknown_ds18b20_sensors[i]);
    	   tft.print(name);
		   posY += 9;
       }


}


void statusLedOn() {
	analogWrite(LED, 96);
}

byte brightness = 0;
void errorLedOn() {
	if (brightness < 128) {
		analogWrite(LED, brightness);
	} else {
		analogWrite(LED, (255 - brightness));
	}
	brightness += 8;
}
