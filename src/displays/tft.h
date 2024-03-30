/*
#ifdef DISPLAY_2004   // für 4 Zeilen/20 Zeichen Displays
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27, 20, 4);                 
#endif
*/
/*
#if defined (DISPLAY_OLED096) || defined (DISPLAY_OLED13) // für 0,96 oder 1,3" OLEDS
 #include "U8g2lib.h"
 U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);
#endif
*/
/*
#if defined (DISPLAY_OLED096_SSD1306)
  #include "U8g2lib.h"
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE, I2C_SDA, I2C_SCL);
#endif
*/
/*
#ifdef DISPLAY_EPAPER29
  #include <GxEPD2_BW.h>
  #include <Fonts/FreeMonoBold9pt7b.h>
  #include <Fonts/FreeMono9pt7b.h>
  GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(GxEPD2_290_T94(/*CS=D8*/ 26, /*DC=D3*/ 25, /*RST=D4*/ 33, /*BUSY=D2*/ 27)); // GDEM029T94 128x296, SSD1680
#endif
*/
/*
#ifdef DISPLAY_TFT
  #include "TFT_eSPI.h" 
  #include <logo/NewsBox.h>
  #include <tft.h>

  /* The product now has two screens, and the initialization code needs a small change in the new version. The LCD_MODULE_CMD_1 is used to define the
  * switch macro. 
  #define LCD_MODULE_CMD_1

  TFT_eSPI tft = TFT_eSPI();
  #define WAIT 1000
  unsigned long targetTime = 0; // Used for testing draw times

  #if defined(LCD_MODULE_CMD_1)
  typedef struct {
      uint8_t cmd;
      uint8_t data[14];
      uint8_t len;
  } lcd_cmd_t;

  lcd_cmd_t lcd_st7789v[] = {
      {0x11, {0}, 0 | 0x80},
      {0x3A, {0X05}, 1},
      {0xB2, {0X0B, 0X0B, 0X00, 0X33, 0X33}, 5},
      {0xB7, {0X75}, 1},
      {0xBB, {0X28}, 1},
      {0xC0, {0X2C}, 1},
      {0xC2, {0X01}, 1},
      {0xC3, {0X1F}, 1},
      {0xC6, {0X13}, 1},
      {0xD0, {0XA7}, 1},
      {0xD0, {0XA4, 0XA1}, 2},
      {0xD6, {0XA1}, 1},
      {0xE0, {0XF0, 0X05, 0X0A, 0X06, 0X06, 0X03, 0X2B, 0X32, 0X43, 0X36, 0X11, 0X10, 0X2B, 0X32}, 14},
      {0xE1, {0XF0, 0X08, 0X0C, 0X0B, 0X09, 0X24, 0X2B, 0X22, 0X43, 0X38, 0X15, 0X16, 0X2F, 0X37}, 14},
  };
  #endif
#endif
*/
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
  
  #ifdef DISPLAY_TFT
    tft.drawString("Connecting WiFi...",DISPLAY_START_X, DISPLAY_START_Y + 5* DISPLAY_ROW_HEIGHT, 2);    
  #endif
#ifdef DISPLAY_TFT
      tft.drawString("WiFi connected, IP:",DISPLAY_START_X, DISPLAY_START_Y + 6* DISPLAY_ROW_HEIGHT, 2);    
      tft.drawString(IP,200, DISPLAY_START_Y + 6* DISPLAY_ROW_HEIGHT, 2);  
    #endif
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