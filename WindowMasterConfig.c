#include <Arduino.h>
#include "WindowMasterConfig.h"

/*
 * Current measurement settings
 */
const char* currentGA[3] = {
  Current_Netzteil_1,
  Current_Netzteil_2,
  Current_Netzteil_3
};
float lastReadCurrent[3] = { 0, 0, 0 };
float current[3] = { 0, 0, 0 };
float uRef = 0;

/*
 * Window Settings
 */

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

// Pinbelegung:
/*
 * 6 5   Technikraum
4 3   Sauna
A14 A13 Terrasse UG
A12 A11 Werkstatt
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
const short statePin[] =    { 3, 5, A14, A12, 46, 34, 22, 26, 24, 48, 69, 38, 40, 44, 30, 42, 36, 32, 54 }; //, 56, 58, 60, 62, 64, 66  //PIN 28 defekt? Auf A15 (69) gelegt
const short functionPin[] = { 4, 6, A13, A11, 47, 35, 23, 27, 25, 49, 29, 39, 41, 45, 31, 43, 37, 33, 55 }; //, 57, 59, 61, 63, 65, 67

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

/*
 * Cistern settings
 */
long cisternEventsSinceReset = 0;
int cisternValue = -1;


/*
 *  Definitions for Dallas Temp sensors
 */

/*
 *  Define all ds18b20_sensors here.
 *  First a human readable name and next the physical address of the sensor in hex.
 */
struct ds18b20_sensor ds18b20_sensors [] = {
//  {"Sensor Vorne" , {0x28, 0xFF, 0xA4, 0x20, 0x54, 0x17, 0x04, 0x12}, "16/1/0"},
//  {"Sensor Hinten", {0x28, 0xFF, 0x76, 0x20, 0x60, 0x17, 0x04, 0x8F}, ""},
  {"Schaltschrank", {0x28, 0xFF, 0x72, 0x80, 0x53, 0x17, 0x04, 0x03}, ""},
  {"Serverschrank", {0x28, 0xFF, 0xBC, 0x7D, 0x53, 0x17, 0x04, 0x6F}, ""},
  {"Technikraum",	{0x28, 0xFF, 0x1E, 0x36, 0x82, 0x16, 0x05, 0x83}, ""},
  {"Vorratskeller", {0x28, 0xFF, 0xD8, 0x4D, 0x54, 0x17, 0x04, 0x53}, ""},
  {"BadUg",	        {0x28, 0xFF, 0x7C, 0x2C, 0x60, 0x17, 0x04, 0x20}, ""},
  {"vorlaufUG",	    {0x28, 0xFF, 0x1E, 0xAD, 0x53, 0x17, 0x04, 0xDE}, ""},
  {"ruecklaufUG",	{0x28, 0xFF, 0x55, 0x77, 0x83, 0x16, 0x04, 0x80}, ""},
  {"ugHk1Vorl",	    {0x28, 0xFF, 0x4D, 0xEC, 0x53, 0x17, 0x04, 0x6A}, ""},
  {"ugHk1Rueckl",	{0x28, 0xFF, 0x73, 0x07, 0x83, 0x16, 0x05, 0x12}, ""},
  {"ugHk2Vorl",	    {0x28, 0xFF, 0x36, 0x60, 0x53, 0x17, 0x04, 0xBC}, ""},
  {"ugHk2Rueckl",	{0x28, 0xEE, 0x09, 0x0A, 0x25, 0x16, 0x01, 0x75}, ""},
  {"ugHk3Vorl",    	{0x28, 0xFF, 0x8B, 0x72, 0x53, 0x17, 0x04, 0x20}, ""},
  {"ugHk3Rueckl",	{0x28, 0xFF, 0x56, 0xA1, 0x81, 0x16, 0x03, 0x0A}, ""},
  {"ugHk4Vorl",	    {0x28, 0xFF, 0x01, 0x1A, 0x60, 0x17, 0x04, 0xE0}, ""},
  {"ugHk4Rueckl",	{0x28, 0xFF, 0x9D, 0x22, 0x60, 0x17, 0x04, 0x22}, ""},
  {"ugHk5Vorl",	    {0x28, 0xFF, 0x1C, 0xAD, 0x53, 0x17, 0x04, 0x5D}, ""},
  {"ugHk5Rueckl",	{0x28, 0xFF, 0xC1, 0x2A, 0x60, 0x17, 0x04, 0x8A}, ""},
  {"ugHk6Vorl",	    {0x28, 0xFF, 0x77, 0xA9, 0x53, 0x17, 0x04, 0x32}, ""},
  {"ugHk6Rueckl",	{0x28, 0xFF, 0x3D, 0x24, 0x60, 0x17, 0x04, 0x18}, ""},
  {"vorlaufEG",	    {0x28, 0xFF, 0xDA, 0x2E, 0x60, 0x17, 0x04, 0x88}, ""},
  {"ruecklaufEG",	{0x28, 0xFF, 0x37, 0xAC, 0x53, 0x17, 0x04, 0x5A}, ""},
  {"vorlaufDG",	    {0x28, 0xFF, 0x62, 0xA7, 0x53, 0x17, 0x04, 0x89}, ""},
  {"ruecklaufDG",	{0x28, 0xFF, 0x1F, 0xCD, 0x53, 0x17, 0x04, 0x83}, ""},
  {"dgHk1Rueckl",   {0x28, 0xFF, 0x7F, 0x06, 0x83, 0x16, 0x05, 0xBC}, ""},
  {"dgHk2Rueckl",  	{0x28, 0xFF, 0xF3, 0x01, 0x54, 0x17, 0x04, 0x29}, ""},
  {"dgHk3Rueckl",  	{0x28, 0xFF, 0x2F, 0xAB, 0x53, 0x17, 0x04, 0x93}, ""},
  {"ugHk2Vorl",	    {0x28, 0xFF, 0x36, 0x60, 0x53, 0x17, 0x04, 0xBC}, ""},
  {"ugHk2Rueckl",  	{0x28, 0xEE, 0x09, 0x0A, 0x25, 0x16, 0x01, 0x75}, ""},
  {"ugHk3Vorl",	    {0x28, 0xFF, 0x8B, 0x72, 0x53, 0x17, 0x04, 0x20}, ""},
  {"ugHk3Rueckl",  	{0x28, 0xFF, 0x56, 0xA1, 0x81, 0x16, 0x03, 0x0A}, ""},
  {"ugHk4Vorl",	    {0x28, 0xFF, 0x01, 0x1A, 0x60, 0x17, 0x04, 0xE0}, ""},
  {"ugHk4Rueckl",	{0x28, 0xFF, 0x9D, 0x22, 0x60, 0x17, 0x04, 0x22}, ""},
  {"ugHk5Vorl",	    {0x28, 0xFF, 0x1C, 0xAD, 0x53, 0x17, 0x04, 0x5D}, ""},
  {"ugHk5Rueckl",  	{0x28, 0xFF, 0xC1, 0x2A, 0x60, 0x17, 0x04, 0x8A}, ""},
  {"ugHk6Vorl",	    {0x28, 0xFF, 0x77, 0xA9, 0x53, 0x17, 0x04, 0x32}, ""},
  {"ugHk6Rueckl",  	{0x28, 0xFF, 0x3D, 0x24, 0x60, 0x17, 0x04, 0x18}, ""}
};




