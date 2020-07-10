
 // Tx_ACK Code By : Arafa Micro-Systems
 //Channel on Youtube : www.youtube.com/arafamicrosystems
 // Pin Connections for Arduino UNO
 /*
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
*/




/* import needed Libraries*/
#include <SPI.h> // Call SPI Library 
#include <nRF24L01.h> // Call NRF Version Library
#include <RF24.h> // RF24 Header File

/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10


// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CSN_PIN); // Set up nRF24L01 radio on SPI bus plus pins 9 & 10
int rx=0;
char tx[32]="";
int time,duration;
byte retries=0;
bool tx_flag=false;

void setup(void)
{
  
  Serial.begin(9600);
  radio.begin();
  // We will be using the Ack Payload feature, so please enable it
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(15,15);
  // Open pipes to other nodes for communication
  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.
  radio.openWritingPipe(pipe);
 
}

void loop(void)
{
  
    // Take the time, and send it.  This will block until complete
    
   if(Serial.available())
   {
    delay(32);
    for(int i=0;i<32;i++)
    {
      tx[i]=Serial.read();
      if(tx[i]==(char)-1)
      {
        tx[i]='\0';
        goto lable;
      }
    }
     lable:
     tx_flag=true;
   }
   
   
   
   if(tx_flag)
   {
     bool ok = radio.write(tx, sizeof(tx));
     if (ok)
     {
       tx_flag=false;
       time=micros();
       if ( radio.isAckPayloadAvailable() )
       {
         radio.read(&rx,sizeof(unsigned int));
         duration=micros()-time;
         Serial.print("Ack: ");
         Serial.print(rx);
         Serial.print("    Tx: ");
         Serial.print(tx);
         Serial.print("   Time: ");
         Serial.print(duration);
         Serial.println();
       }    
     }
   }
  
}


  

// vim:ai:cin:sts=2 sw=2 ft=cpp
