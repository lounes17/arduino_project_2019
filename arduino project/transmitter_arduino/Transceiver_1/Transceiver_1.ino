// Transceiver 1
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <avr/wdt.h>

const uint64_t pipe[4]={0x1212121234LL, 0x1212121256LL, 0xABABABABD1LL, 0xABABABABD2LL};
RF24 radio(9,10);
int i=0;
char re=0;
char income[32];
int sender_id;
byte retx=0;
uint8_t pipe_num;
void setup()
{
  Serial.begin(9600);
  //delay(1000);
  radio.begin();
  print_welcome_message();
  radio.openReadingPipe(1,pipe[2]);
  radio.openReadingPipe(2,pipe[3]);
  radio.startListening();

}

void software_Reboot()
{
  wdt_enable(WDTO_15MS);
}

void print_welcome_message()
{
     Serial.print("Choose a Transceiver\n");
     Serial.print(",You want to send a Message(Transc2= 1, Transc3= 2\n");
     Serial.print("or Wait for Receiving");
     Serial.println(); 
}


void select(int rec)
{
  Serial.print("Transceiver, which was chosen=Transc ");
  Serial.print(rec);
  Serial.println();
}

void trans()
{
    Serial.println("Enter a Message you want to send (up to 32 Characters)");
    while(!Serial.available());
      delay(32);
      char tx[32];
      for(int j=0;j<32;j++)
      {
       tx[j]=Serial.read();
       if(tx[j]==(char)-1)
       {
         tx[j]='\0';
         goto loop1;
       }
      }
      loop1:
     loop2: 
     bool done = radio.write(tx, sizeof(tx));
     if(done==false)
     { 
       retx++;
       Serial.println("Transmission Faild,Retransmit");
       if(retx<10)
       {
       goto loop2;
       }else retx=0;
     }
     radio.startListening();
     print_welcome_message();
}
void loop()
{
  if(Serial.available())
  {
    re=Serial.read();
    if(re=='1')
    {
      radio.stopListening();
      radio.openWritingPipe(pipe[0]);
      select(2);
      trans();
    }else if(re=='2')
    {
      radio.stopListening();
      radio.openWritingPipe(pipe[1]);
      select(3);
      trans();
    }else if(re=='R')
    {
      software_Reboot();
    }else
    {
      Serial.println("Error Selection");
    }
  }else if(radio.available(&pipe_num))
  {
    bool done=false;
      while(!done)
      {
        done = radio.read( income, sizeof(income) );
      }
       Serial.print("Received = ");
      Serial.print(income);
      Serial.println();
      sender_id = pipe_num;
      if(sender_id==1)
      {
      Serial.print("From Transceiver 2");
      Serial.println();
      }else if(sender_id==2)
      {
       Serial.print("From Transceiver 3");
       Serial.println();
      }
      //print_welcome_message();
  }
  //radio.powerUp(); 
}



