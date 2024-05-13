
#include "SoftSPI.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
        ((byte) & 0x80 ? '1' : '0'), \
        ((byte) & 0x40 ? '1' : '0'), \
        ((byte) & 0x20 ? '1' : '0'), \
        ((byte) & 0x20 ? '1' : '0'), \
        ((byte) & 0x08 ? '1' : '0'), \
        ((byte) & 0x04 ? '1' : '0'), \
        ((byte) & 0x02 ? '1' : '0'), \
        ((byte) & 0x01 ? '1' : '0')

SoftSPI* ssDev;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Software SPI EXAMPLE Script");
  ssDev = new SoftSPI(9,11,12,13);
  //Set the mode and bit order
  ssDev->setBitOrder(SoftSPI_BitOrder_e::MSBFIRST_);
  ssDev->setDataMode(SoftSPI_Mode_e::MODE0_);
  ssDev->setClockDivider(SPIClockDiv_e::SOFTSPI_CLOCK_DIV2);

  Serial.println("Spi Object Created. Starting Comms now");
}

void loop() 
{
  /*
    To Send/Receive one byte of data use ssDev->transfer(uint8_t)
  */  
  uint8_t out = 0b00000000;
  uint8_t in = ssDev->transfer(out);
  //Serial.printf("Data Sent to SPI Device:   " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(out));
  //Serial.printf("Data Recieved from Device: " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(in));

  /*
    To Send/Recieve two bytes of data use ssDev->transfer16(uint16_t)
  */
  /*
  union
  {
    uint16_t val;
    struct
    {
      uint8_t msb;
      uint8_t lsb;
    }
  } in,out;
  out.val = 0b0000000000000000;
  in.val = ssDev->transfer16(out.val);
  Serial.printf("Data Sent to SPI Device:   " BYTE_TO_BINARY_PATTERN " | " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(out.msb), BYTE_TO_BINARY(out.lsb));
  Serial.printf("Data Recieved from Device: " BYTE_TO_BINARY_PATTERN " | " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(in.msb), BYTE_TO_BINARY(in.lsb));
  */
}
