//Enter personal, Wifi and Message source configuration below
const char* WIFI_SSID = "your-ssid";                  // Name des eigenen WLAN
const char* WIFI_PASSWORD = "your-password"; // Passwort eigenes WLAN
const char* host = "www.r04-afu.de";                // Link zum OV-Server
const String URL = "http:/www.r04-afu.de/info.txt";
const String OV = "News";          // Individueller Text
const String Rufzeichen = "DK6OC"; // eigenes Call
const String Locator = "JO31ke"; // eigener Locator

IPAddress ip(192, 168, 2, 8);       // feste IP-Adresse im eigenen Netz
IPAddress gateway(192, 168, 2, 1);  // Gateway-Adresse im eigenen Netz
IPAddress subnet(255, 255, 255, 0); // Subnet IP-Adresse im eigenen Netz
