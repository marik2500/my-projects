//*****TFT_1.8_NTP_CLOCK_ESP8266_WEMOS_D1_MINI*****//

#include <SPI.h>
#include "Ucglib.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <Adafruit_GFX.h>
#include <time.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
    
char year_value[20];
char mon_value[20];
char date_value[20];
char dow_value[20];
char time_value[20];
char hour_value[20];
char min_value[20];
char sec_value[20];




//Ucglib8BitPortD ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, /* wr= */ 18 , /* cd= */ 19 , /* cs= */ 17, /* reset= */ 16 );
//Ucglib8Bit ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, 0, 1, 2, 3, 4, 5, 6, 7, /* wr= */ 18 , /* cd= */ 19 , /* cs= */ 17, /* reset= */ 16 );

//Ucglib4WireSWSPI ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, /*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);	// not working
//Ucglib4WireSWSPI ucg(ucg_dev_ili9325_spi_18x240x320, ucg_ext_ili9325_spi_18, /*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);	// not working
//Ucglib3WireILI9325SWSPI ucg(ucg_dev_ili9325_spi_18x240x320, ucg_ext_ili9325_spi_18, /*sclk=*/ 13, /*data=*/ 11, /*cs=*/ 10, /*reset=*/ 8);	// not working
//Ucglib3WireILI9325SWSPI ucg(ucg_dev_ili9325_18x240x320_itdb02, ucg_ext_ili9325_18, /*sclk=*/ 13, /*data=*/ 11, /*cs=*/ 10, /*reset=*/ 8);	// not working

//Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ D5, /*data=*/ D7, /*cd=*/ D3, /*cs=*/ D2, /*reset=*/ D4);
Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ D3, /*cs=*/ D2, /*reset=*/ D4);  // (A0, CS, RESET)

//Ucglib_ILI9163_18x128x128_SWSPI ucg(/*sclk=*/ 7, /*data=*/ 6, /*cd=*/ 5, /*cs=*/ 3, /*reset=*/ 4);
//Ucglib_ILI9163_18x128x128_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);	/* HW SPI Adapter */

//Ucglib_ILI9341_18x240x320_SWSPI ucg(/*sclk=*/ 7, /*data=*/ 6, /*cd=*/ 5, /*cs=*/ 3, /*reset=*/ 4);
//Ucglib_ILI9341_18x240x320_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_ILI9341_18x240x320_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_ILI9341_18x240x320_SWSPI ucg(/*sclk=*/ 4, /*data=*/ 3, /*cd=*/ 6, /*cs=*/ 7, /*reset=*/ 5);	/* Elec Freaks Shield */

//Ucglib_HX8352C_18x240x400_SWSPI ucg(/*sclk=*/ 7, /*data=*/ 6, /*cd=*/ 5, /*cs=*/ 3, /*reset=*/ 4);
//Ucglib_HX8352C_18x240x400_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

//Ucglib_ILI9486_18x320x480_SWSPI ucg(/*sclk=*/ 7, /*data=*/ 6, /*cd=*/ 5, /*cs=*/ 3, /*reset=*/ 4);
//Ucglib_ILI9486_18x320x480_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

//Ucglib_SSD1351_18x128x128_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1351_18x128x128_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1351_18x128x128_FT_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1351_18x128x128_FT_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1351_18x128x128_FT_SWSPI ucg(/*sclk=*/ 4, /*data=*/ 17, /*cd=*/ 16, /*cs=*/ 0, /*reset=*/ 2);   /* FT SSD1351 direct connect to ESP32 */

//Ucglib_PCF8833_16x132x132_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cs=*/ 9, /*reset=*/ 8);	/* linksprite board */
//Ucglib_PCF8833_16x132x132_HWSPI ucg(/*cs=*/ 9, /*reset=*/ 8);	/* linksprite board */

//Ucglib_LD50T6160_18x160x128_6Bit ucg( /*d0 =*/ d0, /*d1 =*/ d1, /*d2 =*/ d2, /*d3 =*/ d3, /*d4 =*/ d4, /*d5 =*/ d5, /*wr=*/ wr, /*cd=*/ cd, /*cs=*/ cs, /*reset=*/ reset);
//Ucglib_LD50T6160_18x160x128_6Bit ucg( /*d0 =*/ 16, /*d1 =*/ 17, /*d2 =*/ 18, /*d3 =*/ 19, /*d4 =*/ 20, /*d5 =*/ 21, /*wr=*/ 14, /*cd=*/ 15); /* Samsung 160x128 OLED with 6Bit minimal interface with Due */
//Ucglib_LD50T6160_18x160x128_6Bit ucg( /*d0 =*/ 5, /*d1 =*/ 4, /*d2 =*/ 3, /*d3 =*/ 2, /*d4 =*/ 1, /*d5 =*/ 0, /*wr=*/ 7, /*cd=*/ 6); /* Samsung 160x128 OLED with 6Bit minimal interface with Uno */

//Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SSD1331_18x96x64_UNIVISION_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

//Ucglib_SEPS225_16x128x128_UNIVISION_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
//Ucglib_SEPS225_16x128x128_UNIVISION_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);

//************* My favorite fonts:
  // Font name: fontgroupx11:
  //ucg.setFont(ucg_font_4x6_mr); //39 char per row
  //ucg.setFont(ucg_font_5x7_mr);
  //ucg.setFont(ucg_font_5x8_mr);
  //ucg.setFont(ucg_font_6x10_mr);
  //ucg.setFont(ucg_font_6x12_mr);
  //ucg.setFont(ucg_font_6x13_mr);
  //ucg.setFont(ucg_font_7x13_mr);
  //ucg.setFont(ucg_font_7x14_mr);
  //ucg.setFont(ucg_font_7x14B_mr);
  //ucg.setFont(ucg_font_8x13_mr);
  //ucg.setFont(ucg_font_8x13B_mr);
  //ucg.setFont(ucg_font_8x13O_mr);
  //ucg.setFont(ucg_font_9x15_mr);
  //ucg.setFont(ucg_font_9x15B_mr);
  //ucg.setFont(ucg_font_9x18_mr);
  //ucg.setFont(ucg_font_9x18B_mr);
  //ucg.setFont(ucg_font_10x20_mr);  //15 char per row

  // Font name: fontgroupadobex11:
  //ucg.setFont(ucg_font_courR08_mr); //26 char per row
  //ucg.setFont(ucg_font_courB08_mr);
  //ucg.setFont(ucg_font_courR10_mr);
  //ucg.setFont(ucg_font_courR12_mr);
  //ucg.setFont(ucg_font_courB12_mr);//THIS
  //ucg.setFont(ucg_font_courR14_mr);
  //ucg.setFont(ucg_font_courB14_mr);
  //ucg.setFont(ucg_font_courR18_mr);
  //ucg.setFont(ucg_font_courB18_mr);
  //ucg.setFont(ucg_font_courR24_mr);
  //ucg.setFont(ucg_font_courB24_mr);
  //ucg.setFont(ucg_font_courR24_mr);
  //ucg.setFont(ucg_font_courB24_mr);  //7 char per row

  // Font name: fontgroupprofont
  //ucg.setFont(ucg_font_profont10_mr); //31 char per row
  //ucg.setFont(ucg_font_profont11_mr);
  //ucg.setFont(ucg_font_profont12_mr);
  //ucg.setFont(ucg_font_profont15_mr);
  //ucg.setFont(ucg_font_profont17_mr);
  //ucg.setFont(ucg_font_profont22_mr);
  //ucg.setFont(ucg_font_profont29_mr); //10 char per row

  // Font name: fontgroupinconsolata:
  //ucg.setFont(ucg_font_inr16_mr); //12 char per row
  //ucg.setFont(ucg_font_inb16_mr);
  //ucg.setFont(ucg_font_inr19_mr);
  //ucg.setFont(ucg_font_inb19_mr);
  //ucg.setFont(ucg_font_inr21_mr); //this
  //ucg.setFont(ucg_font_inb21_mr);
  //ucg.setFont(ucg_font_inr24_mr);
  //ucg.setFont(ucg_font_inb24_mr);
  //ucg.setFont(ucg_font_inr27_mr);
  //ucg.setFont(ucg_font_inb27_mr);
  //ucg.setFont(ucg_font_inr30_mr);
  //ucg.setFont(ucg_font_inb30_mr);//6,5 char per row
  //ucg.setFont(ucg_font_inr33_mr);
  //ucg.setFont(ucg_font_inb33_mr);
  //ucg.setFont(ucg_font_inr38_mr);
  //ucg.setFont(ucg_font_inb38_mr);
  //ucg.setFont(ucg_font_inr42_mr);
  //ucg.setFont(ucg_font_inb42_mr);
  //ucg.setFont(ucg_font_inr46_mr);
  //ucg.setFont(ucg_font_inb46_mr);
  //ucg.setFont(ucg_font_inr49_mr);
  //ucg.setFont(ucg_font_inb49_mr);
  //ucg.setFont(ucg_font_inr53_mr); //3,5 char per row
  //ucg.setFont(ucg_font_inb53_mr);
  //ucg.setFont(ucg_font_inr57_mr);
  //ucg.setFont(ucg_font_inb57_mr); //3 char per row
  //*************************************

  //************My favorite colors:*****
//ucg.setColor(0, 0, 0, 255);   // Set color RED   (0,B,G,R)
//ucg.setColor(0, 0, 255, 0);   // Set color GREEN (0,B,G,R)
//ucg.setColor(0, 255, 0, 0);   // Set color BLUE  (0,B,G,R)  
//ucg.setColor(0, 0, 255, 255); // Set color YELLOW(0,B,G,R)




void setup(void)
{
  delay(1000);
  //ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.begin(UCG_FONT_MODE_SOLID);
  ucg.clearScreen();
  
  ucg.setRotate90(); // Put 90, 180 or 270, or comment to leave default
  ucg.setFont(ucg_font_profont15_mr);
  ucg.setColor(0, 0, 0, 255);  // Set color RED (0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(5,24);  // Set position (x,y)
  ucg.print("Wi-Fi TFT Clock");  // Print text or value
  
  ucg.setColor(0, 127, 0, 255);  // Set color MAGENTA (0,B,G,R)
  ucg.setPrintPos(5,75);  // Set position (x,y)
  ucg.print("IP: 192.168.4.1");  // Print text or value

  WiFiManager wifiManager;
  wifiManager.autoConnect("Wi-Fi TFT Clock");

  ucg.clearScreen();  // Clear the screen
  ucg.setColor(0, 0, 128, 255);  // Set color ORANGE (0,B,G,R)
  ucg.setPrintPos(5,75);  // Set position (x,y)
  ucg.print("Wi-Fi Connected");  // Print text or value
  delay(3000);
  ucg.clearScreen();  // Clear the screen
   
//CHANGE THE POOL WITH YOUR CITY. SEARCH AT https://www.ntppool.org/zone/@
  configTime(0 * 3600, 0, "it.pool.ntp.org", "time.nist.gov");
  setenv("TZ", "GMT-2BST",1);//setenv("TZ", "GMT-2BST",1) = GMT+2

    
  
}
long rssi;
void loop(void)
{
  rssi = WiFi.RSSI();  // eg. -63
  
  time_t now = time(nullptr);
  String time = String(ctime(&now));// ex. Sat Dec 21 09:37:12 2019
  // Serial.println (time);            // ex. Sat Dec 21 09:42:47 2019
  time.trim();                         //.....0123456789
                                    //time_value ex.  09:37:12
                                    //................01234567

  time.substring(20,24).toCharArray(year_value, 10);
  time.substring(4,7).toCharArray(mon_value, 10);
  time.substring(8,10).toCharArray(date_value, 10);
  time.substring(0,3).toCharArray(dow_value, 10);                                  
  time.substring(11,19).toCharArray(time_value, 10); //myString.toCharArray(buf, len)
  time.substring(11,13).toCharArray(hour_value, 10);
  time.substring(14,16).toCharArray(min_value, 10);
  time.substring(17,19).toCharArray(sec_value, 10);


int anul = atoi (year_value);
int luna = atoi (mon_value);
int ziua = atoi(dow_value);
int timpul = atoi(time_value);
int orele = atoi(hour_value);
int minutele = atoi(min_value);
int secundele = atoi(sec_value);
//atoi () este o funcție care transformă un tip de date șir în 
//tip de date întregi în limbaj C.


  
  
  
  
  // Draw rectangle:
  ucg.setRotate90(); // Put 90, 180 or 270, or comment to leave default
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawFrame(0, 0, 160, 128);  // Start from top-left pixel (x,y,wigth,height)

  // Draw line
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawLine(0,25, 160,25);
  
  ucg.setRotate90(); // Put 90, 180 or 270, or comment to leave default
  ucg.setFont(ucg_font_profont22_mr);
  ucg.setColor(0, 0, 0, 255);  // Set color RED (0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(5,20);  // Set position (x,y)
  ucg.print("Wi-Fi CLOCK");  // Print text or value
  
  ucg.setFont(ucg_font_profont10_mr);
  ucg.setColor(0, 0, 255, 255); // Set color YELLOW(0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(140,12);  // Set position (x,y)
  ucg.print(rssi);  // Print text or value
  
  ucg.setPrintPos(140,22);  // Set position (x,y)
  ucg.print("dBm");  // Print text or value
  
  // Draw rounded rectangle:
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawRFrame(5, 30, 150, 30, 5);  // Start from top-left pixel (x,y,wigth,height,radius)
  
  ucg.setFont(ucg_font_profont10_mr);
  ucg.setColor(0, 0, 0, 255);  // Set color RED (0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(10,40);  // Set position (x,y)
  ucg.print("DAY:");  // Print text or value

  ucg.setFont(ucg_font_profont22_mr);
  ucg.setColor(0, 0, 255, 255); // Set color YELLOW(0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(40,52);  // Set position (x,y)
  ucg.print(dow_value);  // Print text or value
  

  // Draw rounded rectangle:
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawRFrame(5, 62, 150, 30, 5);  // Start from top-left pixel (x,y,wigth,height,radius)

  ucg.setFont(ucg_font_profont10_mr);
  ucg.setColor(0, 0, 0, 255);  // Set color RED (0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(10,72);  // Set position (x,y)
  ucg.print("DATE:");  // Print text or value

  ucg.setFont(ucg_font_profont22_mr);
  ucg.setColor(0, 0, 255, 255); // Set color YELLOW(0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(40,84);  // Set position (x,y)
  ucg.print(date_value);  // Print text or value
  ucg.print(mon_value);   // Print text or value
  ucg.print(year_value);  // Print text or value
  
  // Draw rounded rectangle:
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawRFrame(5, 94, 150, 30, 5);  // Start from top-left pixel (x,y,wigth,height,radius)

  ucg.setFont(ucg_font_profont10_mr);
  ucg.setColor(0, 0, 0, 255);  // Set color RED (0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(10,104);  // Set position (x,y)
  ucg.print("TIME:");  // Print text or value

  ucg.setFont(ucg_font_profont22_mr);
  ucg.setColor(0, 0, 255, 255); // Set color YELLOW(0,B,G,R)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(40,116);  // Set position (x,y)
  ucg.print(time_value);  // Print text or value


/*  
// Draw filled rectangle:  
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawBox(65, 5, 60, 30);  // Start from top-left pixel (x,y,wigth,height)
  
// Draw filled Gradient rectangle:
  ucg.setColor(0, 255, 0, 0);
  ucg.setColor(1, 0, 255, 0);
  ucg.setColor(2, 255, 0, 255);
  ucg.setColor(3, 0, 255, 255);
  ucg.drawGradientBox(5, 5, 60, 30);
 
  // Draw filled rounded rectangle:
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawRBox(65, 40, 60, 30, 5);  // Start from top-left pixel (x,y,wigth,height,radius)
  
  // Draw circle:
  ucg.setColor(0, 255, 255);  // Set color YELLOW(B,G,R)
  ucg.drawCircle(30, 90, 15, UCG_DRAW_ALL);  // (x,y,radius,option)

  // Draw filled circle:
  ucg.setColor(0, 0, 255);  // Set color RED(B,G,R)
  ucg.drawDisc(90, 90, 15, UCG_DRAW_ALL);  // (x,y,radius,option)
  
  // Draw triangle:
  ucg.setColor(255, 0, 0);  // Set color BLUE(B,G,R)
  ucg.drawTriangle(40,120,   60,100,   80,120);  // (x0,y0,x1,y1,x2,y2)
  
  // Draw filled triangle:
  ucg.setColor(0, 255, 0);  // Set color GREEN(B,G,R)
  ucg.drawTriangle(100,120,   120,100,   140,120);  // (x0,y0,x1,y1,x2,y2)
  */ 
}
