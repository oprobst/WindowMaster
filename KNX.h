
#ifndef KNX_H_
#define KXN_H_


void initKnx();

void sendSingleWindowEvent(short no, short newType, short oldType);

void sendCurrentUpdate (short no, int value);

#endif /* KXN_H_ */
