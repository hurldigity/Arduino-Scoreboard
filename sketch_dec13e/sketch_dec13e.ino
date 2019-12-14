// 5 pins trip wire
#include <Arduino.h>
#include <TM1637Display.h>
int num = 0;
int lastScore;
#define TEST_DELAY   2000
// Module connection pins (Digital Pins)
#define CLK 6
#define DIO 7
TM1637Display display(CLK, DIO);
// Variables
int mode = 1;
int ambiant;
int ambiant2;
int trip = 1000; // The light value I get when using my laser
int trip2 = 1000;
int minLight = 900;
int makeBeep = 1; //0 for no beep, 1 for beep!
int atAverage;
int atAverage2;
long millisCount;

// Output Pins
int laserPin = 2;
int laserPin2 = 1;
int ledPin = 13;
int buzzerPin = 3;
String  modeNames[3] = {"SETTINGS","ARMED","TRIP"};

// Input Pins
int modePin = 4; 
int tripPin = A0;
int ambiantPin = A1;

int tripPin2 = A2;
int ambiantPin2 = A3;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(laserPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(modePin, INPUT);
  Serial.begin(9600);
}


void loop() {
  display.setBrightness(0x0f);
  display.showNumberDec(num,true);

  // When the button is pushed
  if (digitalRead(modePin) == 1) {
    trip = analogRead(tripPin);
    trip2 = analogRead(tripPin2);
    mode=mode + 1;
    if (mode >= 2) {
      mode = 0;
    }
    beep(1);
    delay(300);
  }

  if (num==10||num==110||num==210||num==310||num==410||num==510||num==610||num==710||num==810||num==910||num>=1000){
  //if((num<11 && num!=0 && (num % 10 == 0))||num>=1000){
      beep(10);
      delay(5000);
      num=0;
      display.showNumberDec(num,true);
    }

  
  //does something when the mode changes
  switch (mode) {
    case 0: //calibration mode
      digitalWrite(laserPin,HIGH);
      digitalWrite(laserPin2,HIGH);
      
      ambiant = analogRead(ambiantPin);
      ambiant2 = analogRead(ambiantPin2);
      trip = analogRead(tripPin);
      trip2 = analogRead(tripPin2);
      atAverage = ambiant + ((trip - ambiant)/2);
      atAverage2 = ambiant2 + ((trip2 - ambiant2)/2);
      stats();
      
      if (trip  >= minLight) {
        digitalWrite(ledPin,HIGH);
      } else {
        digitalWrite(ledPin,LOW);
      }

    break;

    case 1: // Armed mode
      digitalWrite(laserPin,HIGH);
      digitalWrite(laserPin2,HIGH);
      digitalWrite(ledPin,LOW);
      ambiant = analogRead(ambiantPin);
      ambiant2 = analogRead(ambiantPin2);
      atAverage = ambiant + ((trip - ambiant)/2);
      atAverage2 = ambiant2 + ((trip2 - ambiant2)/2);
      if (analogRead(tripPin) < atAverage){
        lastScore=3;
        mode = 2;
      }
      if(analogRead(tripPin2) < atAverage2){
        lastScore=4;
        mode = 2;
      }
      if ((millis() - millisCount) >= 1000) {
        millisCount = millis();
        stats();
      }
    break;
    
    case 2: //Trip Mode
      if ((millis() - millisCount) >= 1000) {
        millisCount = millis();
        stats();
        score();
        mode = 1;
      }
    break;
    }
  delay(1);                       // wait for a bit
}

//It Beeps
void beep(int qty) {
  int count;
  if (makeBeep == 1) {
    for (count = 1;count<=qty;count++) {
      digitalWrite(buzzerPin, HIGH);
      delay(50);
      digitalWrite(buzzerPin, LOW);
      delay(50);
    }
  }
}

void score(){
  if(lastScore==3){
    num++;
  }
  if(lastScore==4){
    num=num+100;
  }
  display.setBrightness(0x0f);
  display.showNumberDec(num,true);
}

//Writes stats to the Serial Port
void stats() {
  Serial.print("A:");
  Serial.print(ambiant);
  Serial.print(" T:");
  Serial.print(trip);
  Serial.print(" AT:");
  Serial.print(atAverage);
  Serial.print(" MODE:");
  Serial.print(modeNames[mode]);
  Serial.println("");
}
