#include "WindowMasterConfig.h"

const char* functionGA[ARRAYSIZE] = {
UG_Fenster_Offen_Sauna,
UG_Fenster_Offen_Technik,
UG_Fenster_Offen_Family_Tuer,
UG_Fenster_Offen_Werkstatt,
UG_Fenster_Offen_Garagenkeller,
EG_Fenster_Offen_Kueche,
EG_Fenster_Offen_Kueche_Terrasse,
EG_Fenster_Offen_WZ_Terrasse,
EG_Fenster_Offen_WZ_Balkon,
EG_Fenster_Offen_Gaeste_Garten,
EG_Fenster_Offen_Gaeste_Nord,
EG_Fenster_Offen_GaesteWc,
DG_Fenster_Offen_Kind_1,
DG_Fenster_Offen_Schlafzimmer_Wand,
DG_Fenster_Offen_Schlafzimmer_Gaube,
DG_Fenster_Offen_Flur_Gaube,
DG_Fenster_Offen_Bad_Gaube,
DG_Fenster_Offen_Bad_Wand,
DG_Fenster_Offen_Kind_2 };

const char* contactGA[ARRAYSIZE] = {
UG_Fenster_Sabo_Sauna,
UG_Fenster_Sabo_Technik,
UG_Fenster_Sabo_Family_Tuer,
UG_Fenster_Sabo_Werkstatt,
UG_Fenster_Sabo_Garagenkeller,
EG_Fenster_Sabo_Kueche,
EG_Fenster_Sabo_Kueche_Terrasse,
EG_Fenster_Sabo_WZ_Terrasse,
EG_Fenster_Sabo_WZ_Balkon,
EG_Fenster_Sabo_Gaeste_Garten,
EG_Fenster_Sabo_Gaeste_Nord,
EG_Fenster_Sabo_GaesteWc,
DG_Fenster_Sabo_Kind_1,
DG_Fenster_Sabo_Schlafzimmer_Wand,
DG_Fenster_Sabo_Schlafzimmer_Gaube,
DG_Fenster_Sabo_Flur_Gaube,
DG_Fenster_Sabo_Bad_Gaube,
DG_Fenster_Sabo_Bad_Wand,
DG_Fenster_Sabo_Kind_2 };

const short windowCount = ARRAYSIZE;
const short isOnfloor[] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
		2, 2 };

const char* currentGA[3] = {
  Current_Netzteil_1,
  Current_Netzteil_2,
  Current_Netzteil_3
};
float lastReadCurrent[3] = { 0, 0, 0 };
float current[3] = { 0, 0, 0 };
float uRef = 0;

// Pinbelegung:
/*
 * 6 5   Technikraum
4 3   Sauna
14 15 Terrasse UG
16 17 Werkstatt
22 23 Kueche Terrasse
24 25 Balkon Esszimmmer
26 27 Esszimmer Terrasse
69 29 Gäste Norden Richtung Garage ! 28 defekt. Mit 69 gebrueckt.
30 31 Schlafzimmer Richtung Tal
32 33 Bad OG neben Toilette
34 35 Kuechenfenster
36 37 Bad OG neben Waschbecken
38 39 Bad Eg
40 41 Nicos Zimmer
42 43 OG Flur Gaube
44 45 Schlafzimmer Richtung Strasse
46 47 Garagenkeller
48 49 Gaeste Richtung Garten
54 55 Bastis Zimmer

    
*/
//Arduino MEGA ports for comm: SPI >=50; Serial 0-1, 18-19; --> Do not use 18/19, 20/21 for window check.
const short statePin[] =    { 3, 5, 14, 16, 46, 34, 22, 26, 24, 48, 69, 38, 40, 44, 30, 42, 36, 32, 54 }; //, 56, 58, 60, 62, 64, 66  //PIN 28 defekt? Auf A15 (69) gelegt
const short functionPin[] = { 4, 6, 15, 17, 47, 35, 23, 27, 25, 49, 29, 39, 41, 45, 31, 43, 37, 33, 55 }; //, 57, 59, 61, 63, 65, 67
//const short statePin[] =    { 4, 6, 14, 16, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 54 }; //, 56, 58, 60, 62, 64, 66
//const short functionPin[] = { 3, 5, 15, 17, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 55 }; //, 57, 59, 61, 63, 65, 67
const char * windowName[] = { "Sauna", "Technikraum", "UG Terrassentuer",
		"Werkstatt", "Garagenkeller", "Kuechenfenster", "Kueche Tuer", "Ess.Terrasse",
		"Ess.Balkon", "Gaeste Garten", "Gaeste Nord", "Gaeste WC", "Nicos Zimmer",
		"Umkleide", "Schlafzimmer", "Flur Gaube", "Bad Gaube", "Bad Nord",
		"Bastis Zimmer" };



short windowState[] = { WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF,
WINDOW_UNDEF,
WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF,
WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF,
WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF };

long eventsSinceReset = 0;

short lastEvent = 127;

int timeUntilDisplayOff = TIME_UNTIL_DISPLAY_OFF;
