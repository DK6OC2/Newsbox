#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <config.h>
WiFiMulti wifiMulti;

//LiquidCrystal_I2C lcd(0x27, 16, 2);                 // für 2 Zeilen Displays
LiquidCrystal_I2C lcd(0x27, 20, 4);                 // für 4 Zeilen Displays

const uint32_t connectTimeoutMs = 10000;
byte mac[6];   // byte-array für Mac-Adresse
String payload; // Variable für Nachricht
String old_payload = "empty"; // Variable für Änderungsprüfung, muss beim ersten Durchlauf abweichen
String MacAddr;
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long interval  = 600000 ; //Nachricht aller 10m abfragen

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
}

void loop()
{
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
      if (payload_length > 16) //Wenn zweizeilige Nachricht, die Anfrage ist unnötig. Wenn die Nachricht kürzer als 16 bzw. 20 Zeichen ist, bleiben die weiteren Zeilen automatisch leer.
        {
        int Trennen = payload.indexOf(";");
        String MESSAGE1 = payload.substring(0, (Trennen)); //Trenne am Semicolon
        int temp_var = payload.indexOf(";");
        String MESSAGE_TEMP = payload.substring(temp_var + 1);
        Trennen = MESSAGE_TEMP.indexOf(";");
        String MESSAGE2 = MESSAGE_TEMP.substring(0, (Trennen)); //Trenne wieder Semicolon
        temp_var = MESSAGE_TEMP.indexOf(";");
        String MESSAGE_TEMP2 = MESSAGE_TEMP.substring(temp_var + 1);
        Trennen = MESSAGE_TEMP2.indexOf(";");
        String MESSAGE3 = MESSAGE_TEMP2.substring(0, (Trennen)); //Trenne wieder Semicolon
        temp_var = MESSAGE_TEMP2.indexOf(";");
        String MESSAGE4 = MESSAGE_TEMP2.substring(temp_var + 1);


        //Schreibe Nachricht aufs Display wenn 2-zeilig
        lcd.setCursor(0, 0);
        lcd.print(MESSAGE1);
        lcd.setCursor(0, 1);
        lcd.print(MESSAGE2);
        lcd.setCursor(0, 2);
        lcd.print(MESSAGE3);
        lcd.setCursor(0, 3);
        lcd.print(MESSAGE4);
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
