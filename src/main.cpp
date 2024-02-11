#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <config.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);                 // für andere Displays oder // Adressen anzupassen

const int DISPLAY_WIDTH = 16; // Definition fürs Display
String payload; // Variable für Nachricht
String old_payload = "empty"; // Variable für Änderungsprüfung, muss beim ersten Durchlauf abweichen
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long interval  = 600000 ; //Nachricht aller 10m abfragen
//SSL-Zertifikat
//const char* rootCACertificate = \
     "-----BEGIN CERTIFICATE-----\n" \
     "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\n" \
     "-----END CERTIFICATE-----\n";

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
  lcd.setCursor(0, 0);
  lcd.print(Rufzeichen);
  lcd.setCursor(10, 0);
  lcd.print(Locator);
  lcd.setCursor(0, 1);
  lcd.print(OV);
//WiFi-Parameter setzen und verbinden
 if (!WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS)) {
  Serial.println("STA Failed to configure");
}
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

}

void loop()
{
  //client->setCACert(rootCACertificate); // Set Server-Certificate
  client->setInsecure(); // ignoriere SSL-Cert
  
  HTTPClient http ; //Webclient starten
  //Nachricht holen, wenn erster Durchlauf oder Intervall abgelaufen ist
  if ((fetchmessage) && (WiFi.status() == WL_CONNECTED))
    {
    Serial.println("Fetching ... "+URL);
    http.begin(*client, URL); //Verbindung zum Server aufbauen
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      payload = http.getString(); // Abruf Textdatei
      Serial.println(payload); // Print response
      payload_length = payload.length(); //Nachrichtenlänge bestimmen
      Serial.println("Laenge: ");
      Serial.println(payload_length);
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
  if (payload != old_payload)  //bei neuer Nachricht auf dem Server
     {
     payload = payload.substring(0, (payload_length -1)); //entferne endzeichen
     lcd.clear();  // Display löschen für neue Nachrichte 
      // Nachricht generieren, aufteilen und anzeigen
      if (payload_length > 16) //Wenn zweizeilige Nachricht
        {
        int Trennen = payload.indexOf(";");
        String MESSAGE1 = payload.substring(0, (Trennen)); //Trenne am Semicolon
        int temp_var = payload.indexOf(";");
        String MESSAGE2 = payload.substring(temp_var + 1);
        //Schreibe Nachricht aufs Display wenn 2-zeilig
        lcd.setCursor(0, 0);
        lcd.print(MESSAGE1);
        lcd.setCursor(0, 1);
        lcd.print(MESSAGE2);
        }
      else // wenn Nachricht kürzer als 16 Zeichen
        {
        //schreibe einzeilige Nachricht aufs display
        lcd.setCursor(0, 0);
        lcd.print(payload);
        }
      old_payload = payload ; //Sichere alte Nachricht zum Vergleich
     } 
    if (millis() - startTime >= interval)
      {
        fetchmessage = true; //wenn Interval um, hole neue Nachricht vom Server in der nächsten loop
      }
}