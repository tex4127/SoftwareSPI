#include "SoftSPI.h"

/*!
 *  @brief Default Constructor for the SoftSPI Device
 */
SoftSPI::SoftSPI(uint8_t cs, uint8_t miso, uint8_t mosi, uint8_t sck)
{
    //We just need to assign our default chip values passed as parameters above
    this->_cs = cs;
    this->_mosi = mosi;
    this->_miso = miso;
    this->_sck = sck;
    //For simplicity, we will default to MSBFIRST for our bit order
    this->_bitOrder = MSBFIRST;
    //missing clock polarity and clock edge
    this->setDataMode(SoftSPI_Mode_e::MODE0_);
}

/*!
 *  @brief initializer for the SPI Class object | Sets the pins and class dependant variables
 */
void SoftSPI::begin()
{
    pinMode(this->_cs, OUTPUT);
    pinMode(this->_miso, INPUT);
    pinMode(this->_mosi, OUTPUT);
    pinMode(this->_sck, OUTPUT);
    digitalWrite(this->_cs, HIGH);
    this->_initialized = true;
}

/*!
 *  @brief Terminates the SPI Class and frees its pins
 */
void SoftSPI::end()
{
    //need to reset all of our pins to inputs and change our init status
    pinMode(this->_cs, INPUT);
    pinMode(this->_miso, INPUT);
    pinMode(this->_mosi, INPUT);
    pinMode(this->_sck, INPUT);
    this->_initialized = false;
}

/*!
 *  @brief getter for the initialization status of the SPI Object
 */
bool SoftSPI::initStatus()
{
    return this->_initialized;
}

/*!
 *  @brief setter for the bit order after SPI Class Creation
 */
void SoftSPI::setBitOrder(SoftSPI_BitOrder_e order)
{
    this->_bitOrder = (uint8_t)order & 1;
}

/*!
 *  @brief setter for the data mode (Clock Polarity and Clock Edge)
 */
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
    for (uint_fast8_t i = 0; i < del; i++){asm volatile("nop");} //Some newness, asm volitile give access to lower level register 
}

/*!
 *  @brief sets our clock divider for timing purposes
 */
void SoftSPI::setClockDivider(SPIClockDiv_e div)
{
    this->_delay = (uint8_t)div;
}   

/*!
 *  @brief Shifts out one byte of data while simultaneously shifting in one byte of data
 */
uint8_t SoftSPI::transfer(uint8_t val)
{
    uint8_t inVal = 0;
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
        val = _;
    }

    uint8_t del = this->_delay >> 1;

    uint8_t bval = 0;

    int sck = (this->_cpol) ? HIGH : LOW;

    for (uint8_t bit = 0; bit < 8; bit++)
    {
        //set our clock either rise or fall depending on SPI MODE (we are using _cedge explicitly here)
        if(this->_cedge)
        {
            sck ^= 1;
            digitalWrite(this->_sck, sck);
            this->wait(del);
        }
        //write the bit for our value to the mosi line. Use 'bit' to iterate through the byte value
        digitalWrite(this->_mosi, ((val * (1<<bit)) ? HIGH : LOW));

        this->wait(del);

        //set our clock to switch so we can read the incoming data bit
        sck ^= 1u; digitalWrite(_sck, sck);

        //read said bit!
        bval = digitalRead(this->_miso);
        if (this->_bitOrder == MSBFIRST)
        {
            //Same as before, we need to ensure the bits are in the expected order
            inVal <<= 1;  //sets the next bit to 1 for the or assignemnt on the next line
            inVal |= bval;
        }
        else
        {
            inVal >>= 1;  //sets the next bit to 1 for the or assignemnt on the next line
            inVal |= bval << 7;
        }
        this->wait(del);

        if (!this->_cedge)
        {
            sck ^= 1u;
            digitalWrite(this->_sck, sck);
        }
        
    }

    return inVal;
    
}

/*!
 *  @brief transfers 2 bytes in and 2 bytes out to a SPI device
 */
uint16_t SoftSPI::transfer16(uint16_t val)
{
    //to simplify our data lets use a union. 
    union
    {
        uint16_t val;
        struct
        {
            uint8_t lsb;
            uint8_t msb;
        };
    }in, out;

    in.val = val;

    if(this->_bitOrder == MSBFIRST){out.msb = this->transfer(in.msb); out.lsb = this->transfer(in.lsb);}
    else{out.lsb = this->transfer(in.lsb); out.msb = this->transfer(in.msb);}
    return out.val;
}


