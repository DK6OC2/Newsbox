/*
******************************************************************************************************
* Newsbox-Projekt des OVV R01 und R04                                                                *
******************************************************************************************************
*/
#include "Arduino.h"
#include "FS.h"         // enable Filesystem
#include "SPIFFS.h"     // enable SPI Flash FileSystem
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <config.h>
#include <ArduinoJson.h>

WiFiMulti wifiMulti;

// Debug Switch - auf true um Ausgaben auf die Konsole zu bekommen
boolean __DEBUG = true; 

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 10000;
setup_display();


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
String IP;
int payload_length; // Variable für die Länge des Textes vom Server definieren
unsigned long startTime = 0; //startpunkt für Zeitschleife
unsigned long interval  = 600000 ; //Nachricht aller 10m abfragen



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
  lcd.init(I2C_SDA, I2C_SCL);
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
  #ifdef DISPLAY_OLED096_SSD1306
    oled.begin();
    oled.clearBuffer();
    oled.setFont(u8g2_font_profont12_mf);
  #endif
  #ifdef DISPLAY_EPAPER29
    display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    display.setRotation(3);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
  #endif  

  #ifdef DISPLAY_TFT
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);
    tft.begin();

    #if defined(LCD_MODULE_CMD_1)
        for (uint8_t i = 0; i < (sizeof(lcd_st7789v) / sizeof(lcd_cmd_t)); i++) {
            tft.writecommand(lcd_st7789v[i].cmd);
            for (int j = 0; j < (lcd_st7789v[i].len & 0x7f); j++) {
                tft.writedata(lcd_st7789v[i].data[j]);
            }

            if (lcd_st7789v[i].len & 0x80) {
                delay(120);
            }
        }
    #endif

    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.pushImage(0, 0, 320, 170, (uint16_t *)gImage_NewsBox);
    delay(2000);

    #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
        ledcSetup(0, 2000, 8);
        ledcAttachPin(PIN_LCD_BL, 0);
        ledcWrite(0, 255);
    #else
        ledcAttach(PIN_LCD_BL, 200, 8);
        ledcWrite(PIN_LCD_BL, 255);
    #endif
  #endif


  WiFi.mode(WIFI_STA);
  WiFi.macAddress(mac);
  MacAddr += String(mac[5],HEX);
  MacAddr += String(mac[4],HEX);
  MacAddr += String(mac[3],HEX);
  MacAddr += String(mac[2],HEX);
  MacAddr += String(mac[1],HEX);
  MacAddr += String(mac[0],HEX);
  if (__DEBUG) Serial.println("MAC: "+MacAddr);

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

  #ifdef DISPLAY_OLED096_SSD1306
    oled.drawStr(0,15, "Newsbox-Projekt");
    oled.drawStr(0,30, Rufzeichen);
    oled.drawStr(70,30, Locator);
    oled.drawStr(0,40, "Mac: ");
    oled.drawStr(25,40, MacAddr.c_str());
    oled.sendBuffer();
  #endif

  #ifdef DISPLAY_EPAPER29
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 15);
    display.setFont(&FreeMonoBold9pt7b);
    display.print("Newsbox-Projekt");
    display.setCursor(10, 45);
    display.setFont(&FreeMono9pt7b);
    display.print(Rufzeichen);
    display.setCursor(100, 45);
    display.print(Locator);
    display.setCursor(10, 65);
    display.print("Mac: ");
    display.setCursor(100, 65);
    display.print(MacAddr.c_str());
    display.display(false);
  #endif

  #ifdef DISPLAY_TFT
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("NewsBox Projekt:", DISPLAY_START_X, DISPLAY_START_Y, 2);

    tft.setTextSize(1);
    //tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString("Callsign:", DISPLAY_START_X, DISPLAY_START_Y + 2* DISPLAY_ROW_HEIGHT, 2);
    tft.drawString(Rufzeichen,160, DISPLAY_START_Y + 2* DISPLAY_ROW_HEIGHT, 2);
    tft.drawString("Locator:", DISPLAY_START_X, DISPLAY_START_Y + 3* DISPLAY_ROW_HEIGHT, 2);
    tft.drawString(Locator,160, DISPLAY_START_Y + 3* DISPLAY_ROW_HEIGHT, 2);
    tft.drawString("MAC:", DISPLAY_START_X, DISPLAY_START_Y + 4* DISPLAY_ROW_HEIGHT, 2);
    tft.drawString(MacAddr,160, DISPLAY_START_Y + 4* DISPLAY_ROW_HEIGHT, 2);
//    int xpos = 0;
//    xpos += tft.drawString("xyz{|}~", 0, 64, 2);
//    tft.drawChar(127, xpos, 64, 2);
    delay(WAIT);
  #endif

  delay(3000);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
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
  #ifdef DISPLAY_TFT
    tft.drawString("Connecting WiFi...",DISPLAY_START_X, DISPLAY_START_Y + 5* DISPLAY_ROW_HEIGHT, 2);    
  #endif

  if(wifiMulti.run() == WL_CONNECTED) {
    IP = WiFi.localIP().toString();
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(IP);
    new_wifi = false;
    #ifdef DISPLAY_EPAPER29
      display.setCursor(10, 85);
      display.print("IP: ");
      display.setCursor(100, 85);
      display.print(WiFi.localIP()); 
      display.display(true); 
    #endif

    #ifdef DISPLAY_TFT
      tft.drawString("WiFi connected, IP:",DISPLAY_START_X, DISPLAY_START_Y + 6* DISPLAY_ROW_HEIGHT, 2);    
      tft.drawString(IP,200, DISPLAY_START_Y + 6* DISPLAY_ROW_HEIGHT, 2);  
    #endif
    delay(5000);
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
      //setze Abrufsignal ('*')
      #ifdef DISPLAY_2004
        lcd.setCursor(0, 9);
        lcd.print("*");
      #endif
      #if defined (DISPLAY_OLED096) || defined (OLED096_SSD1306) // für 0,96 OLEDS
        oled.drawStr(55,15, "*");
        oled.sendBuffer();
      #endif
      #ifdef DISPLAY_OLED13
        oled.drawStr(60,15, "*");
        oled.sendBuffer();
      #endif
      #ifdef DISPLAY_EPAPER29
        display.setCursor(110, 10);
        display.print("*");
        display.display(true);
      #endif

      Serial.println("Fetching ... "+URL);
      http.begin(*client, URL+"?mac="+MacAddr+"&call="+Rufzeichen+"&loc="+Locator); //Verbindung zum Server aufbauen
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        JSonMessage = http.getString(); // Abruf JsonObject
        Serial.println("Server Antwort: "+JSonMessage);
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
          /* Neuer Code für Webservervariante
          news_date = doc["date"];
          news_topic = doc["topic"];
          news_line1 = doc["line1"];
          news_line2 = doc["line2"];
          news_line3 = doc["line3"];
          */
          news_date = doc["Datum"];
          news_topic = doc["Message"]["Topic"];
          news_line1 = doc["Message"]["Zeile1"];
          news_line2 = doc["Message"]["Zeile2"];
          news_line3 = doc["Message"]["Zeile3"];
          if(strlen(news_topic) > 9) news_date = "";  // wenn das TOIPC mehr als 9 Zeichen hat, wird das Datum nicht ausgegeben...
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
      delay(1000); //Wartezeit für Sichtbarkeit des Abrufsignals
      //Entferne Abrufsignal ('*')
      #ifdef DISPLAY_2004
        lcd.setCursor(0, 9);
        lcd.print(" ");
      #endif
      #if defined (DISPLAY_OLED096) || defined (OLED096_SSD1306) // für 0,96 OLEDS
        oled.drawStr(55,15, " ");
        oled.sendBuffer();
      #endif
      #ifdef DISPLAY_OLED13
        oled.drawStr(60,15, " ");
        oled.sendBuffer();
      #endif
      #ifdef DISPLAY_EPAPER29
        display.setCursor(110, 10);
        display.setTextColor(GxEPD_WHITE);
        display.print("*");
        display.display(true);
        display.setTextColor(GxEPD_BLACK);
      #endif
     
    if (news_id != old_id)  //bei neuer Nachricht auf dem Server
      {
        
        if (__DEBUG) {
          Serial.println(news_topic);
          Serial.println(news_date);
          Serial.println(news_line1);
          Serial.println(news_line2);
          Serial.println(news_line3);
        }

        #ifdef DISPLAY_2004
          lcd.clear();  // Display löschen für neue Nachrichte 
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
          oled.drawStr(0,15, news_topic);
          oled.drawStr(60,15, news_date);
          oled.drawStr(0,30, news_line1);
          oled.drawStr(0,40, news_line2);
          oled.drawStr(0,50, news_line3);
          oled.sendBuffer();
        #endif
        #ifdef DISPLAY_OLED13
          oled.clear();  // Display löschen für neue Nachrichte 
          oled.drawStr(2,15, news_topic);
          oled.drawStr(65,15, news_date);
          oled.drawStr(2,36, news_line1);
          oled.drawStr(2,50, news_line2);
          oled.drawStr(2,63, news_line3);
          oled.sendBuffer();
        #endif
        #ifdef DISPLAY_OLED096_SSD1306
          oled.clear();  // Display löschen für neue Nachrichte 
          oled.drawStr(0,15, news_topic);
          oled.drawStr(60,15, news_date);
          oled.drawStr(0,30, news_line1);
          oled.drawStr(0,40, news_line2);
          oled.drawStr(0,50, news_line3);
          oled.sendBuffer();
        #endif
        #ifdef DISPLAY_EPAPER29
          display.fillScreen(GxEPD_WHITE);
          display.setCursor(10, 15);
          display.setFont(&FreeMonoBold9pt7b);
          display.print(news_topic);
          display.setCursor(120,15);
          display.print(news_date);
          display.setCursor(10, 45);
          display.setFont(&FreeMono9pt7b);
          display.print(news_line1);
          display.setCursor(10, 65);
          display.print(news_line2);
          display.setCursor(10, 85);
          display.print(news_line3);
          display.display(false);
        #endif 

        #ifdef DISPLAY_TFT
//          tft.setTextSize(1);
          tft.fillScreen(TFT_BLACK);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);

//          tft.drawString("Topic:", 0, 32, 7);
//          tft.drawString(news_topic,160, 32, 7);
//          tft.drawString("Date:", 0, 48, 7);
//          tft.drawString(news_date,160, 48, 7);
//          tft.drawString("Message:", 0, 64, 7);
//          tft.drawString(news_line1,160, 64, 7);
//          tft.drawString(news_line2,160, 80, 7);
//          tft.drawString(news_line3,160, 96, 7);

          tft.setCursor(0, DISPLAY_START_Y + 2* DISPLAY_ROW_HEIGHT);
          tft.setFreeFont(FMBO9);
          tft.print("Topic:");

          tft.setCursor(90,DISPLAY_START_Y + 2* DISPLAY_ROW_HEIGHT);
          tft.setFreeFont(FMO9);
          tft.println(news_topic);

          tft.setCursor(0, DISPLAY_START_Y + 3* DISPLAY_ROW_HEIGHT);
          tft.setFreeFont(FMBO9);
          tft.print("Date::");
          tft.setCursor(90,DISPLAY_START_Y + 3* DISPLAY_ROW_HEIGHT);
          tft.setFreeFont(FMO9);
          tft.println(news_date);

          tft.setCursor(0, DISPLAY_START_Y + 4* DISPLAY_ROW_HEIGHT);
          tft.setFreeFont(FMBO9);
          tft.print("Message:");
          tft.setCursor(90,DISPLAY_START_Y + 4* DISPLAY_ROW_HEIGHT);
          tft.setFreeFont(FMO9);
          tft.println(news_line1);
          tft.setCursor(90,DISPLAY_START_Y + 5* DISPLAY_ROW_HEIGHT);
          tft.println(news_line2);
          tft.setCursor(90,DISPLAY_START_Y + 6* DISPLAY_ROW_HEIGHT);
          tft.println(news_line3);

        #endif

        old_id = news_id; //Sichere alte Nachrichten-id zum Vergleich
        
        #if defined(ENABLE_BUZZER)
          digitalWrite(LED_PIN, HIGH); // Schalte LED ein
          #ifdef BUZZER_PASSIVE
            tone(BUZZER_PIN, 1000, 1000);
          #endif
          #ifdef BUZZER_ACTIVE
            digitalWrite(BUZZER_PIN, HIGH);
            delay(1000);
            digitalWrite(BUZZER_PIN, LOW);
          #endif    
        #endif  
        
      }
    }
    B_currentState = digitalRead(BUTTON_PIN);
    
    if (B_lastState == HIGH && B_currentState == LOW)
      {
      digitalWrite(LED_PIN, LOW); //Schalte LED aus wenn Taste gedrückt
      } 
    B_lastState = B_currentState; // save the the last state of the button 
    
    if (millis() - startTime >= interval)
      {
        fetchmessage = true; //wenn Interval um, hole neue Nachricht vom Server in der nächsten loop
      }
}
