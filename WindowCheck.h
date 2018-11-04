
#ifndef WINDOWCHECK_H_
#define WINDOWCHECK_H_
//init this functions
void initWindowCheck();

//return 1 if there was a change in between current state and last states
short checkContact (short no);

//trigger an event to knx for a single window
void sendSingleWindowEvent(short no, short type);

//iterative check all contacts return 1 if there was a change in at least one contact.
short checkAllContacts ();

#endif /* WINDOWCHECK_H_ */
