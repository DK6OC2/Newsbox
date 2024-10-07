// EEPROM routines

#define EEPROM_NAMESPACE "app"
#include <Preferences.h> // (EEPROM) https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html
Preferences eeprom;

// overwrites the EEPROM area with zeros
void factory_reset()
{
  eeprom.begin(EEPROM_NAMESPACE, false);
  eeprom.clear();
  eeprom.end();
  // The namespace is retained. To delete this,
  // the non-volatile storage (nvs) must be overwritten:
  // #include <nvs_flash.h>
  // void setup() {
  //   nvs_flash_erase (); // erase the NVS partition and
  //   nvs_flash_init (); // initialize the NVS partition
  // }
}

// initialize the EEPROM area
void eeprom_init()
{
  eeprom.begin(EEPROM_NAMESPACE, false); // must be readonly=false for the first time at startup so that the namespace is created
  // initialize missing values if necessary
  if (!eeprom.isKey("wifi_ssid"))
    eeprom.putString("wifi_ssid", "");
  if (!eeprom.isKey("wifi_psk"))
    eeprom.putString("wifi_psk", "");
  if (!eeprom.isKey("hostname"))
    eeprom.putString("hostname", "");
  if (!eeprom.isKey("mqtt_broker"))
    eeprom.putString("mqtt_broker", "");
  if (!eeprom.isKey("mqtt_user"))
    eeprom.putString("mqtt_user", "");
  if (!eeprom.isKey("mqtt_pass"))
    eeprom.putString("mqtt_pass", "");
  if (!eeprom.isKey("mqtt_topic"))
    eeprom.putString("mqtt_topic", "rotorbox");
  eeprom.end();
}

String eeprom_get_string(String key)
{
  String value;
  eeprom.begin(EEPROM_NAMESPACE, true);
  value = eeprom.getString (key.c_str());
  eeprom.end();
  return (value);
}

void eeprom_save_string(String key, String value)
{
  eeprom.begin(EEPROM_NAMESPACE, false);
  eeprom.putString(key.c_str(), value.c_str());
  eeprom.end();
}

// EOF