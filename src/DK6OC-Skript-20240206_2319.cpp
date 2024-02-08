// Bibliotheken laden, WLAN einstellen und Links festlegen
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);                 // für andere Displays oder // Adressen anzupassen
const char* WIFI_SSID = "IKS-2.4";                  // Name des eigenen WLAN
const char* WIFI_PASSWORD = "90563549770968987168"; // Passwort eigenes WLAN
const char* host = "www.r04-afu.de";                // Link zum OV-Server

const String URL = "http:/www.r04-afu.de/info.txt";
const String OV = "News";          // Individueller Text
const String Rufzeichen = "DK6OC"; // eigenes Call
const String payload = "Nachricht laden";
const String MESSAGE1;
const String MESSAGE2;

int MESSAGE_LENGTH = 100;     // Länge der Nachricht
const int DISPLAY_WIDTH = 16; // Definition fürs Display

int payload_length; // Variable für die Länge des Textes vom Server definieren
int cnt = 3600;     // Zählervariable

String Locator = "JO31ke"; // eigener Locator

// feste IP einstellen
IPAddress ip(192, 168, 2, 8);       // feste IP-Adresse im eigenen Netz
IPAddress gateway(192, 168, 2, 1);  // Gateway-Adresse im eigenen Netz
IPAddress subnet(255, 255, 255, 0); // Subnet IP-Adresse im eigenen Netz

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

    // Abfrage der Info.txt Datei auf OV-Server mit http-Client
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
  }  
}

void loop()
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

  // WHAT THE FUCK ??????
  delay(12000);
  ESP.restart();
}
