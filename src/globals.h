long news_id;
const char* news_date;
const char* news_topic;
const char* news_subject;
const char* news_line1;
const char* news_line2;
const char* news_line3;
const uint32_t connectTimeoutMs = 10000;
String MacAddr;
String IP;
byte mac[6];   // byte-array for Mac-Adresse
String JSonMessage;
long old_id = 999999; // Variable für Änderungsprüfung, muss beim ersten Durchlauf abweichen
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long readNewsInterval  = 600000 ; //Nachricht aller 10m abfragen
int max_news; //Anzahl der Nachrichten in einem JSon-Paket
int akt_news = 0; //aktuell angezeigte Nachricht
bool fetchmessage = true ; // Flag für Zeitschleife des Nachrichtenabrufs
bool first_run; // Workaround für Buzzer_bug beim LILIGO-t-display-s3
// extern const int LED_PIN; // Global Definition for LED_PIN
const int LED_PIN = 2; //Anschluss für LED

/* Board/Display specific includes, defined in PlatformIO.ini */
#ifdef AZ_DELIVERY_DEVKIT_V4
  #include <boards/az-delivery-devkit-v4.h>
#endif
#ifdef LILYGO_T_DISPLAY_S3
  #include <boards/lilygo-t-displays3.h>
#endif
#ifdef DISPLAY_2004
  #include <displays/lcd2004.h>
#endif
#ifdef DISPLAY_EPAPER29BW
  #include <displays/epaper29_bw.h>
#endif
#ifdef DISPLAY_OLED13
  #include <displays/oled13.h>
#endif
#ifdef DISPLAY_OLED096
  #include <displays/oled096.h>
#endif
#ifdef DISPLAY_OLED096_SSD1306
  #include <displays/oled096_SSD1306.h>
#endif
#ifdef DISPLAY_TFT
  #include <displays/tft.h>
#endif
