
#ifndef UI_H_
#define UI_H_



void displayInit();
void displayError (char* reason);
void displayCancelError();
void displayUpdate();
void displayCurrentUpdate ();
void displayCisternUpdate();
void displayTempSensors(short page);
void displayUnknownTempSensors();
void statusLedOn();
void errorLedOn();

void displayWindowState ();


#endif /* UI_H_ */
