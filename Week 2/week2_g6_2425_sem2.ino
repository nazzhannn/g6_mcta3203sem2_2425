#include "SevSeg.h" //use library that easy to use with seven segment display

SevSeg S; //Instantiate a seven segment controller object    
byte CommonPins[] = {};            // common pin numbers for multi-digit display(array)    
byte SegPins[] = {2,3,4,5,11,7,8};  // 7-segment display pins in the order,{a,b,c,d,e,f,g,dp}(array)

int button1=9;   //button for increment
int button2=10;  //button for reset
int num=0;


void setup()
{
  // begin(COMMON_CATHODE, NumberOfDigits, CommonPins[], SevenSegPins[], resistorUsed);
  
  S.begin(COMMON_CATHODE, 1, CommonPins, SegPins, 1);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
}

void loop()
{
    int incre = digitalRead(9);
    int reset = digitalRead(10);

    //increment
    if((incre == HIGH) && (num < 9))
    {
      delay(100);
      num++;
    }
    
     //reset
    if((reset == HIGH))
    {
      delay(100);
      num=0;
    }

    //Logic to print digit/character on 7 segment display
    S.setNumber(num);
    S.refreshDisplay();
    delay(100);
}
