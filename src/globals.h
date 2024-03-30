long news_id;
const char* news_date;
const char* news_topic;
const char* news_line1;
const char* news_line2;
const char* news_line3;
const uint32_t connectTimeoutMs = 10000;
String MacAddr;
String IP;
byte mac[6];   // byte-array for Mac-Adresse
String JSonMessage;
long old_id =999999; // Variable für Änderungsprüfung, muss beim ersten Durchlauf abweichen
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long interval  = 600000 ; //Nachricht aller 10m abfragen
int B_lastState = LOW;  // the previous state from the input pin
int B_currentState;     // the current reading from the input pin
bool fetchmessage = true ; // Flag für Zeitschleife des Nachrichtenabrufs
