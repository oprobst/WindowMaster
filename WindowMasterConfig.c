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
		2, 2, 2 };

//Arduino MEGA ports for comm: SPI >=50; Serial 0-1, 18-19; --> Do not use 18/19 and 38/39 for window check.
const short functionPin[] = { 4, 5, 30, 31, 32, 33, 34, 35, 36, 37, 40, 41, 42,
		43, 44, 45, 46, 47, 48 };
const short statePin[] = { 6, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 20, 21,
		22, 23, 24, 25, 26 };
const char * windowName[] = { "Sauna", "Technikraum", "Terrassentuer",
		"Werkstatt", "Garagenkeller", "Kueche", "Kueche Tuer", "WZ Terrasse",
		"WZ Balkon", "Gaeste Garten", "GaesteNord", "Gaeste WC", "Nicos Zimmer",
		"Umkleide", "Schlafzimmer", "Flur Gaube", "Bad Gaube", "Bad Nord",
		"Bastis Zimmer" };

short windowState[] = { WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF,
		WINDOW_UNDEF,
		WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF,
		WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF,
		WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF, WINDOW_UNDEF };

long eventsSinceReset = 0;

short lastEvent = 127;

extern int timeUntilDisplayOff = TIME_UNTIL_DISPLAY_OFF;
