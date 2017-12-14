const int RCLK = 5; //RCLK 
const int SCLK = 7; //SCLK
const int DIO = 6;  //DIO

byte value[] ={ B11000000, // 0
                B11111001, // 1
                B10100100, // 2
                B10110000, // 3
                B10011001, // 4
                B10010010, // 5
                B10000010, // 6
                B11111000, // 7
                B10000000, // 8
                B10010000, // 9
                B01000000, // 0.
                B01111001, // 1.
                B00100100, // 2.
                B00110000, // 3.
                B00011001, // 4.
                B00010010, // 5.
                B00000010, // 6.
                B01111000, // 7.
                B00000000, // 8.
                B00010000, // 9.
                B10111111, // -
                B11111111};// display nothing

byte digit[] ={ B00000000,
                B00000001, // left segment
                B00000010,
                B00000100,
                B00001000,
                B00010000,
                B00100000,
                B01000000,
                B10000000}; // right segment
                                
unsigned long int seconds=0;

String acpt_cmd;

void setup() {
  Serial.begin(9600);

  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  
  cli();//stop interrupts
  TCCR1A = 0x00;                // Normal mode, just as a Timer

  TCCR1B |= _BV(CS12);          // prescaler = CPU clock/1024
  TCCR1B &= ~_BV(CS11);       
  TCCR1B |= _BV(CS10);    
  
  TIMSK1 |= _BV(TOIE1);         // enable timer overflow interrupt
  
  TCNT1 = -15625;               // Ticks for 1 second @16 MHz,prescale=1024
  sei(); //allow interrupts
}

ISR (TIMER1_OVF_vect)
{
   seconds = (seconds + 1) % 86400;
   TCNT1 = -15625;               // Ticks for 1 second @16 MHz,prescale=1024
}

void loop()
{
  
//  digitalWrite(RCLK,LOW);
//  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // select all segments
//  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // display nothing
//  digitalWrite(RCLK,HIGH); 

   static unsigned int ss, mm, hh;
   
   if(Serial.available() > 0){
        acpt_cmd = Serial.readString();
//        Serial.print("You typed: ");
//        Serial.println(acpt_cmd);
        seconds=acpt_cmd.toInt();
   }

   
   hh = (unsigned int) (seconds / 3600);
   mm = (unsigned int) (seconds / 60) - (60 * hh);
   ss = (unsigned int) (seconds % 60);

   disp(1, (int) hh / 10);
   disp(2, (int) hh % 10);
   sign1(3);
   disp(4, (int) mm / 10);
   disp(5, (int) mm % 10);
   sign1(6);
   disp(7, (int) ss / 10);
   disp(8, (int) ss % 10);
}

void showtime(int second)
{
   disp(1,2);
   disp(2,3);
   sign1(3);
   disp(4,3);
   disp(5,4);
   sign1(6);
   disp(7,5);
   disp(8,6);
}

void disp(int d , int num ){          //透過本動作函數 disp( 位數， 數字 ); 依序顯示
//  digitalWrite(RCLK,LOW);
//  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // select all segments
//  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // display nothing
//  digitalWrite(RCLK,HIGH); 

  digitalWrite(RCLK,LOW);
  // select the fourth segment from left
  shiftOut(DIO,SCLK,MSBFIRST,value[num]); 
  shiftOut(DIO,SCLK,MSBFIRST,digit[9-d]);  
  digitalWrite(RCLK,HIGH);
  }
void sign1(int d ){                            //透過本動作函數 sign( 位數 ) ; 加上負號
//  digitalWrite(RCLK,LOW);
//  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // select all segments
//  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // display nothing
//  digitalWrite(RCLK,HIGH); 

  digitalWrite(RCLK,LOW);
  // select the fourth segment from left
  shiftOut(DIO,SCLK,MSBFIRST,value[20]);  
  shiftOut(DIO,SCLK,MSBFIRST,digit[9-d]); 
  digitalWrite(RCLK,HIGH);
  }
  
void sign(int d ){                            //透過本動作函數 sign( 位數 ) ; 加上負號
  digitalWrite(RCLK,LOW);
  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // select all segments
  shiftOut(DIO,SCLK,MSBFIRST,B11111111); // display nothing
  digitalWrite(RCLK,HIGH); 

  digitalWrite(RCLK,LOW);
  // select the fourth segment from left
  shiftOut(DIO,SCLK,MSBFIRST,value[20]);  
  shiftOut(DIO,SCLK,MSBFIRST,digit[5-d]); 
  digitalWrite(RCLK,HIGH);
  }
