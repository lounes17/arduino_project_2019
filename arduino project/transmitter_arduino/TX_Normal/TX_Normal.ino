 // Transceiver 1 Code By : Arafa Micro-Systems
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

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe[2] = {0xE8E8F0F0E1LL, 0xE8E8F0F0F2LL}; // Define the transmitter pipe as Pipe[0] and Receiver Pipe as Pipe[1]
int rx=0;
char tx[32]="";
int time,duration;
bool tx_flag=false;

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Set up nRF24L01 radio on SPI bus plus pins 9 & 10
/*-----( Declare Variables )-----*/

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  radio.begin();//Intializing NRF24L01 Module
  //radio.setRetries(15,15);// increase the delay between retries & Number of retries 
  radio.openWritingPipe(pipe[0]);// For Transmitting Operation, Must OpenWritingPipe
  radio.openReadingPipe(1,pipe[1]); //For Receiving Operation
  radio.startListening();
}//--(end setup )---


 void loop()
 {
   // the First Part of the Code (For Transmitting Operation)
   if(Serial.available())
   {
       //For Data Collection
       delay(100); // Data Collection Time(take it by trials)
       for(int i=0;i<32;i++)
       {
           tx[i]=Serial.read();
           if(tx[i] == (char)-1)//For check an Empty Characters
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
     radio.stopListening();
     bool ok = radio.write(tx, sizeof(tx));
     if(ok)
     {
       
       tx_flag=false;
       time=micros();  
       radio.startListening();
     }
   }

   // Check Data Availability on Air
   if (radio.available()){
    bool done = false;  
    while(!done)
    {
     
      done = radio.read( &rx, sizeof(unsigned int) );
      
    }
     duration=micros()-time;
     Serial.print("Tx: ");
     Serial.print(tx);
     Serial.print("     Ack: ");
     Serial.print(rx);
     Serial.print("   Time: ");
     Serial.print(duration);
     Serial.println();
    }
   
 
   

 }//end main loop
