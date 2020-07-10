// Transceiver 3
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

LiquidCrystal lcd(8, 7, 5, 4, 3, 2);
const uint64_t pipe[4]={0xABABABABD2LL, 0x1212121299LL, 0x1212121256LL, 0x1212121278LL};
RF24 radio(9,10);
int i=0;
char re=0;
char income[32];
int sender_id;
byte retx=0;
byte mask=0;
byte count=0;
uint8_t pipe_num;
const int sw1=0;
const int sw2=1;
bool ok=false;
bool s1=true,s2=true;
void setup()
{
  pinMode(sw1,INPUT_PULLUP);
  pinMode(sw2,INPUT_PULLUP);
  lcd.begin(16, 2);
  radio.begin();
  radio.openReadingPipe(1,pipe[2]);
  radio.openReadingPipe(2,pipe[3]);
  radio.startListening();
  print_welcome_message();

}


void print_welcome_message()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("To send Amessage");
  lcd.setCursor(0,1);
  lcd.print("Click on Sw1/Sw2");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Or Wait for");
  lcd.setCursor(0,1);
  lcd.print("Receiving");
}


void select(int rec)
{
  lcd.clear();
  lcd.print("Send a message");
  lcd.setCursor(0,1);
  lcd.print("to Transceiver");
  lcd.print(rec);

}
void trans(char tx[])
{
 lable:
    ok=radio.write(tx,sizeof(tx));
    if(ok==false)
    {
      retx++;
      if(retx<10)
      {
        goto lable;
      }else retx=0;
    }
    radio.startListening();
    

}


void loop()
{
  s1=digitalRead(sw1);
  s2=digitalRead(sw2);
    if(s1==false)
    {
      delay(100);
      radio.stopListening();
      radio.openWritingPipe(pipe[0]);
      select(1);
      trans("SW1");
      while(digitalRead(sw1)==false);
    }
    
    if(s2==false)
    {
      delay(100);
      radio.stopListening();
      radio.openWritingPipe(pipe[1]);
      select(2);
      trans("SW2");
      while(digitalRead(sw2)==false);
    }
    
    
  if(radio.available(&pipe_num))
  {
    bool done=false;
      while(!done)
      {
        done = radio.read( income, sizeof(income) );
      }
      sender_id = pipe_num;
      if(sender_id==1)
      {
        lcd.clear();
        lcd.print("From Transceiver");
        lcd.setCursor(0,1);
        lcd.print("1");
      }else if(sender_id==2)
      {
        lcd.clear();
        lcd.print("From Transceiver");
        lcd.setCursor(0,1);
        lcd.print("2");
      }
      delay(2000);
      lcd.clear();
      lcd.print("Rec=");
      for(int c=0;c<32;c++)
      {
        if(income[c]=='\0')
       {
         goto lable2;
       }else count++; 
      }
 lable2:
      lcd.setCursor(4,0);
      for(int u=0;u<count;u++)
      { 
        if(u<=11)
        {
        lcd.print(income[u]); 
        
        }else
        {
          if(mask==0)
          {
            lcd.setCursor(0,1);
            mask=1;
          }
        
        lcd.print(income[u]); 
        
        }     
      }
      count=0;
      mask=0;

      
      //print_welcome_message();
  }
  //radio.powerUp(); 
}



