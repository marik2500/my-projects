   //////////////////////////////////////////////
  //       Arduino Rotary Encoder Menu        //
 //                 v1.0                     //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radiohf(9, 10);
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };
#include <EEPROM.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

#include <RDA5807M.h>
RDA5807M radio;


int menuitem = 1;
int frame = 1;
int page = 1;
int lastMenuItem = 1;

String menuItem1 = "Frequency";
String menuItem2 = "Volume";
String menuItem3 = "Temperature";
String menuItem4 = "Contrast";
String menuItem5 = "Light: ON";
String menuItem6 = "Reset";

boolean backlight = true;
int contrast = EEPROM.read(201);
int volume = EEPROM.read(202);
float f = EEPROM.get(203,f);
float temp1, temp2, temp3;




boolean up = false;
boolean down = false;
boolean middle = false;

ClickEncoder *encoder;
int16_t last, value;

Adafruit_PCD8544 display = Adafruit_PCD8544( 7, 6, 5, 4, 3); 
// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!



void setup()
{
  
radio.init();
radio.setBandFrequency(RADIO_BAND_FM, f);
radio.setVolume(volume);

radiohf.begin();
radiohf.setPALevel ( RF24_PA_MAX ) ;
radiohf.setDataRate(RF24_250KBPS);
radiohf.openReadingPipe(1, pipes[1]);
radiohf.openReadingPipe(2, pipes[2]);
radiohf.openReadingPipe(3, pipes[3]);
radiohf.startListening();


  pinMode(8,OUTPUT);
  turnBacklightOn();
  
  encoder = new ClickEncoder(A1, A0, A2);
  encoder->setAccelerationEnabled(false);
   
  display.begin();      
  display.clearDisplay(); 
  setContrast();  

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = encoder->getValue();
}

void loop()


 
{
  if ( radiohf.available() )
{
delay(50);
radiohf.read(&temp1, sizeof(temp1));//view themperature from sensor 1
}
  drawMenu();

  readRotaryEncoder();

   ClickEncoder::Button b = encoder->getButton();
   if (b != ClickEncoder::Open) {
   switch (b) {
      case ClickEncoder::Clicked:
         middle=true;
        break;
    }
  }    
  
  if (up && page == 1 ) {
     
    up = false;
    if(menuitem==2 && frame ==2)
    {
      frame--;
    }

     if(menuitem==4 && frame ==4)
    {
      frame--;
    }
      if(menuitem==3 && frame ==3)
    {
      frame--;
    }
    lastMenuItem = menuitem;
    menuitem--;
    if (menuitem==0)
    {
      menuitem=1;
    } 
  }else if (up && page == 2 && menuitem==1 ) 
  {
    up = false;
    f=f-10;
    if(f < 8800)
    {
      f = 8800;
    }
    radio.setBandFrequency(RADIO_BAND_FM, f);
    EEPROM.put(203,f);
  }
  else if (up && page == 2 && menuitem==2 ) 
  {
    up = false;
    volume--;
    EEPROM.write(202,volume);
    if (volume<0)
    {
    volume=0;
    }
    radio.setVolume(volume);
  }
  
  else if (up && page == 2 && menuitem==3 ) 
   {
    up = false;
    down = false;
   }
    
  
    else if (up && page == 2 && menuitem==4 ) 
    {
    up = false;
    contrast=contrast-5;
    EEPROM.write(201,contrast);
    if (contrast<0)
         {
         contrast=0;
         }
    setContrast();
  }

  if (down && page == 1) //We have turned the Rotary Encoder Clockwise
  {

    down = false;
    if(menuitem==3 && lastMenuItem == 2)
    {
      frame ++;
    }else  if(menuitem==4 && lastMenuItem == 3)
    {
      frame ++;
    }
     else  if(menuitem==5 && lastMenuItem == 4 && frame!=4)
    {
      frame ++;
    }
    lastMenuItem = menuitem;
    menuitem++;  
    if (menuitem==7) 
    {
      menuitem--;
    }
  
  }else if (down && page == 2 && menuitem==1) {
    down = false;
    f=f+10;
    if(f > 10800)
    {
      f = 10800;
    }
    radio.setBandFrequency(RADIO_BAND_FM, f);
    EEPROM.put(203,f);
  }
  else if (down && page == 2 && menuitem==2) {
    down = false;
    volume++;
    EEPROM.write(202,volume);
    if (volume > 15)
    {
      volume = 15;
    }
    radio.setVolume(volume);
  }
   else if (down && page == 2 && menuitem==3 ) 
   {
    up = false;
    down = false;
  }
  else if (down && page == 2 && menuitem==4 ) 
  {
    down = false;
    contrast=contrast+5;
    EEPROM.write(201,contrast);
    if (contrast>100)
    {
      contrast=100;
    }
    setContrast();
  }
  
  if (middle) //Middle Button is Pressed
  {
    middle = false;
   
    if (page == 1 && menuitem==5) // Backlight Control 
    {
      if (backlight) 
       {
        backlight = false;
        menuItem5 = "Light: ON";
        turnBacklightOff();
        }
      else 
       {
        backlight = true; 
        menuItem5 = "Light: OFF";
        turnBacklightOn();
       }
    }

    if(page == 1 && menuitem ==6)// Reset
    {
      resetDefaults();
    }


    else if (page == 1 && menuitem<=4) 
     {
      page=2;
     }
      else if (page == 2) 
     {
      page=1; 
     }
   }   
  }
  
  void drawMenu()
  {
    
  if (page==1) 
  {    
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.print(" FM RADIO MENU");
    display.drawFastHLine(0,10,83,BLACK);

    if(menuitem==1 && frame ==1)
    {   
      displayMenuItem(menuItem1, 15,true);
      displayMenuItem(menuItem2, 25,false);
      displayMenuItem(menuItem3, 35,false);
    }
    else if(menuitem == 2 && frame == 1)
    {
      displayMenuItem(menuItem1, 15,false);
      displayMenuItem(menuItem2, 25,true);
      displayMenuItem(menuItem3, 35,false);
    }
    else if(menuitem == 3 && frame == 1)
    {
      displayMenuItem(menuItem1, 15,false);
      displayMenuItem(menuItem2, 25,false);
      displayMenuItem(menuItem3, 35,true);
    }
     else if(menuitem == 4 && frame == 2)
    {
      displayMenuItem(menuItem2, 15,false);
      displayMenuItem(menuItem3, 25,false);
      displayMenuItem(menuItem4, 35,true);
    }

      else if(menuitem == 3 && frame == 2)
    {
      displayMenuItem(menuItem2, 15,false);
      displayMenuItem(menuItem3, 25,true);
      displayMenuItem(menuItem4, 35,false);
    }
    else if(menuitem == 2 && frame == 2)
    {
      displayMenuItem(menuItem2, 15,true);
      displayMenuItem(menuItem3, 25,false);
      displayMenuItem(menuItem4, 35,false);
    }
    
    else if(menuitem == 5 && frame == 3)
    {
      displayMenuItem(menuItem3, 15,false);
      displayMenuItem(menuItem4, 25,false);
      displayMenuItem(menuItem5, 35,true);
    }

    else if(menuitem == 6 && frame == 4)
    {
      displayMenuItem(menuItem4, 15,false);
      displayMenuItem(menuItem5, 25,false);
      displayMenuItem(menuItem6, 35,true);
    }
    
      else if(menuitem == 5 && frame == 4)
    {
      displayMenuItem(menuItem4, 15,false);
      displayMenuItem(menuItem5, 25,true);
      displayMenuItem(menuItem6, 35,false);
    }
      else if(menuitem == 4 && frame == 4)
    {
      displayMenuItem(menuItem4, 15,true);
      displayMenuItem(menuItem5, 25,false);
      displayMenuItem(menuItem6, 35,false);
    }
    else if(menuitem == 3 && frame == 3)
    {
      displayMenuItem(menuItem3, 15,true);
      displayMenuItem(menuItem4, 25,false);
      displayMenuItem(menuItem5, 35,false);
    }
        else if(menuitem == 2 && frame == 2)
    {
      displayMenuItem(menuItem2, 15,true);
      displayMenuItem(menuItem3, 25,false);
      displayMenuItem(menuItem4, 35,false);
    }
    else if(menuitem == 4 && frame == 3)
    {
      displayMenuItem(menuItem3, 15,false);
      displayMenuItem(menuItem4, 25,true);
      displayMenuItem(menuItem5, 35,false);
    }   
    display.display();
  }
  else if (page==2 && menuitem == 1) 
  {    
   displayFrequencyMenuPage(menuItem1, f/100);
  }
  else if (page==2 && menuitem == 2) 
  {
   displayVolumeMenuPage(menuItem2, volume);
  }
   else if (page==2 && menuitem == 3) 
  {
   displayTemperatureMenuPage(menuItem3, temp1);
  }
  else if (page==2 && menuitem == 4) 
  {
   displayIntMenuPage(menuItem4, contrast);
  }
  
  
  }

  void resetDefaults()
  {
    contrast = 50;
    volume = 5;
    f = 10430;
    radio.setBandFrequency(RADIO_BAND_FM, f);
    temp1;
    setContrast();
    backlight = true;
    menuItem5 = "Light: ON";
    turnBacklightOn();
  }

  void setContrast()
  {
    display.setContrast(contrast);
    display.display();
  }

  void turnBacklightOn()
  {
    digitalWrite(8,HIGH);
  }

    void turnBacklightOff()
  {
    digitalWrite(8,LOW);
  }

  void timerIsr() {
  encoder->service();
}
void displayVolumeMenuPage(String menuItem, int value)
{
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print(menuItem);
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(value*5);
    display.print("%");
    display.setTextSize(2);
    display.display();
}
void displayIntMenuPage(String menuItem, int value)
{
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print(menuItem);
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("Value");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(value);
    display.setTextSize(2);
    display.display();
}
void displayFrequencyMenuPage(String menuItem, float value)
{
  display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print(menuItem);
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("MHz");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(value,1);
    display.setTextSize(2);
    display.display();
}
void displayTemperatureMenuPage(String menuItem, float value)
{
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print(menuItem);
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("Celsius");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(value,1);
    display.print((char)247);
    display.setTextSize(2);
    display.display();
}
void displayStringMenuPage(String menuItem, String value)
{
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print(menuItem);
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("Value");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.print(value);
    display.setTextSize(2);
    display.display();
}

void displayMenuItem(String item, int position, boolean selected)
{
    if(selected)
    {
      display.setTextColor(WHITE, BLACK);
    }else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.setCursor(0, position);
    display.print(">"+item);
}

void readRotaryEncoder()
{
  value += encoder->getValue();
  
  if (value/2 > last) {
    last = value/2;
    down = true;
    delay(150);
  }else   if (value/2 < last) {
    last = value/2;
    up = true;
    delay(150);
  }
}



