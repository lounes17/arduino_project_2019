#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>



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







/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe[2] = {0xE8E8F0F0E1LL, 0xE8E8F0F0F2LL}; // Define the transmitter pipe as Pipe[0] and Receiver Pipe as Pipe[1]
char income[32]=""; //Array of Characters for Receiver
char input[32]="";
int count=0;
bool tx_mode=false;

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Set up nRF24L01 radio on SPI bus plus pins 9 & 10
/*-----( Declare Variables )-----*/
bool done = false; 
void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(115200);
  radio.begin();//Intializing NRF24L01 Module
  radio.setRetries(15,15);// increase the delay between retries & Number of retries 
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
       delay(32); // Data Collection Time(take it by trials)
       for(int i=0;i<32;i++)
       {
           input[i]=Serial.read();
           if(input[i] == (char)-1)//For check an Empty Characters
           {
             input[i]='\0';
             goto lable;
           }
       }
      lable:
      tx_mode=true;      
   }
   
   
   if(tx_mode)
   {
     radio.stopListening();
     bool ok = radio.write(input, sizeof(input));
     if(ok)
     {
       radio.startListening();
       tx_mode=false;
     }
   }else if(tx_mode==false)
   {

   // Check Data Availability on Air
   if(radio.available())
   {
     bool done=false;
   while (!done){
    done=radio.read( income, sizeof(income) );
    }
    for(int j=0;j<32;j++)
    {
    if(income[j]>0)count++;
    }
    Serial.print(income);
    Serial.println();
    Serial.print("Numofbytes= ");
    Serial.print(count);
    count=0;
    Serial.println();
   }
   
 }
   

 }//end main loop

