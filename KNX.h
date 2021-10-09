
#ifndef KNX_H_
#define KXN_H_


void initKnx();

void sendSingleWindowEvent(short no, short newType, short oldType);

void sendCurrentUpdate (short no, float value);

void sendCisternUpdate (int value);

void sendTemperatureUpdate (char * addr, float value);

#endif /* KXN_H_ */
