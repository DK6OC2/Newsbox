//general display setup
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=D8*/ 26, /*DC=D3*/ 25, /*RST=D4*/ 33, /*BUSY=D2*/ 27)); // GDEM029T94 128x296, SSD1680
String list;
void init_display()
{
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  display.setRotation(3);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
}
void display_splash()
{
  //code für splash screen, wenn gewüncht
}
void display_boxdata()
{
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
}
void display_wifi_connecting()
{
   display.setCursor(10, 85);
   display.setTextColor(GxEPD_BLACK);
   display.print("Connecting to Wifi ...");
   display.display(true); 
}
void display_wifi_connected()
{
   display.setCursor(10, 85);
   display.setTextColor(GxEPD_WHITE);
   display.print("Connecting to Wifi ...");
   display.display(true);
   display.setTextColor(GxEPD_BLACK);
   display.setCursor(10, 85);
   display.print("IP: ");
   display.setCursor(100, 85);
   display.print(WiFi.localIP()); 
   display.display(true); 
}
void display_fetch_flag()
{
   display.setCursor(110, 10);
   display.print("*");
   display.display(true);
}
void remove_fetch_flag()
{
   display.setCursor(110, 10);
   display.setTextColor(GxEPD_WHITE);
   display.print("*");
   display.display(true);
   display.setTextColor(GxEPD_BLACK);
}
void display_message()
{
   display.fillScreen(GxEPD_WHITE);
   display.setCursor(10, 15);
   display.setFont(&FreeMonoBold9pt7b);
   display.print(news_topic);
   display.setCursor(120,15);
   display.print(news_date);
   display.setCursor(10, 45);
   display.setFont(&FreeMono9pt7b);
   display.print(news_subject);
   display.drawLine(10, 50, 250, 50, 0);
   display.setCursor(10, 70);
   display.print(news_line1);
   display.setCursor(10, 90);
   display.print(news_line2);
   display.setCursor(10, 110);
   display.print(news_line3);
   display.setCursor(240, 115);
   if (akt_news == 0)
     {
     list = String(10);
     }
   else
     {
     list = String((akt_news));
     }
   list = list+"/";
   list = list+ String(max_news);
   display.print(list);
   display.display(false);
}