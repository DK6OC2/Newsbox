//general display setup
#include <LiquidCrystal_I2C.h>
//#include <GxEPD2_BW.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);                 
void init_display()
{
  // code für Displayinitialisierung
  lcd.init();
  lcd.backlight();
}
void display_splash()
{
  //code für splash screen, wenn gewüncht
}
void display_boxdata()
{
  //Anzeige der Daten der Box (Rufzeichen, Locator, MacAdresse)
  lcd.setCursor(0, 0);
  lcd.print("Newsbox-Projekt");
  lcd.setCursor(0,1);
  lcd.print(Rufzeichen);
  lcd.setCursor(10, 1);
  lcd.print(Locator);
  lcd.setCursor(0, 2);
  lcd.print("Mac: "+MacAddr);
}
void display_wifi_connecting()
{
  //code für connecting, wenn gewünscht 
  lcd.setCursor(0, 3);
  lcd.print("Connecting to Wifi..");
}
void display_wifi_connected()
{
  //code für die Anzeige der IP-Adresse nach Verbindung zum WLAN
  lcd.setCursor(0, 3);
  lcd.print("IP: "+IP+"  ");
   
}
void display_fetch_flag()
{
   //code für Abrufsignal
   lcd.setCursor(9, 0); 
   lcd.print("*");
}
void remove_fetch_flag()
{
   //code für Entfernung des Abrufsignals
   lcd.setCursor(9, 0);
   lcd.print(" ");
}
void display_message()
{
   //code für das Anzeigen einer Nachricht
   lcd.clear();  
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
}