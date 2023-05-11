// ---------------------------------------------------------------------------
// SimonW, 11May 2023, SPI communication between Arduino Nano and MCP3208
// 
// Development board,Single Board Computer, Arduino Nano, ATmega328, 
// 22 I/O, 32KB Flash, 2KB SRAM, 1KB EEPROM
// Element 14 order code: 	1848691
// Datasheet : https://www.farnell.com/datasheets/1682238.pdf
// 
// ---------------------------------------------------------------------------


#include <SoftwareSerial.h>
// #include "Tlc5940.h"


SoftwareSerial bluetooth(4,2);  // TX 4, RX 2

#define SELPIN          10      // SPI SS, Selection Pin MCP3208 ADC
#define DATAOUT         11      // MOSI 
#define DATAIN          12      // MISO 
#define SPICLOCK        13      // Clock 


#define CHANNEL_0       0
#define CHANNEL_1       1
#define CHANNEL_2       2
#define CHANNEL_3       3
#define CHANNEL_4       4
#define CHANNEL_5       5
#define CHANNEL_6       6
#define CHANNEL_7       7



int readValue;
byte readValuArray[32];
int intensity = 0;


// Function Declaration.
int read_adc(int channel);
void DisplayChannelResult(int channelNo);

void setup() 
{
  // Pin configuration.
  pinMode(SELPIN, OUTPUT);    // adc 1 selection pin
  pinMode(DATAOUT, OUTPUT); 
  pinMode(DATAIN, INPUT); 
  pinMode(SPICLOCK, OUTPUT); 

  // Initially disable devices to start with 
  digitalWrite(SELPIN,HIGH); 
  digitalWrite(DATAOUT,LOW); 
  digitalWrite(SPICLOCK,LOW); 
  Serial.begin(9600);           // For serial debug print. 


}

void loop() 
{

  // Get the ADC output from specified channel
  DisplayChannelResult(CHANNEL_0);

  delay(2000);

}


// Channel number start from 0 (channel 0) to 7 (Channel 7)
int read_adc(int channel)
{
  int adcvalue = 0;
  byte commandbits = B11000000; // command bits - start, mode, chn (3), dont care (3)

  // Allow channel selection
  commandbits|=((channel-1)<<3);

  digitalWrite(SELPIN,LOW); // Select adc

  // Setup bits to be written
  for (int i=7; i>=3; i--)
  {
    digitalWrite(DATAOUT,commandbits&1<<i);
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);    
  }

  // Skip 2 null bits.
  digitalWrite(SPICLOCK,HIGH);    
  digitalWrite(SPICLOCK,LOW);
  digitalWrite(SPICLOCK,HIGH);  
  digitalWrite(SPICLOCK,LOW);

  // Read bits from adc
  for (int i=11; i>=0; i--)
  {
    adcvalue += digitalRead(DATAIN)<<i;
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }
  digitalWrite(SELPIN, HIGH);     // Turn off device

  return adcvalue;
}


void DisplayChannelResult(int channelNo)
{
  readValue = read_adc( channelNo + 1 );
  readValuArray[ 2*channelNo ] = highByte(readValue);
  readValuArray[ 2*channelNo + 1 ] = lowByte(readValue);

  Serial.println(word(readValuArray[ 2*channelNo ],readValuArray[2*channelNo + 1 ]));
}



