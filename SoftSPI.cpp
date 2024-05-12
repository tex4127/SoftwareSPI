#include "SoftSPI.h"

SoftSPI::SoftSPI(int8_t cs, uint8_t miso, uint8_t mosi, uint8_t sck)
{
    //We just need to assign our default chip values passed as parameters above
    this->_cs = cs;
    this->_mosi = mosi;
    this->_miso = miso;
    this->_sck = sck;
    //For simplicity, we will default to MSBFIRST for our bit order
    this->_bitOrder = MSBFIRST;
    //missing clock polarity and clock edge
    this->_spiMode = SoftSPI_Mode_e::MODE0;
}

void SoftSPI::begin()
{
    pinMode(this->cs, OUTPUT);
    pinMode(this->miso, INPUT);
    pinMode(this->mosi, OUTPUT);
    pinMode(this->sck, OUTPUT);
    digitalWrite(_cs, HIGH);
    this->_initialized = true;
}

void SoftSPI::end()
{
    //need to reset all of our pins to inputs and change our init status
    pinMode(this->cs, INPUT);
    pinMode(this->miso, INPUT);
    pinMode(this->mosi, INPUT);
    pinMode(this->sck, INPUT);
    this->_initialized = false;
}
clockDi
bool SoftSPI::initStatus()
{
    return this->_initialized;
}

void SoftSPI::setBitOrder(SoftSPI_BitOrder_e order)
{
    this->_bitOrder = (uint8_t)bo & 1;
}

void SoftSPI::setDataMode(SoftSPI_Mode_e mode)
{
    switch (mode)
    {
        case SoftSPI_Mode_e::MODE0_:
        this->_cpol = 0;
        this->_cedge = 0;
        break;
        case SoftSPI_Mode_e::MODE1_:
        this->_cpol = 0;
        this->_cedge = 1;
        break;
        case SoftSPI_Mode_e::MODE2_:
        this->_cpol = 1;
        this->_cedge = 1;
        break;
        case SoftSPI_Mode_e::MODE3_:
        this->_cpol = 1;
        this->_cedge = 0;
        break;
        default:
        //default to MODE0
        this->_cpol = 0;
        this->_cedge = 0;
        break;
    }
    digitalWrite(this->_sck, (this->_cpol) ? HIGH : LOW);
}

/*!
 *  @brief simple Wait method accessing lower level registers; note the ASM VOLATILE keyword(s)
 */
void SoftSPI::wait(uint_fast8_t del)
{
    for (uint_fast8_t i = 0; i < del, i++){asm volatile("nop");} //Some newness, asm volitile give access to lower level register 
}

/*!
 *  @brief sets our clock divider for timing purposes
 */
void SoftSPI::setClockDivider(SPIClockDiv_e div)
{
    uint8_t _clockDiv = (uint8_t)div;
}   

/*!
 *  @brief Shifts out one byte of data 
 */
uint8_t SoftSPI::transfer(uint8_t val)
{
    //we need to account for both bitorders
    if (this->_bitOrder == MSBFIRST)
    {
        uint8_t _ = ((val & 0x01) << 7) |
                    ((val & 0x02) << 5) |
                    ((val & 0x04) << 3) |
                    ((val & 0x08) << 1) |
                    ((val & 0x10) >> 1) |
                    ((val & 0x20) >> 3) |
                    ((val & 0x40) >> 5) |
                    ((val & 0x80) >> 7);
        val - _;
    }

    uint8_t del = this->_delay - 1;

    
}
