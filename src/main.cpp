/*
******************************************************************************************************
* Newsbox-Projekt des OVV R01 und R04                                                                *
******************************************************************************************************
*/
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <config.h>
#include <ArduinoJson.h>

WiFiMulti wifiMulti;

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 20000;

#ifdef DISPLAY_2004   // für 4 Zeilen/20 Zeichen Displays
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);                 
#endif
#if defined (DISPLAY_OLED096) || defined (DISPLAY_OLED13) // für 0,96 oder 1,3" OLEDS
#include "U8g2lib.h"
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);
#endif
byte mac[6];   // byte-array for Mac-Adresse
String JSonMessage;
JsonDocument doc; //JSON Opject
//variables for message 
long news_id;
const char* news_date;
const char* news_topic;
const char* news_line1;
const char* news_line2;
const char* news_line3;
long old_id; // Variable für Änderungsprüfung, muss beim ersten Durchlauf abweichen

String MacAddr;
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long interval  = 600000 ; //Nachricht aller 10m abfragen
int LED_PIN = 2; //Anschluss für LED
int BUTTON_PIN = 4; //Anschluss für Bestätigungstaste
int buzzer_pin = 5; //Anschluss für Buzzer
int B_lastState = LOW;  // the previous state from the input pin
int B_currentState;     // the current reading from the input pin
bool fetchmessage = true ; // Flag für Zeitschleife des Nachrichtenabrufs
WiFiClientSecure *client = new WiFiClientSecure ; // initialisieren des WifiClients mit SSL
bool new_wifi = true ; // Flag für neue Wifi-Verbindung nach Verbindungsverlust

// LCD initialisieren und Starttexte anzeigen
void setup()
{
  // Start Serial
  Serial.begin(9600);
  Serial.setTimeout(1000); // miliseconds to wait (50 mili) for USB Data. Default 1000

  // LCD
  #ifdef DISPLAY_2004
  lcd.init();
  lcd.backlight();
  #endif
  #ifdef DISPLAY_OLED096
  oled.begin();
  oled.clearBuffer();
  oled.setFont(u8g2_font_resoledbold_tr);
  #endif
  #ifdef DISPLAY_OLED13
  oled.begin();
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x13_tr);
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.macAddress(mac);
  MacAddr += String(mac[5],HEX);
  MacAddr += String(mac[4],HEX);
  MacAddr += String(mac[3],HEX);
  MacAddr += String(mac[2],HEX);
  MacAddr += String(mac[1],HEX);
  MacAddr += String(mac[0],HEX);
  Serial.println("MAC: "+MacAddr);
  #ifdef DISPLAY_2004
  lcd.setCursor(0, 0);
  lcd.print("Newsbox-Projekt");
  lcd.setCursor(0,1);
  lcd.print(Rufzeichen);
  lcd.setCursor(10, 1);
  lcd.print(Locator);
  lcd.setCursor(0, 2);
  lcd.print("Mac: "+MacAddr);
  #endif
  #ifdef DISPLAY_OLED096
  oled.drawStr(0,15, "Newsbox-Projekt");
  oled.drawStr(0,30, Rufzeichen);
  oled.drawStr(70,30, Locator);
  oled.drawStr(0,40, "Mac: ");
  oled.drawStr(25,40, MacAddr.c_str());
  oled.sendBuffer();
  #endif
  #ifdef DISPLAY_OLED13
  oled.drawStr(2,15, "Newsbox-Projekt");
  oled.drawStr(2,36, Rufzeichen);
  oled.drawStr(70,36, Locator);
  oled.drawStr(2,50, "Mac: ");
  oled.drawStr(27,50, MacAddr.c_str());
  oled.sendBuffer();
  #endif
  delay(3000);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Add list of wifi networks
  wifiMulti.addAP(WIFI_SSID1, WIFI_PASSWORD1);
  wifiMulti.addAP(WIFI_SSID2, WIFI_PASSWORD2);
  wifiMulti.addAP(WIFI_SSID3, WIFI_PASSWORD3);
  wifiMulti.addAP(WIFI_SSID4, WIFI_PASSWORD4);
  wifiMulti.addAP(WIFI_SSID5, WIFI_PASSWORD5);

 // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } 
  else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }

  // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
  Serial.println("Connecting Wifi...");
  if(wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    new_wifi = false;
  }
  
}

void loop()
{
   if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    if (new_wifi)
      {
    Serial.print("WiFi connected: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.RSSI());
    new_wifi = false;
      }
  }
  else {
    Serial.println("WiFi not connected!");
    new_wifi = true;
  }
  client->setInsecure(); // ignoriere SSL-Cert
  
  HTTPClient http ; //Webclient starten
  //Nachricht holen, wenn erster Durchlauf oder Intervall abgelaufen ist
  if ((fetchmessage) && (WiFi.status() == WL_CONNECTED))
    {
    Serial.println("Fetching ... "+URL);
    http.begin(*client, URL+"?mac="+MacAddr+"&call="+Rufzeichen+"&loc="+Locator); //Verbindung zum Server aufbauen
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      JSonMessage = http.getString(); // Abruf JsonObject
      DeserializationError err = deserializeJson(doc, JSonMessage); //Parse message   
      if (err) 
      {
           Serial.print(F("deserializeJson() failed with code: "));
           Serial.println(err.f_str());
           news_id = 10;
           news_topic = "ERROR";
           news_line1 = "";
           news_line2 = "Nachrichtenfehler!";
           news_line3 = "Bitte warten ...";
      }
      else
      {
        news_id = doc["ID"];
        news_date = doc["Datum"];
        news_topic = doc["Message"]["Topic"];
        news_line1 = doc["Message"]["Zeile1"];
        news_line2 = doc["Message"]["Zeile2"];
        news_line3 = doc["Message"]["Zeile3"];
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
      #ifdef DISPLAY_2004
      lcd.clear();  // Display löschen für neue Nachrichte 
      //Schreibe Nachricht aufs Display wenn 2-zeilig
      lcd.setCursor(0, 0);
      lcd.print(news_topic);
      lcd.setCursor(10, 0);
      lcd.print(news_date);
      lcd.setCursor(0, 1);
      lcd.print(news_line1);
      lcd.setCursor(0, 2);
      lcd.print(news_line2);
      lcd.setCursor(0, 3);
      lcd.print(news_line3);
      #endif
      #ifdef DISPLAY_OLED096
      oled.clear();  // Display löschen für neue Nachrichte 
      //Schreibe Nachricht aufs Display wenn 2-zeilig
      oled.drawStr(0,15, news_topic);
      oled.drawStr(60,15, news_date);
      oled.drawStr(0,30, news_line1);
      oled.drawStr(0,40, news_line2);
      oled.drawStr(0,50, news_line3);
      oled.sendBuffer();
      #endif
      #ifdef DISPLAY_OLED13
      oled.clear();  // Display löschen für neue Nachrichte 
      //Schreibe Nachricht aufs Display wenn 2-zeilig
      oled.drawStr(2,15, news_topic);
      oled.drawStr(65,15, news_date);
      oled.drawStr(2,36, news_line1);
      oled.drawStr(2,50, news_line2);
      oled.drawStr(2,63, news_line3);
      oled.sendBuffer();
      #endif
      old_id = news_id; //Sichere alte Nachrichten-id zum Vergleich
      digitalWrite(LED_PIN, HIGH); // Schalte LED ein
      #ifdef BUZZER_PASSIVE
      tone(buzzer_pin, 1000, 1000);
      #endif
      #ifdef BUZZER_ACTIVE
      digitalWrite(buzzer_pin, HIGH);
      delay(1000);
      digitalWrite(buzzer_pin, LOW);
      #endif      
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
