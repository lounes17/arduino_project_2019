// Reciever Code
 // Pins Connection
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
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   48
#define CSN_PIN 49

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Write Same Address of Transmitter
char income[11];
char re[]={'L','E','D','_','R','E','D'};
char gr[]={'L','E','D','_','G','R','E','E','N'};
char bl[]={'L','E','D','_','B','L','U','E'};
char off[]={'L','E','D','_','O','F','F'};
// pin connections of LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int l=5;
int r=0,f=0,g=0,b=0;
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/

const int green=9;
const int red=8;
const int blue=10;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  digitalWrite(blue,LOW);
  digitalWrite(green,LOW);
  digitalWrite(red,LOW);
  lcd.begin(16, 2);
  // Print a message to the LCD at Startup.
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Arafa Micro-sys.");
  lcd.setCursor(0,1);
  lcd.print("Wireless Comm.");
  delay(5000);
  lcd.clear();
  lcd.print("REC: ");
  lcd.setCursor(0,1);
  lcd.print("LED COLOR:OFF  ");
  // NRF Initializing, and Setup.
  radio.begin();
  //Choose a high channel in order_matrix to prevent an interference.
  radio.setChannel(70);
  // optionally, reduce the payload size.  seems to
  radio.setPayloadSize(11); // improve reliability
  radio.setPALevel( RF24_PA_MAX ) ; // max power
  radio.setDataRate( RF24_250KBPS ) ; //Min speed (for better range. from Datasheet)
  radio.setCRCLength( RF24_CRC_8 ) ; //use 8-bit CRC
  radio.setRetries(15,15);// increase the delay between retries & # of retries 
  radio.openReadingPipe(1,pipe);// Open Reading Pipe to Receiving data
  radio.startListening(); // Start listening on the pipes opened for reading.
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
   if (radio.available()){
    bool done = false;  
    while(!done)
    {
     
      done = radio.read( income, sizeof(income) );
      
    }
    
        lcd.setCursor(5,0);
        lcd.print("           ");
    
    for(int j=0;j<11;j++)
    {
      if(income[j] != (char)-1)
      {
        lcd.setCursor(l,0);
        lcd.print(income[j]);
        l++; 
      }
     
    }
    for(int m=0;m<7;m++)
    {
      if(income[m]==re[m])r++;
    }
    for(int z=0;z<9;z++)
    {
      if(income[z]==gr[z])g++;
    }
    for(int x=0;x<8;x++)
    {
      if(income[x]==bl[x])b++;
    }
    for(int y=0;y<7;y++)
    {
      if(income[y]==off[y])f++;
    }
    
    if(g==9)
    {
      digitalWrite(red,LOW);
      digitalWrite(green,HIGH);
      digitalWrite(blue,LOW);
      lcd.setCursor(0,1);
      lcd.print("LED COLOR:GREEN");
    }else if(f==7)
    {
      digitalWrite(red,LOW);
      digitalWrite(green,LOW);
      digitalWrite(blue,LOW);
      lcd.setCursor(0,1);
      lcd.print("LED COLOR:OFF  ");
    }else if(b==8)
    {
      digitalWrite(red,LOW);
      digitalWrite(green,LOW);
      digitalWrite(blue,HIGH);
      lcd.setCursor(0,1);
      lcd.print("LED COLOR:BLUE ");
    }else if(r==7)
    {
      digitalWrite(red,HIGH);
      digitalWrite(green,LOW);
      digitalWrite(blue,LOW);
      lcd.setCursor(0,1);
      lcd.print("LED COLOR:RED  ");
    }
    l=5;
    r=0;
    g=0;
    b=0;
    f=0;
    
    
       
    }
   
  
}//--(end main loop )---

