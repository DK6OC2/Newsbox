#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <config.h>
#include <ArduinoJson.h>

//LiquidCrystal_I2C lcd(0x27, 16, 2);                 // für 2 Zeilen Displays
LiquidCrystal_I2C lcd(0x27, 20, 4);                 // für 4 Zeilen Displays

//const int DISPLAY_WIDTH = 16; // Definition fürs Display
const int DISPLAY_WIDTH = 20; // Definition fürs Display, wäre das so richtig?
byte mac[6];   // byte-array für Mac-Adresse
String JSonMessage;
JsonDocument doc; //JSON Opject
//Variablen für Nachricht
long news_id;
const char* news_datum;
const char* news_topic;
const char* news_zeile1;
const char* news_zeile2;
const char* news_zeile3;
long old_id; // Variable für Änderungsprüfung, muss beim ersten Durchlauf abweichen
String MacAddr;
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long interval  = 600000 ; //Nachricht aller 10m abfragen
int LED_PIN = 2; //Anschluss für LED
int BUTTON_PIN = 4; //Anschluss für Bestätigungstaste
int B_lastState = LOW;  // the previous state from the input pin
int B_currentState;     // the current reading from the input pin
bool fetchmessage = true ; // Flag für Zeitschleife des Nachrichtenabrufs
WiFiClientSecure *client = new WiFiClientSecure ; // initialisieren des WifiClients mit SSL


// LCD initialisieren und Starttexte anzeigen
void setup()
{
  // Start Serial
  Serial.begin(9600);
  Serial.setTimeout(1000); // miliseconds to wait (50 mili) for USB Data. Default 1000

  // LCD
  //lcd.begin(0, 2); //je nach Display ggf. statt init()
  lcd.init();
  lcd.backlight();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  WiFi.macAddress(mac);
  MacAddr += String(mac[5],HEX);
  MacAddr += String(mac[4],HEX);
  MacAddr += String(mac[3],HEX);
  MacAddr += String(mac[2],HEX);
  MacAddr += String(mac[1],HEX);
  MacAddr += String(mac[0],HEX);
  Serial.println("MAC: "+MacAddr);
  lcd.setCursor(0, 0);
  lcd.print(Rufzeichen);
  lcd.setCursor(10, 0);
  lcd.print(Locator);
  lcd.setCursor(0, 1);
  lcd.print("Mac:"+MacAddr);
  delay(3000);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);
}

void loop()
{
  client->setInsecure(); // ignoriere SSL-Cert
  
  HTTPClient http ; //Webclient starten
  //Nachricht holen, wenn erster Durchlauf oder Intervall abgelaufen ist
  if ((fetchmessage) && (WiFi.status() == WL_CONNECTED))
    {
    Serial.println("Fetching ... "+URL);
    http.begin(*client, URL+"?mac="+MacAddr+"&call="+Rufzeichen); //Verbindung zum Server aufbauen
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      JSonMessage = http.getString(); // Abruf JsonObject
      DeserializationError err = deserializeJson(doc, JSonMessage); //Parse message   
      if (err) 
      {
           Serial.print(F("deserializeJson() failed with code "));
           Serial.println(err.f_str());
      }
      else
      {
        news_id = doc["ID"];
        news_datum = doc["Datum"];
        news_topic = doc["Message"]["Topic"];
        news_zeile1 = doc["Message"]["Zeile1"];
        news_zeile2 = doc["Message"]["Zeile2"];
        news_zeile3 = doc["Message"]["Zeile3"];
      }
    } 
    else 
    {
        //Fehlermeldung für Debugzwecke
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    fetchmessage = false; // Pause für den Abruf
    startTime = millis(); // Startzeit für Abruf neusetzten auf aktuellen Stand
    } 
  if (news_id != old_id)  //bei neuer Nachricht auf dem Server
     {
      lcd.clear();  // Display löschen für neue Nachrichte 
      //Schreibe Nachricht aufs Display wenn 2-zeilig
      lcd.setCursor(0, 0);
      lcd.print(news_topic);
      lcd.setCursor(10, 0);
      lcd.print(news_datum);
      lcd.setCursor(0, 1);
      lcd.print(news_zeile1);
      lcd.setCursor(0, 2);
      lcd.print(news_zeile2);
      lcd.setCursor(0, 3);
      lcd.print(news_zeile3);
      old_id = news_id; //Sichere alte Nachricht zum Vergleich
      digitalWrite(LED_PIN, HIGH); // Schalte LED ein
     }
    B_currentState = digitalRead(BUTTON_PIN);
    if (B_lastState == HIGH && B_currentState == LOW)
    digitalWrite(LED_PIN, LOW); //Schalte LED aus wenn Taste gedrückt
    // save the the last state of the button
    B_lastState = B_currentState;  
    if (millis() - startTime >= interval)
      {
        fetchmessage = true; //wenn Interval um, hole neue Nachricht vom Server in der nächsten loop
      }
}
