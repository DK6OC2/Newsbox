void setup_display()
{
  #include <GxEPD2_BW.h>
  #include <Fonts/FreeMonoBold9pt7b.h>
  #include <Fonts/FreeMono9pt7b.h>
  GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=D8*/ 26, /*DC=D3*/ 25, /*RST=D4*/ 33, /*BUSY=D2*/ 27)); // GDEM029T94 128x296, SSD1680
}
void init_display()
{
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  display.setRotation(3);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
}