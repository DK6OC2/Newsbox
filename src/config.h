//Enter personal, Wifi and Message source configuration below
const char* WIFI_SSID = "YourSSID";                  // Name des eigenen WLAN
const char* WIFI_PASSWORD = "Yourpass"; // Passwort eigenes WLAN
const char* host = "www.r04-afu.de";                // Link zum OV-Server
const String URL = "http:/www.r04-afu.de/info.txt";
const String OV = "News";          // Individueller Text
const String Rufzeichen = "YourCall"; // eigenes Call
const String Locator = "JO31kd"; // eigener Locator

IPAddress ip(192, 168, 178, 72);       // feste IP-Adresse im eigenen Netz
IPAddress gateway(192, 168, 178, 1);  // Gateway-Adresse im eigenen Netz
IPAddress subnet(255, 255, 255, 0); // Subnet IP-Adresse im eigenen Netz
IPAddress primaryDNS(8, 8, 8, 8); // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional