 // Transceiver 2 Code By : Arafa Micro-Systems
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

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe[2] = {0xE8E8F0F0E1LL, 0xE8E8F0F0F2LL}; // Define the transmitter pipe as Pipe[1] and Receiver Pipe as Pipe[0] Unlike the Transceiver 1
int tx=1;
char rx[32]="";
bool tx_flag=false;


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  radio.begin();//Intializing NRF24L01 Module
  //radio.setRetries(15,15);// increase the delay between retries & Number of retries 
  radio.openWritingPipe(pipe[1]);// For Transmitting Operation, Must OpenWritingPipe
  radio.openReadingPipe(1,pipe[0]); //For Receiving Operation
  radio.startListening();
}//--(end setup )---
 void loop()
 {

   
   
   if(tx_flag)
   {
     radio.stopListening();
     bool ok = radio.write(&tx, sizeof(unsigned int));
     if(ok)
     {
       radio.startListening();
       tx_flag=false;
       tx++;
      }
   }
   // Check Data Availability on Air
   if (radio.available()){
    bool done = false;  
    while(!done)
    {
     
      done = radio.read( rx, sizeof(rx) );
      
    }
    Serial.print("REC: ");
    Serial.print(rx);
    Serial.print("    Tx: ");
    Serial.print(tx);
    Serial.println();
    tx_flag=true;
    }
   
 
   

 }//end main loop
