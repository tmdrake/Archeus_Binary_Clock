/*
Clock driver for binary clock, used internal RTC in the Teensy CPU
10-13-2024 - Coded by Drake Dragon, Fast/slow buttions added.
Using Teensy 3.2 as the core CPU.
 */


#include <TimeLib.h> //For RTC

#include "Timer.h" //for multi tasking
Timer t;


void setup()  {
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  Serial.begin(115200);
  //while (!Serial);  // Wait for Arduino Serial Monitor to open
  delay(100);
  if (timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }

//pinMode(LED_BUILTIN, OUTPUT);
  
pinMode(23, OUTPUT);  //Minuite L, 8
pinMode(22, OUTPUT);  //Minuite L, 4
pinMode(21, OUTPUT);  //Minuite L, 2
pinMode(20, OUTPUT);  //Minuite L, 1
pinMode(19, OUTPUT);  //Second H, 4
pinMode(18, OUTPUT);  //Second H, 2
pinMode(17, OUTPUT);  //Second H, 1
pinMode(16, OUTPUT);  //Second L, 8
pinMode(15, OUTPUT);  //Second L, 4
pinMode(14, OUTPUT);  //Second L, 2
pinMode(13, OUTPUT);  //Second L, 1
pinMode(12, OUTPUT);  //Minuite H, 4
pinMode(11, OUTPUT);  //Minuite H, 2
pinMode(10, OUTPUT);  //Minuite H, 1
pinMode(9, OUTPUT);  //Hour, 8
pinMode(8, OUTPUT);  //Hour, 4
pinMode(7, OUTPUT);  //Hour, 2
pinMode(6, OUTPUT);  //Hour, 1

//pinMode(4, INPUT_PULLUP);  //Input Hold
pinMode(3, INPUT_PULLUP);  //Input Fast
pinMode(2, INPUT_PULLUP);  //Input Slow
pinMode(1, INPUT_PULLUP);  //Input Lamp test

t.every(100, binaryClockOutputs);

}

void loop() {
  if (Serial.available()) {
    time_t t = processSyncMessage();
    if (t != 0) {
      Teensy3Clock.set(t); // set the RTC
      setTime(t);
    }
  }
  //digitalClockDisplay();  
  //delay(1000);
  //Serial.println(second() % 2);

/*
  if (second() % 2 == 0) //using the RTC to flash LED_PIN every second
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
*/

  //binaryClockOutputs();
  t.update();


  if (digitalRead(3)==0)
    {
      //triggers the fast incriment
      delay(20);
      //adjustTime(60);
      setTime(hour(), minute()+1, second(), day(), month(), year());
      binaryClockOutputs();
    }
  
  if (digitalRead(2)==0)
    {
      //triggers the slow incriment
      delay(20);
      //adjustTime(1);
      setTime(hour(), minute(), second()+1, day(), month(), year());
      binaryClockOutputs();
    }
  if (digitalRead(1)==0)
    lampTest();
}
/***********************************/

void binaryClockOutputs()
{

//Computes hours in 12 hour format
byte hour_12  = hour(); //% 12;

//Correction for 12 hour format
if (hour_12 == 0 )
  hour_12 = 12;
else if (hour_12 > 12 )
  hour_12 = hour_12 - 12; // We only want 12 hour time

  Serial.print("Hours:");
  Serial.println(hour_12, BIN);
  digitalWrite(6, hour_12 & 0b1);
  digitalWrite(7, hour_12 & 0b10);
  digitalWrite(8, hour_12 & 0b100);
  digitalWrite(9, hour_12 & 0b1000);  
  //Serial.println(hour_12);

//COmpute the Minuits in BCD format
  byte minute_low = minute() % 10;
  byte minute_high = (byte) minute() / 10;
  Serial.print("Minuites:");
  //Serial.print(minute_high);
  Serial.print(minute_high, BIN);
  digitalWrite(10, minute_high & 0b1);
  digitalWrite(11, minute_high & 0b10);
  digitalWrite(12, minute_high & 0b100);  
  //Serial.println(minute_low);
  Serial.print(" ");
  Serial.println(minute_low, BIN);
  digitalWrite(20, minute_low & 0b1);
  digitalWrite(21, minute_low & 0b10);
  digitalWrite(22, minute_low & 0b100);
  digitalWrite(23, minute_low & 0b1000);

//COmpute the Seconds in BCD format
  byte seconds_low = second() % 10;
  byte seconds_high = (byte) second() / 10;
  Serial.print("Seconds:");
  //Serial.print(seconds_high);
  Serial.print(seconds_high, BIN);
  
  digitalWrite(17, seconds_high & 0b1);
  digitalWrite(18, seconds_high & 0b10);
  digitalWrite(19, seconds_high & 0b100);
  //Serial.println(seconds_low);
  Serial.print(" ");
  Serial.println(seconds_low, BIN);
  digitalWrite(13, seconds_low & 0b1);
  digitalWrite(14, seconds_low & 0b10);
  digitalWrite(15, seconds_low & 0b100);
  digitalWrite(16, seconds_low & 0b1000);
}




/*void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  //Serial.print(" ");
  //Serial.print(day());
  //Serial.print(" ");
  //Serial.print(month());
  //Serial.print(" ");
  //Serial.print(year()); 
  Serial.println(); 
}*/

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     return pctime;
     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
       pctime = 0L; // return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void lampTest() {
  digitalWrite(23, HIGH);  //Minuite L, 8
  digitalWrite(22, HIGH);  //Minuite L, 4
  digitalWrite(21, HIGH);  //Minuite L, 2
  digitalWrite(20, HIGH);  //Minuite L, 1
  digitalWrite(19, HIGH);  //Second H, 4
  digitalWrite(18, HIGH);  //Second H, 2
  digitalWrite(17, HIGH);  //Second H, 1
  digitalWrite(16, HIGH);  //Second L, 8
  digitalWrite(15, HIGH);  //Second L, 4
  digitalWrite(14, HIGH);  //Second L, 2
  digitalWrite(13, HIGH);  //Second L, 1
  digitalWrite(12, HIGH);  //Minuite H, 4
  digitalWrite(11, HIGH);  //Minuite H, 2
  digitalWrite(10, HIGH);  //Minuite H, 1
  digitalWrite(9, HIGH);   //Hour, 8
  digitalWrite(8, HIGH);   //Hour, 4
  digitalWrite(7, HIGH);   //Hour, 2
  digitalWrite(6, HIGH);   //Hour, 1
}

