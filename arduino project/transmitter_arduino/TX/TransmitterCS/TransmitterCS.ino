 // Transmitter Code
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
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
char trans[11];
boolean Ack;

/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Set up nRF24L01 radio on SPI bus plus pins 9 & 10
/*-----( Declare Variables )-----*/

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);

  radio.begin();//Intializing NRF24L01 Module
  //Choose a high channel in order_matrix to prevent an interference.
  // optionally, reduce the payload size.  seems to
  radio.setPayloadSize(11); // improve reliability
  radio.setPALevel( RF24_PA_MAX ) ; // max power
  radio.setDataRate( RF24_250KBPS ) ; //Min speed (for better range from Datasheet)
  radio.setRetries(15,15);// increase the delay between retries & # of retries 
  radio.openWritingPipe(pipe);// For Transmitting Operation, Must OpenWritingPipe
   
}//--(end setup )---


 void loop()
 {
   // Check for available Serial Data
   if(Serial.available())
   {
       //For Data Collection
       delay(11); 
       for(int i=0;i<11;i++)
       {
           trans[i]=Serial.read();
           
           if(trans[i] != (char)-1)
           {
                Serial.print(trans[i]);//Print the input Data
           }
       
       } 
        Serial.println();
        //Send data twice, because of delays in the code
        for(int m=0;m<2;m++)
        {
             
             Ack = radio.write(trans, sizeof(trans));
             
        }
        //Check the Acknowledgment, and print it.
        Serial.print("Ack---> ");
        if(Ack==true)
        {
          Serial.print("Succeeded");
          Serial.println();
        }else if(Ack==false)
        {
          Serial.print("Failed");
          Serial.println();
        }
        
            
       
   }
   

 }//end main loop
