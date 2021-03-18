#include "RS485.h"
#include <stdarg.h>

typedef unsigned int word;
typedef uint8_t byte;
typedef uint8_t boolean;
typedef void (*voidFuncPtr)(void);
Timer lapse;
    const byte STX = '\2';
    const byte ETX = '\3';

RS485::RS485(PinName tx, PinName rx, PinName dere)
    : BufferedSerial(tx, rx)
{
    return;
}
    

byte RS485::crc8(const byte *addr, byte len)
{
  byte crc = 0;
  while (len--)
    {
    byte inbyte = *addr++;
    for (byte i = 8; i; i--)
      {
      byte mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix)
        crc ^= 0x8C;
      inbyte >>= 1;
      }  // end of for
    }  // end of while
  return crc;
}  // end of crc8

void RS485::sendComplemented( const byte what)
{
  byte c ;
  // first nibble
  c = what >> 4;
  putc((c << 4) | (c ^ 0x0F));

  // second nibble
  c = what & 0x0F;
  putc((c << 4) | (c ^ 0x0F));
}  // end of sendComplemented


void RS485::sendMsg(const byte * data, const byte length)
{
  putc(STX);  // STX
  for (byte i = 0; i < length; i++)
    sendComplemented (data[i]);
  putc(ETX);  // ETX
  sendComplemented(crc8(data, length));
}  // end of sendMsg

// receive a message, maximum "length" bytes, timeout after "timeout" clock_mseconds
// if nothing received, or an error (eg. bad CRC, bad data) return 0
// otherwise, returns length of received data
byte RS485::recvMsg (byte * data,                    // buffer to receive into
              const byte length,              // maximum buffer size
              unsigned long timeout)          // clock_mseconds before timing out
  {

  unsigned long start_time = lapse.read_ms();

  bool have_stx = false;

  // variables below are set when we get an STX
  bool have_etx;
  byte input_pos;
  bool first_nibble;
  byte current_byte;

  while (lapse.read_ms() - start_time < timeout)
    {
    if (readable() > 0)
      {
      byte inByte = getc();

      switch (inByte)
        {

        case STX:   // start of text
          have_stx = true;
          have_etx = false;
          input_pos = 0;
          first_nibble = true;
          start_time = lapse.read_ms();  // reset timeout period
          break;

        case ETX:   // end of text
          have_etx = true;
          break;

        default:
          // wait until packet officially starts
          if (!have_stx)
            break;

          // check byte is in valid form (4 bits followed by 4 bits complemented)
          if ((inByte >> 4) != ((inByte & 0x0F) ^ 0x0F) )
            return 0;  // bad character

          // convert back
          inByte >>= 4;

          // high-order nibble?
          if (first_nibble)
            {
            current_byte = inByte;
            first_nibble = false;
            break;
            }  // end of first nibble

          // low-order nibble
          current_byte <<= 4;
          current_byte |= inByte;
          first_nibble = true;

          // if we have the ETX this must be the CRC
          if (have_etx)
            {
            if (crc8 (data, input_pos) != current_byte)
              return 0;  // bad crc
            return input_pos;  // return received length
            }  // end if have ETX already

          // keep adding if not full
          if (input_pos < length)
            data [input_pos++] = current_byte;
          else
            return 0;  // overflow
          break;

        }  // end of switch
      }  // end of incoming data
    } // end of while not timed out

  return 0;  // timeout
} // end of recvMsg