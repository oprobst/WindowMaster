#ifndef WINDOWMASTERCONFIG_H_
#define WINDOWMASTERCONFIG_H_

//USER Setup
#define KNX_PA               "1.1.230"

#define UG_Fenster_Offen_Sauna 				"9/1/0"
#define UG_Fenster_Offen_Technik		 	"9/1/1"
#define UG_Fenster_Offen_Family_Tuer		"9/1/2"
#define UG_Fenster_Offen_Werkstatt			"9/1/3"
#define UG_Fenster_Offen_Garagenkeller		"9/1/4"
#define EG_Fenster_Offen_Kueche				"9/1/5"
#define EG_Fenster_Offen_Kueche_Terrasse	"9/1/6"
#define EG_Fenster_Offen_WZ_Terrasse		"9/1/7"
#define EG_Fenster_Offen_WZ_Balkon 			"9/1/8"
#define EG_Fenster_Offen_Gaeste_Garten		"9/1/9"
#define EG_Fenster_Offen_Gaeste_Nord		"9/1/10"
#define EG_Fenster_Offen_GaesteWc			"9/1/11"
#define DG_Fenster_Offen_Kind_1				"9/1/12"
#define DG_Fenster_Offen_Schlafzimmer_Wand	"9/1/13"
#define DG_Fenster_Offen_Schlafzimmer_Gaube	"9/1/14"
#define DG_Fenster_Offen_Flur_Gaube			"9/1/15"
#define DG_Fenster_Offen_Bad_Gaube			"9/1/16"
#define DG_Fenster_Offen_Bad_Wand			"9/1/17"
#define DG_Fenster_Offen_Kind_2				"9/1/18"
#define UG_Fenster_Sabo_Sauna				"9/0/0"
#define UG_Fenster_Sabo_Technik				"9/0/1"
#define UG_Fenster_Sabo_Family_Tuer			"9/0/2"
#define UG_Fenster_Sabo_Werkstatt			"9/0/3"
#define UG_Fenster_Sabo_Garagenkeller		"9/0/4"
#define EG_Fenster_Sabo_Kueche				"9/0/5"
#define EG_Fenster_Sabo_Kueche_Terrasse		"9/0/6"
#define EG_Fenster_Sabo_WZ_Terrasse			"9/0/7"
#define EG_Fenster_Sabo_WZ_Balkon			"9/0/8"
#define EG_Fenster_Sabo_Gaeste_Garten		"9/0/9"
#define EG_Fenster_Sabo_Gaeste_Nord			"9/0/10"
#define EG_Fenster_Sabo_GaesteWc			"9/0/11"
#define DG_Fenster_Sabo_Kind_1				"9/0/12"
#define DG_Fenster_Sabo_Schlafzimmer_Wand	"9/0/13"
#define DG_Fenster_Sabo_Schlafzimmer_Gaube	"9/0/14"
#define DG_Fenster_Sabo_Flur_Gaube			"9/0/15"
#define DG_Fenster_Sabo_Bad_Gaube			"9/0/16"
#define DG_Fenster_Sabo_Bad_Wand			"9/0/17"
#define DG_Fenster_Sabo_Kind_2 				"9/0/18"

#define WINDOW_OPEN  0
#define WINDOW_CLOSE 1
#define WINDOW_ERROR 2
#define WINDOW_UNDEF 3

#define FALSE 0
#define TRUE  1
#define UNDEF 2

#define TFT_PIN_CS   12 // Display CS
#define TFT_PIN_DC   11  // Display DC
#define TFT_PIN_RST  10 // Display RST

#define LED 9 // PIN Error LED
#define LED_DISPLAY 13 // PIN display backlight
#define USER_SWITCH 8

#define ARRAYSIZE 19

#define TIME_UNTIL_DISPLAY_OFF 120

extern const char * contactGA[ARRAYSIZE];

extern const char * functionGA[ARRAYSIZE];

extern const short isOnfloor[ARRAYSIZE];

extern const short functionPin[ARRAYSIZE];
extern const short statePin[ARRAYSIZE];
extern const char * windowName[ARRAYSIZE];

extern short windowState[ARRAYSIZE];

extern long eventsSinceReset;

extern short lastEvent;

extern int timeUntilDisplayOff;

#endif /* WINDOWMASTERCONFIG_H_ */
