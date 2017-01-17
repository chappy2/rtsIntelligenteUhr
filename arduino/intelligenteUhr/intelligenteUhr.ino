
#include <TimeLib.h>

// single character message tags
#define TIME_HEADER   'T'   // Header tag for serial time sync message
#define FORMAT_HEADER 'F'   // Header tag indicating a date format message
#define FORMAT_SHORT  's'   // short month and day strings
#define FORMAT_LONG   'l'   // (lower case l) long month and day strings

#define TIME_REQUEST  7     // ASCII bell character requests a time sync message 

static boolean isLongFormat = true;

  
void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  if(isLongFormat)
    Serial.print(dayStr(weekday()));
  else  
   Serial.print(dayShortStr(weekday()));
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  if(isLongFormat)
     Serial.print(monthStr(month()));
  else
     Serial.print(monthShortStr(month()));
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}
void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void  processFormatMessage() {
   char c = Serial.read();
   if( c == FORMAT_LONG){
      isLongFormat = true;
      Serial.println(F("Setting long format"));
   }
   else if( c == FORMAT_SHORT) {
      isLongFormat = false;   
      Serial.println(F("Setting short format"));
   }
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 - paul, perhaps we define in time.h?

   pctime = Serial.parseInt();
   if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
     setTime(pctime); // Sync Arduino clock to the time received on the serial port
   }
}

time_t requestSync() {
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}

/*
Stundenraster definitions
*/
// 0-6 hour, 7-12 min
// id i hour, i+6 min
uint8_t Anfangszeit[12]={8,9,12,13,15,17,0,30,0,40,20,0};
uint8_t Endzeit[12]={9,11,13,15,16,18,30,15,30,10,50,30};
// Anfangszeit Zeit z
int greaterTime(int i, uint8_t hour, uint8_t minute){
  if (Anfangszeit[i] < hour){
    return 1;
  }
  if (hour == Anfangszeit[i]){
    if ( Anfangszeit[i+6] <= minute){
    return 1;
  }
    }
  
  return 0;
};

// Endzeit Zeit z
int lessTime(int i, uint8_t hour, uint8_t minute){
  if ( Endzeit[i] > hour){
    return 1;
  }
  if ( Endzeit[i] == hour){
    if (Endzeit[i+6] >= minute){
      return 1;
    }
  }
  return 0;
};

void setStundenraster(int currentMonth){
  uint8_t anf3_h,anf3_m, end3_h, end3_m;
   if (currentMonth > 8|| currentMonth <= 2){
        Anfangszeit[2]=12;
        Anfangszeit[2+6]=0;
        Endzeit[2]=13;
        Endzeit[2+6]=30;
   }else {
    Anfangszeit[2]=11;
     Anfangszeit[2]=30;
     Endzeit[2]=12;
         Endzeit[2+6]=0;
    } 
}

int isTimeInCurrentBlock(int i, uint8_t hour, uint8_t  minute){
  if (hour==0){return 0;}
  if(greaterTime(i, hour, minute)==0){
    return 0;
    }
  
  if( lessTime(i, hour, minute)==0){
    return 0;
  }
  return 1;
}

int isCurrentlyPause(uint8_t hour, uint8_t  minute){
 if(greaterTime(0, hour, minute)==0){
    return 0;
    }
  
  if( lessTime(5, hour, minute)==0){
    return 0;
  }
  return 1;
}

uint8_t getTimeLeft(int ende, uint8_t hour, uint8_t  minute){
    uint8_t minutesLeft=0;
    if ((Endzeit[ende]-hour)==0){
      return Endzeit[ende+6]-minute;
     }
      if ((Endzeit[ende]-hour)>=1){
        minutesLeft=60-minute;
        minutesLeft+=Endzeit[ende+6];
         if ((Endzeit[ende]-hour)==2){
          minutesLeft+=60;
         }   
          
      }
       return minutesLeft;
}
int getCurrentBlock ( uint8_t hour, uint8_t minute){
  int result=-1;
  for(int i=0; i<6; i++) {
    if (isTimeInCurrentBlock(i, hour, minute)==1){
      result = i;
    }
  }
  
  return result;
}
/**
 *     
LED Definitions

byte LEDARRAY_D, byte LEDARRAY_C, 
      byte LEDARRAY_B, byte LEDARRAY_A, 
      byte LEDARRAY_G, byte LEDARRAY_DI, 
      byte LEDARRAY_CLK, byte LEDARRAY_LAT
 */

#include "LedMatrixObject.h"

//LedMatrixObject *ledOben = new LedMatrixObject(A3, A2, A1, A0, 13, 10, 11, 12);
//LedMatrixObject *ledUnten = new LedMatrixObject(8, 7, 6, 5, 9, 4, 3, 2);
LedMatrixObject *led = new LedMatrixObject(2, 3, 4, 5, 6, 7, 8, 9,13, 12, A1, A2, A0, A3, A4, A5);

void setup(){
  Serial.begin(9600);
  //while (!Serial) ; // Needed for Leonardo only
  //setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
  if (timeStatus()!= timeNotSet) {
    setStundenraster(month());
  }
}
  void setScenes(uint8_t timeLeft){
    //unsigned char Oben[16][16];
    //unsigned char Unten[16][16];
    float percentage=((float)timeLeft/90.0);
    int ledsOff=((1.0-percentage)*(16*32));
    int secondsOffset=(second()/60.0)*5.688;
    ledsOff=ledsOff+secondsOffset;
    //Serial.println(ledsOff);
    if (ledsOff>=(16*32)){
      led->displayDoneScene();
      return;
     }
    int counterSetted=0;
    // obere szene immer durch iterieren und leds auschalten
     
      for (byte ii=0;ii<16;ii++){
        for(byte ji=0;ji<16;ji++){
          if (counterSetted<=ledsOff){
            //Oben[ii][ji]=1;
            led->setLedOff2(ii,ji);
            counterSetted++;
         } else {
              //Oben[ii][ji]=0;
              led->setLedOn2(ii,ji);
           }
        }
      }
       //led->setScene2(Oben);
        for (byte ii=0;ii<16;ii++){
          for(byte ji=0;ji<16;ji++){
            if (counterSetted<ledsOff){
                led->setLedOff(ii,ji);
              counterSetted++;
            } else {
               led->setLedOn(ii,ji);
              }
          }
        }
        //led->setScene(Unten);
        led->draw();
    }
    
void loop(){
  uint8_t timeLeft;
   int block=getCurrentBlock(hour(),minute());
  if(block==-1){
    if (isCurrentlyPause(hour(),minute())==1){
      led->displayPauseScene();
      } else {
        led->displayHomeScene();
        }
    } else if (block==0 || block==1 || block==2 || block==3 || block==4 || block==5){ 
      timeLeft=getTimeLeft(block,hour(),minute());
        setScenes(timeLeft);  
      }
 if (Serial.available() > 1) { // wait for at least two characters
    char c = Serial.read();
    if( c == TIME_HEADER) {
      processSyncMessage();
       setStundenraster(month());
    }
   
  }
}


