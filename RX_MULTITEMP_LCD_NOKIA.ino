#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

float temp1, temp2, temp3;

RF24 radio(9, 10);
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };

void setup(void) {
//Serial.begin(9600);
display.begin();       
display.setContrast(60);
display.display(); // show splashscreen
delay(2000);
display.clearDisplay();
  
radio.begin();
radio.setPALevel ( RF24_PA_MAX ) ;
radio.setDataRate(RF24_250KBPS);
radio.openReadingPipe(1, pipes[1]);
radio.openReadingPipe(2, pipes[2]);
radio.openReadingPipe(3, pipes[3]);
radio.startListening();




display.setCursor(0,10);
display.setTextSize(2);
display.setTextColor(BLACK, WHITE);
display.print("Wi-Fi");

display.setCursor(0,30);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.print("Thermometer");

display.display();
delay(2000);

}

void loop(void)
{

if ( radio.available() )
{
delay(50);
radio.read(&temp1, sizeof(temp1));//view themperature from sensor 1

Serial.println(temp1);

display.setCursor(0,0);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.print("Wi-Fi O)))");
display.setCursor(0,10);
display.setTextSize(1);
display.print("Thermometer");
display.setCursor(0,17);
display.setTextSize(1);
display.print("...");
display.setCursor(0,20);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.print("T1:");
display.setTextSize(1);;
display.print(temp1,1);
display.print((char)247);
display.print('C');

display.display();
delay(50);
display.clearDisplay();

radio.read(&temp2, sizeof(temp2));//view themperature from sensor 2

Serial.println(temp2);

display.setCursor(0,30);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.print("T2:");
display.setTextSize(1);
display.print(temp2,1);
display.print((char)247);
display.print('C');
}
else
{
display.setCursor(0,0);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.print("Wi-Fi");
display.setTextSize(1);
display.print("Thermometer");
display.setCursor(0,10);
display.setTextSize(1);
display.print("_____________________");

display.setCursor(0,20);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.print("T1:");
display.setTextSize(1);;
display.print(temp1,1);
display.print((char)247);
display.print('C');

display.setCursor(0,30);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.print("T2:");
display.setTextSize(1);
display.print(temp2,1);
display.print((char)247);
display.print('C');

display.display();
delay(50);
//display.clearDisplay();
}

//delay(1000);
//LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
