
// RX_ACK Code By : Arafa Micro-Systems
 //Channel on Youtube : www.youtube.com/arafamicrosystems
 // Pins Connection for Arduino Mega (Change the Connection For Arduino uno)
 /*
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 48
   4 - CSN to Arduino pin 49
   5 - SCK to Arduino pin 52
   6 - MOSI to Arduino pin 51
   7 - MISO to Arduino pin 50
   8 - UNUSED
*/


/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   48
#define CSN_PIN 49


// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
int tx=1;
char rx[32]="";
void setup(void)
{
  
  Serial.begin(9600);
  //delay(1500);
  radio.begin();
  // We will be using the Ack Payload feature, so please enable it
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(15,15);
  // Open pipes to other nodes for communication
  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.
  radio.openReadingPipe(1,pipe);
  radio.startListening();

}

void loop(void)
{

    // if there is data ready
    if ( radio.available() )
    {
      radio.writeAckPayload( 1, &tx, sizeof(unsigned int) );
      radio.read( rx, sizeof(rx) );
      Serial.print("REC: ");
      Serial.print(rx);
      Serial.print("    Tx: ");
      Serial.print(tx);
      Serial.println();
      tx++;
    }
  }

