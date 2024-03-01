//Enter personal, Wifi and Message source configuration below
const char* WIFI_SSID = "DL0DX";                  // Name des eigenen WLAN
const char* WIFI_PASSWORD = "r01-jo31jf"; // Passwort eigenes WLAN
const String URL = "https://newsbox.r04-afu.de/news.json"; //Link zum Server
const String Rufzeichen = "DD1NK"; // eigenes Call
const String Locator = "JO31kd"; // eigener Locator

//Hardware configuration, jeweils eine Alternative wählen

//Display-Typ 
#define DISPLAY_2004
//#define DISPLAY_OLED13
//#define DISPLAY_OLED096

//Buzzer-Typ
#define BUZZER_PASSIVE
//#define BUZZER_ACTIVE