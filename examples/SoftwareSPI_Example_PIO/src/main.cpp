#include <Arduino.h>
#include "../../../src/SoftwareSPI.h"

SSPIClass* sspi;
uint8_t cs = 10;
uint8_t mosi = 11;
uint8_t miso = 12;
uint8_t sck = 13;
uint8_t dataOrder = MSBFIRST;
uint8_t dataMode = SPI_MODE0;
uint32_t spiClockDiv = SPI_CLOCK_DIV2;

void setup() {
  Serial.println("Starting SoftwareSPI.h Example");
  //The Constructor requires the mosi and miso, all other parameters are optional.
  sspi = new SSPIClass(mosi, miso, sck, dataOrder, dataMode, spiClockDiv);
  //When writting data to an IC, we need to trigger a chip select, configure that peripherally to the SSPIClass object
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
}

void loop() {
  unsigned long startTime = millis();
  //write data to the device
  uint8_t outData = 0x00;
  digitalWrite(cs, LOW);
  sspi->transfer(outData);
  digitalWrite(cs, HIGH);
  delay(10);
  //now Read our Data into a new byte
  uint8_t inData = sspi->transfer(0xFF);
  Serial.printf("Data Written -> %03u\n");
  Serial.printf("Data Received -> %03u\n");
  while (millis() - startTime < 1000)
    ;
}

