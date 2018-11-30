#include "LowPower.h"

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temp1;

RF24 radio(9, 10);
const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };

void setup(void) {
//Serial.begin(9600);
sensors.begin();
radio.begin();
radio.setPALevel ( RF24_PA_MAX ) ;
radio.setDataRate(RF24_250KBPS);
radio.openWritingPipe(pipes[1]);
//radio.startListening();
}

void loop(void)
{
sensors.requestTemperatures();
temp1 = sensors.getTempCByIndex(0);
//Serial.println(temp1);
radio.write(&temp1, sizeof(temp1));
delay(100);
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
//LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

}
