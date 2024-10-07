// serial console routines

String cmd_line;
String cmd_line_module;
char cmd_line_return = 0;
const PROGMEM String msg_current_value = "Current value: ";
const PROGMEM String msg_change_or_escape = "Enter new value or press [Escape].";

// processes a finished input line
void parse_cmd_input(String input)
{
  if (cmd_line_module == "") // Shell
  {
    input.trim();
    if (input == "" || input == "[ESCAPE]")
    {
      Serial.print(F(">"));
      return;
    }
    if (input == "help")
    {
      Serial.println();
      Serial.println(F("help            - shows this help"));
      Serial.println(F("scan            - scans and shows availabe WiFi networks"));
      Serial.println(F("debug on        - switch on the debug mode (more serial output)"));
      Serial.println(F("debug off       - switch off debug mode"));
      Serial.println(F("set wifi-ssid   - sets the WiFi SSID"));
      Serial.println(F("set wifi-psk    - sets the WiFi pre-shared key"));
      Serial.println(F("set hostname    - sets the hostname"));
      Serial.println(F("factory-reset   - drops all saved configs and reboots"));
      Serial.println(F("reboot          - restarts the controller"));
      Serial.println();
      Serial.print(F(">"));
      return;
    }
    if (input == "debug on")
    {
      debug = true;
      Serial.println(F("enabled debug mode"));
      Serial.print(F(">"));
      return;
    }
    if (input == "debug off")
    {
      debug = false;
      Serial.println(F("disabled debug mode"));
      Serial.print(F(">"));
      return;
    }
    if (input == "scan")
    {
      // WiFi.enableSTA (false);
      WiFi.disconnect();
      Serial.println(F("Scanning for networks."));
      WiFi.scanNetworks(true);
      char const PROGMEM animation[] = {'-', '\\', '|', '/'};
      int i = 0;
      while (WiFi.scanComplete() < 0)
      {
        Serial.print(animation[i % sizeof(animation)]);
        Serial.print(F("\r"));
        i++;
        delay(250);
      }
      Serial.println(F("Found networks:"));
      Serial.println("  #  SSID                              Ch    dBm  Auth");
      Serial.println("---  --------------------------------  ---  ----  --------------");
      for (i = 0; i < WiFi.scanComplete(); i++)
      {
        Serial.printf("% 3d  %-32s  % 3d  % 4d  ", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i));
        if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
          Serial.println(F("OPEN"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WEP)
          Serial.println(F("WEP"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WPA_PSK)
          Serial.println(F("WPA_PSK"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WPA2_PSK)
          Serial.println(F("WPA2_PSK"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WPA_WPA2_PSK)
          Serial.println(F("WPA_WPA2_PSK"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WPA2_ENTERPRISE)
          Serial.println(F("WPA2_ENTERPRISE"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WPA3_PSK)
          Serial.println(F("WPA3_PSK"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WPA2_WPA3_PSK)
          Serial.println(F("WPA2_WPA3_PSK"));
        if (WiFi.encryptionType(i) == WIFI_AUTH_WAPI_PSK)
          Serial.println(F("WAPI_PSK"));
      }
      Serial.println("---  --------------------------------  ---  ----  --------------");
      Serial.print(F(">"));
      // WiFi.enableSTA (true);
      return;
    }
    if (input == "set wifi-ssid")
    {
      Serial.print(msg_current_value);
      Serial.println(eeprom_get_string("wifi_ssid"));
      Serial.println(msg_change_or_escape);
      Serial.print(F("SSID: "));
      cmd_line_module = "set_wifi_ssid";
      return;
    }
    if (input == "set wifi-psk")
    {
      Serial.print(msg_current_value);
      Serial.println(eeprom_get_string("wifi_psk"));
      Serial.println(msg_change_or_escape);
      Serial.print(F("PSK: "));
      cmd_line_module = "set_wifi_psk";
      return;
    }
    if (input == "set hostname")
    {
      Serial.print(msg_current_value);
      Serial.println(WiFi.getHostname());
      Serial.println(msg_change_or_escape);
      Serial.print(F("Hostname: "));
      cmd_line_module = "set_hostname";
      return;
    }
    if (input == "factory-reset")
    {
      factory_reset();
      return;
    }
    if (input == "reboot")
    {
      ESP.restart();
      return;
    }
    Serial.println(F("Unknown command. Type 'help' for help."));
    Serial.print(F(">"));
    return;
  }
  if (cmd_line_module != "")
  {
    if (input != "[ESCAPE]")
    {
      String key = "";
      if (cmd_line_module == F("set_wifi_ssid"))
        key = "wifi_ssid";
      if (cmd_line_module == F("set_wifi_psk"))
        key = "wifi_psk";
      if (cmd_line_module == F("set_hostname"))
        key = "hostname";
      if (cmd_line_module == F("set_mqtt_broker"))
        key = "mqtt_broker";
      if (cmd_line_module == F("set_mqtt_user"))
        key = "mqtt_user";
      if (cmd_line_module == F("set_mqtt_pass"))
        key = "mqtt_pass";
      if (cmd_line_module == F("set_mqtt_topic"))
        key = "mqtt_topic";
      if (key != "")
      {
        add_tone(800, 100);
        add_tone(0, 100);
        eeprom_save_string(key, input);
        Serial.println("Saved " + key + ": " + input);
        Serial.println(F("Please reboot to apply."));
      }
    }
    cmd_line_module = "";
    Serial.print(F(">"));
    return;
  }
}

// process user input via the serial interface (via USB)
void handle_serial()
{
  if (Serial.available() <= 0) // no data in the input buffer
    return;
  char key = Serial.read();
  if (key == 8) // Backspace
  {
    if (cmd_line == "")
      return;                                                // nothing to delete
    cmd_line = cmd_line.substring(0, cmd_line.length() - 1); // cut off a character at the back
    Serial.write(0x08);                                      // BS
    Serial.write(0x20);                                      // SP
    Serial.write(0x08);                                      // BS
    // Serial.print ("\r>" + cmd_line + " ");
    // Serial.print ("\r>" + cmd_line);
    return;
  }
  if (key == 10 || key == 13) // Enter / Return
  {
    if (cmd_line_return == 0)   // first input character since program start
      cmd_line_return = key;    // remember the input characters of the terminal program (\n or \r)
    if (cmd_line_return != key) // not the input character of the terminal program (e.g. \r instead of \n)
      return;                   // ignore character
    Serial.println();
    parse_cmd_input(cmd_line); // process input
    cmd_line = "";
    return;
  }
  if (key == 27) // Escape
  {
    Serial.println();
    parse_cmd_input("[ESCAPE]"); // process input
    cmd_line = "";
    return;
  }
  if (key == 32) // Space
    if (cmd_line == "")
      return; // ignore spaces at the beginning of the command
  Serial.print(key);
  cmd_line += key;
}

// EOF