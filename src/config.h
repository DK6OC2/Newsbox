//Enter personal, Wifi and Message source configuration below
const char* WIFI_SSID1 = "SSID1";                  // Name des eigenen WLAN1
const char* WIFI_PASSWORD1 = "PW1";        // Passwort eigenes WLAN1
const char* WIFI_SSID2 = "Homenet";               // Name des eigenen WLAN2
const char* WIFI_PASSWORD2 = "nichtzuknacken";    // Passwort eigenes WLAN2
//more WLAN-Data if applicable with increasing number 
const char* WIFI_SSID3 = "SSID3";                    // Name des eigenen WLAN3
const char* WIFI_PASSWORD3 = "PW3";                // Passwort eigenes WLAN3
const char* WIFI_SSID4 = "SSID4";                    // Name des eigenen WLAN4
const char* WIFI_PASSWORD4 = "PW4";                // Passwort eigenes WLAN4
const char* WIFI_SSID5 = "SSID5";                    // Name des eigenen WLAN4
const char* WIFI_PASSWORD5 = "PW5";                // Passwort eigenes WLAN4

const String URL = "https://Dein Server/news.json"; //Link zum Server hier eintragen
const char* Rufzeichen = "CALL"; // eigenes Call
const char* Locator = "JO31xx"; // eigener Locator

//Hardware configuration, jeweils eine Alternative wählen

//Display-Typ 
#define DISPLAY_2004
//#define DISPLAY_OLED13
//#define DISPLAY_OLED096

//Buzzer-Typ
#define BUZZER_PASSIVE
//#define BUZZER_ACTIVE
