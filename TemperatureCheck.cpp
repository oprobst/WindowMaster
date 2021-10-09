#include "TemperatureCheck.h"
#include <Arduino.h>
#include "WindowMasterConfig.h"
//#include "UI.h"
#include "KNX.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass oneWire reference to Dallas Temperature.
DallasTemperature dallasSensors(&oneWire);

//Number of ds18b20_sensors to be determined
short amountKnownSensors = 36; //sizeof(ds18b20_sensors) / sizeof(ds18b20_sensor);

unsigned long lastMeasurement = 0;

/*
 * Read the byte address and translate it into a human readable string containing
 * hex values.
 * buf: buffer to write the created string into. Must be of size 24!
 * addr: The address to parse.
 */
void getStringForSensorAddress(char *buf, byte *addr) {
	unsigned char *pin = addr;
	const char *hex = "0123456789ABCDEF";
	char *pout = buf;
	for (; pin < addr + BYTE_SIZE_ADDRESS; pout += 3, pin++) {
		pout[0] = hex[(*pin >> 4) & 0xF];
		pout[1] = hex[*pin & 0xF];
		pout[2] = ':';
	}
	pout[-1] = 0;
}


/* Helper function:
 *  Compare content of two arrays with the length 'len'
 */
boolean array_cmp(byte *a, short *b, int len) {
	for (uint8_t i = 0; i < len; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

/*
 * Iterate through the ds18b20_sensors array and compare hard coded sensor addresses with the one read from the sensor.
 * Return an const char * with the name stored in the array.
 */
const short getIndexForSensorAddress(byte *addr) {
	for (uint8_t j = 0; j < amountKnownSensors; j++) {
		if (array_cmp(addr, ds18b20_sensors[j].address, BYTE_SIZE_ADDRESS)
				== true) {
			return j;
		}
	}
	return 255;
}

void initTemperatureCheck() {
	dallasSensors.begin();
}

void requestTemperatures() {

 //dallasSensors.begin();
 
	short indexUnknownSensors = 0;
	if (millis() > lastMeasurement + TEMPERATURE_MEASURE_INTERVALL) {
      for (short i = 0; i < 16; i++){
         for (short j = 0; j < 8; j++){ 
           unknown_ds18b20_sensors [i][j]= 0;
          }
        }
		lastMeasurement = millis();

		byte addr[BYTE_SIZE_ADDRESS];

		dallasSensors.requestTemperatures(); // Send the command to get temperature readings

		for (uint8_t i = 0; i < dallasSensors.getDeviceCount(); i++) {
			float temp = dallasSensors.getTempCByIndex(i);
			dallasSensors.getAddress(addr, i);
			short index = getIndexForSensorAddress(addr);

			if (index < 255){
			ds18b20_sensors[index].lastTemperatur = temp;
			sendTemperatureUpdate(ds18b20_sensors[index].temperatureGA, temp);
			} else { // sensor not in sensors list
				for (short i = 0; i<8; i++){
					unknown_ds18b20_sensors[indexUnknownSensors][i] = addr[i];
				}
       indexUnknownSensors++;
        if(indexUnknownSensors > 15) return;
			}

		}
	}
}
