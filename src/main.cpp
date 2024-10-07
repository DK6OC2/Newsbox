/*
******************************************************************************************************
* Newsbox-Projekt des OVV R01 und R04    *
* Version 2.0                            *                               
******************************************************************************************************
*/
#include <Arduino.h>
#include <config.h>     // we need to load config settings first
#include <globals.h>    // now we load global values
#include <WiFi.h>
#include <tones.h>
#if defined(SMALL_WIFI_MAN)
  #include <eeprom.h>
  #include <console.h>
#endif
#if defined(WIFI_MAN)
  #include <WiFiMulti.h>
#endif
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <EventButton.h>
#include <ESP32Web.h>


#if defined(WIFI_MAN)
WiFiMulti wifiMulti;
#endif
#if defined(SMALL_WIFI_MAN)
  WiFiEvent_t wifi_last_event = ARDUINO_EVENT_MAX;
  WiFiClient wifi_client;
#endif
EventButton button1(BUTTON_PIN);
JsonDocument doc; //JSON Opject
WiFiClientSecure *client = new WiFiClientSecure ;       // initialisieren des WifiClients mit SSL
bool new_wifi = true ;                                  // Flag für neue Wifi-Verbindung nach Verbindungsverlust
void onButton1LongPress(EventButton& eb);
void onbutton1Clicked(EventButton& eb);
#if defined(SMALL_WIFI_MAN)
  //void check_factory_reset();
  void start_wifi();
  void wifi_on_event(WiFiEvent_t event);
#endif

void setup()
{
  // Start Serial
  Serial.begin(115200);
  Serial.setTimeout(1000); // miliseconds to wait (50 mili) for USB Data. Default 1000
  button1.setLongPressHandler(onButton1LongPress, true);
  button1.setClickHandler(onbutton1Clicked);
  init_display();
  display_splash();
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  #if defined(WIFI_MAN)
   // Looping Functions: 
   t0_AP_Mode.setInterval(1000, ledBlink); // timer for LED blink
   t0_AP_Mode.setInterval(1000, stopAP); // timer for AP mode (WiFi Hostpot)
   checkWiFiConfig(); // check config file -> run AP or STA mode -> start web server
  #endif

  #if defined(SMALL_WIFI_MAN)
   //check_factory_reset(); // reset to factory settings if requested
   eeprom_init();         // initialize EEPROM
   start_wifi(); // start WiFi client
  #endif 
  
  Serial.println("Setup done");
  
  //WiFi.mode(WIFI_STA);
  WiFi.macAddress(mac);
  MacAddr += String(mac[5],HEX);
  MacAddr += String(mac[4],HEX);
  MacAddr += String(mac[3],HEX);
  MacAddr += String(mac[2],HEX);
  MacAddr += String(mac[1],HEX);
  MacAddr += String(mac[0],HEX);
  if (debug) Serial.println("MAC: "+MacAddr);

  display_boxdata();
  IP = WiFi.localIP().toString();
  display_wifi_connected();
  //pinMode(LED_PIN, OUTPUT);
  //pinMode(BUZZER_PIN, OUTPUT);
  //digitalWrite(LED_PIN, LOW);
  delay(3000);
 
}

void loop()
{
  #if defined(WIFI_MAN)
    handleState(); // handle the state of the web server (AP or STA)
  #endif
  #if defined(SMALL_WIFI_MAN)
    handle_serial();
  #endif  

  client->setInsecure(); // ignoriere SSL-Cert


  
  HTTPClient http ; //Webclient starten
  //Nachricht holen, wenn erster Durchlauf oder Intervall abgelaufen ist
  if ((fetchmessage) && (WiFi.status() == WL_CONNECTED))
    {
      //setze Abrufsignal ('*')
      display_fetch_flag();
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
      remove_fetch_flag();
    }

  if (WiFi.status() == WL_CONNECTED) 

  {    
  max_news = doc["messages"].size();
  news_id = doc["messages"][0]["id"];
  news_date = doc["messages"][akt_news]["date"];
  news_topic = doc["messages"][akt_news]["topic"];
  news_subject = doc["messages"][akt_news]["subject"];
  news_line1 = doc["messages"][akt_news]["line1"];
  news_line2 = doc["messages"][akt_news]["line2"];
  news_line3 = doc["messages"][akt_news]["line3"];
  if(strlen(news_topic) > 9) news_date = "";  // wenn das TOPIC mehr als 9 Zeichen hat, wird das Datum nicht ausgegeben...    
  }
  if ((news_id != old_id) && (WiFi.status() == WL_CONNECTED))   //bei bestehender WLAN-Verbindung und neuer Nachricht auf dem Server
      {
        
        if (debug) {
          Serial.println(news_topic);
          Serial.println(news_date);
          Serial.println(news_line1);
          Serial.println(news_line2);
          Serial.println(news_line3);
        }

        akt_news = 1;
        old_id = news_id; //Sichere alte Nachrichten-id zum Vergleich
        #if defined(ENABLE_LED)
        //digitalWrite(LED_PIN, HIGH); // Schalte LED ein
        #endif
        #if defined(ENABLE_BUZZER)
          #ifdef BUZZER_PASSIVE
            tone(BUZZER_PIN, 1000, 1000);
          #endif
          #ifdef BUZZER_ACTIVE
            digitalWrite(BUZZER_PIN, HIGH);
            delay(1000);
            digitalWrite(BUZZER_PIN, LOW);
          #endif    
        #endif
        delay(500);
        first_run = true;
        display_message();  
        first_run = false;      
      }
    
    button1.update();
    if (millis() - startTime >= readNewsInterval)
      {
        fetchmessage = true; //wenn Interval um, hole neue Nachricht vom Server in der nächsten loop
      }
}
void onButton1LongPress(EventButton& eb) {
   #if defined(ENABLE_LED)
         digitalWrite(LED_PIN, LOW); //Schalte LED aus wenn Taste lange gedrückt
   #endif
} 
void onbutton1Clicked(EventButton& eb) {
  if ((akt_news + 1) < doc["messages"].size())
    {
      akt_news = akt_news + 1;
    }
  else
    {
      akt_news = 0;
    }
  display_message(); 
}

void check_factory_reset()
{
  if (digitalRead(BUTTON_PIN) == LOW) // button was pressed during startup
  {
    factory_reset();                 // factory reset on pressed button during power on
    for (uint8_t i = 0; i < 20; i++) // five seconds to wait
    {
      digitalWrite(LED_PIN, digitalRead(LED_PIN) % 2 == 0); // fast blinking
      delay(250);
    }
    ESP.restart();
  }
}
// start Wifi client
void start_wifi()
{
  String wifi_ssid = eeprom_get_string("wifi_ssid");
  String wifi_psk = eeprom_get_string("wifi_psk");
  String hostname = eeprom_get_string("hostname");
  if (hostname != "")
    WiFi.hostname(hostname);
  Serial.print(F("- MAC address: "));
  Serial.println(WiFi.macAddress());
  Serial.print(F("- Hostname:    "));
  Serial.println(WiFi.getHostname());
  if (wifi_ssid != "")
  {
    Serial.print(F("- Network:     "));
    Serial.println(wifi_ssid);
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    WiFi.onEvent(wifi_on_event);
    WiFi.begin(wifi_ssid, wifi_psk); // WPA2 PSK
    WiFi.setAutoReconnect(true);
  }
  else
  {
    Serial.println(F("No WiFi settings. Please configure via command line interface."));
    fetchmessage = false;
  }
  // tones only play when loop() is running - Mario Land 1st Level
  if (false)
  {
    add_tone(659.255, 140); // E5 1/8
    add_tone(0, 10);        //
    add_tone(659.255, 150); // E5 1/8
    add_tone(0, 150);       // -- 1/8
    add_tone(659.255, 150); // E5 1/8
    add_tone(0, 150);       // -- 1/8
    add_tone(523.251, 140); // C5 1/8
    add_tone(0, 10);        //
    add_tone(659.255, 290); // E5 1/4
    add_tone(0, 10);        //
    add_tone(783.991, 300); // G5 1/4
    add_tone(0, 300);       // -- 1/4
    add_tone(391.995, 300); // G4 1/4
    add_tone(0, 300);       // -- 1/4
  }
}
void wifi_on_event(WiFiEvent_t event)
{
  // Serial.print ("[WiFi-Event] event / last_event: ");  Serial.print (event);
  // Serial.print (" / ");  Serial.println (wifi_last_event);
  if (wifi_last_event == event)
    return; // prevent duplicates
  wifi_last_event = event;
  if (event == ARDUINO_EVENT_WIFI_STA_CONNECTED)
  {
    Serial.println(F("[WiFi-Event] Connected to access point"));
    if (true)
    {
      add_tone(392, 25);
      add_tone(784, 100);
      add_tone(0, 50);
    }
  }
  if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED)
  {
    Serial.println(F("[WiFi-Event] Disconnected from access point"));
    if (true)
    {
      add_tone(784, 25);
      add_tone(392, 100);
      add_tone(0, 50);
    }
    WiFi.reconnect();
  }
  if (event == ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE)
  {
    Serial.println(F("[WiFi-Event] Authentication mode of access point has changed"));
  }
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP)
  {
    Serial.println(F("[WiFi-Event] Got IP address:"));
    Serial.println("- IP address:  " + WiFi.localIP().toString());
    Serial.println("- Subnet mask: " + WiFi.subnetMask().toString());
    Serial.println("- Gateway:     " + WiFi.gatewayIP().toString());
    Serial.println("- DNS server:  " + WiFi.dnsIP().toString());
    Serial.println();
    if (true)
    {
      add_tone(392, 25);
      add_tone(784, 100);
      add_tone(0, 50);
    }
  }
  if (event == ARDUINO_EVENT_WIFI_STA_LOST_IP)
  {
    Serial.println(F("[WiFi-Event] Lost IP address"));
    if (true)
    {
      add_tone(784, 25);
      add_tone(392, 100);
      add_tone(0, 50);
    }
    WiFi.reconnect();
  }
}