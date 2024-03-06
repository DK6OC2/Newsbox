//Enter personal, Wifi and Message source configuration below
const char* WIFI_SSID = "DL0DX";                  // Name des eigenen WLAN1
const char* WIFI_PASSWORD = "r01-jo31jf";        // Passwort eigenes WLAN1
//const char* WIFI_SSID[2] = "Homenet";               // Name des eigenen WLAN2
//const char* WIFI_PASSWORD[2] = "nichtzuknacken";    // Passwort eigenes WLAN2
//more WLAN-Data if applicable with increasing number 
//const char* WIFI_SSID[3] = "";                    // Name des eigenen WLAN3
//const char* WIFI_PASSWORD[3] = "";                // Passwort eigenes WLAN3

const String URL = "https://newsbox.r04-afu.de/news.json"; //Link zum Server
const char* Rufzeichen = "DD1NK"; // eigenes Call
const char* Locator = "JO31kd"; // eigener Locator

//Hardware configuration, jeweils eine Alternative wählen

//Display-Typ 
#define DISPLAY_2004
//#define DISPLAY_OLED13
//#define DISPLAY_OLED096

//Buzzer-Typ
#define BUZZER_PASSIVE
//#define BUZZER_ACTIVE