#define SER 8
#define CLOCK 9
#define LATCH 10
int LDR=A5;
int LDRP;
int tab[]={B10000000,B01000000,B00100000,B00010000};
void setup() 
{
  pinMode(LATCH,OUTPUT);
  pinMode(CLOCK,OUTPUT);
  pinMode(SER,OUTPUT);
  
  

}

void lounes(byte a) {
 digitalWrite(LATCH,LOW);
 shiftOut(SER,CLOCK,LSBFIRST,tab[a]);
 digitalWrite(LATCH,HIGH);
 delay(9000);

}

void loop() 
{
LDRP=analogRead(LDR);

if (LDRP>0){
     if(LDRP<99 && LDRP>10) {
         lounes(0);
      }
      else if(LDRP<360 && LDRP>100) {
        lounes(1);
       }
      else if(LDRP<1500 && LDRP>363) {
        lounes(2);
       }
     else {
      lounes(3);
       }

}

}
