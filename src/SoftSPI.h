///////////////////////////////////////////////
///
//  Copyright @ 5/10/2024 Jacob Garner, mlgtex4127@gmail.com
//
//  File Name: SoftSPI.h
//
//  Description: This file contains the header for the Software Spi Library (SoftSPI)
//  That was created to allow for a more portable (if slower) version of SPI communication
//  accross various libraries and chipsets (mainly the ESP32, Teensy and Keyestudio Arduinos)
//  to make my life simpler at work.
//
///
///////////////////////////////////////////////

#ifndef __SOFTSPI_HH__
#define __SOFTSPI_HH__

#include "Arduino.h"
#include <SPI.h>

typedef enum
{
    MODE0_ = 0x00, //CPOL = 0 | Data Shifted on Falling Clock Edge
    MODE1_ = 0x04, //CPOL = 0 | Data Shifted on Rising Clock Edge 
    MODE2_ = 0x08, //CPOL = 1 | Data Shifted on Rising Clock Edge
    MODE3_ = 0x0C  //CPOL = 1 | Data Shifted On Falling Clock Edge
} SoftSPI_Mode_e;

typedef enum
{
    LSBFIRST_ = 0x00,
    MSBFIRST_ = 0x01
} SoftSPI_BitOrder_e;

typedef enum
{
    SOFTSPI_CLOCK_DIV4 = 0x00,
    SOFTSPI_CLOCK_DIV16,
    SOFTSPI_CLOCK_DIV64,
    SOFTSPI_CLOCK_DIV128,
    SOFTSPI_CLOCK_DIV2,
    SOFTSPI_CLOCK_DIV8,
    SOFTSPI_CLOCK_DIV32
} SPIClockDiv_e;

class SoftSPI : public SPIClass
{
    public:
    SoftSPI(uint8_t cs, uint8_t miso, uint8_t mosi, uint8_t sck);
    void begin();
    void end();
    bool initStatus();
    void setBitOrder(SoftSPI_BitOrder_e order);
    void setDataMode(SoftSPI_Mode_e mode);
    void setClockDivider(SPIClockDiv_e div);
    uint8_t transfer(uint8_t);
    uint16_t transfer(uint16_t);
    void wait(uint8_t val);
    private:
    bool _initialized = false;
    uint8_t _bitOrder;
    uint8_t _cs;
    uint8_t _miso;
    uint8_t _mosi;
    uint8_t _sck;
    uint8_t _cpol;
    uint8_t _cedge;
    uint8_t _clockDiv;
    uint8_t _delay;

};
#endif