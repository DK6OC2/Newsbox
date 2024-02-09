#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <config.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);                 // für andere Displays oder // Adressen anzupassen

int MESSAGE_LENGTH = 100;     // Länge der Nachricht
const int DISPLAY_WIDTH = 16; // Definition fürs Display
String payload; // Variable für Nachricht
String old_payload; // Variable für Änderungsprüfung
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long interval  = 600000 ; //Nachricht aller 10m abfragen

bool fetchmessage = true ; // Flag für Zeitschleife des Nachrichtenabrufs

// LCD initialisieren und Starttexte an-zeigen
void setup()
{
  // Start Serial
  Serial.begin(9600);
  Serial.setTimeout(3000); // miliseconds to wait (50 mili) for USB Data. Default 1000

  // LCD
  lcd.begin(0, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(Rufzeichen);
  lcd.setCursor(10, 0);
  lcd.print(Locator);
  lcd.setCursor(0, 1);
  lcd.print(OV);

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


  if(WiFi.status()== WL_CONNECTED){
     WiFiClient client;
     HTTPClient http;

}

void loop()
{
  if (fetchmessage)
    {
    Serial.print("Fetching ... ");
    http.begin(client, URL); // http.begin(URL);
    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      String payload = http.getString();
      Serial.println(payload); // Print re-sponse

      payload_length = payload.length();
      Serial.println("Laenge: ");
      Serial.println(payload_length);
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    fetchmessage = false;
    } 
    
  if (payload != old_payload)  //bei neuer Nachricht auf dem Server
     {
      // Nachricht generieren, aufteilen und anzeigen
      if (payload_length > 16)
        {
        int Trennen = payload.indexOf(";");
        String MESSAGE1 = payload.substring(0, (Trennen));
        int temp_var = payload.indexOf(";");
        String MESSAGE2 = payload.substring(temp_var + 1);
        lcd.setCursor(0, 0);
        lcd.print(MESSAGE1);
        lcd.setCursor(0, 1);
        lcd.print(MESSAGE2);
        }
      else
        {
        lcd.setCursor(0, 0);
        lcd.print(payload);
        }
      old_payload = payload ; //Sichere alte Nachricht zum Vergleich
     } 
    if (millis() - startTime >= interval)
      {
        fetchmessage = true; //wenn Interval um hole neue Nachricht vom Server
      }
