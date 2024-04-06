//general display setup
#include <GxEPD2_4C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
GxEPD2_4C<GxEPD2_290c_GDEY029F51H, GxEPD2_290c_GDEY029F51H::HEIGHT> display(GxEPD2_290c_GDEY029F51H(/*CS=*/ 26, /*DC=*/ 25, /*RST=*/ 33, /*BUSY=*/ 27)); // GDEY029F51H 168x384, JD79667 (FPC-H004 22.03.24)
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
   //code für connecting, wenn gewünscht
}
void display_wifi_connected()
{
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
   display.print(news_line1);
   display.setCursor(10, 65);
   display.print(news_line2);
   display.setCursor(10, 85);
   display.print(news_line3);
   display.display(false);
}