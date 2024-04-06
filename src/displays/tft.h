
/*
#if defined (DISPLAY_OLED096_SSD1306)
  #include "U8g2lib.h"
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE, I2C_SDA, I2C_SCL);
#endif



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
 #ifdef DISPLAY_OLED096_SSD1306
    oled.begin();
    oled.clearBuffer();
    oled.setFont(u8g2_font_profont12_mf);
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

  #ifdef DISPLAY_OLED096_SSD1306
    oled.drawStr(0,15, "Newsbox-Projekt");
    oled.drawStr(0,30, Rufzeichen);
    oled.drawStr(70,30, Locator);
    oled.drawStr(0,40, "Mac: ");
    oled.drawStr(25,40, MacAddr.c_str());
    oled.sendBuffer();
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
     
      #if defined (DISPLAY_OLED096) || defined (OLED096_SSD1306) // für 0,96 OLEDS
        oled.drawStr(55,15, "*");
        oled.sendBuffer();
      #endif
      #if defined (DISPLAY_OLED096) || defined (OLED096_SSD1306) // für 0,96 OLEDS
        oled.drawStr(55,15, " ");
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