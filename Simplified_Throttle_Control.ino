//Implement Software Serial for multiple serial prints? So far only have one Serial output
//Firmata to communicate with labview?

#include <Servo.h>

Servo myservo;  // create servo object to control a servo 

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9
  myservo.write(0); // sets servo to 0
  Serial.flush(); // clears searial data
  Serial.begin(2400);  // setup serial at 2400 bps
} 

// ------------------Initialize Variables-------------------------------

const int dyno = 2; // set dyno input pin number
volatile float V; // define voltage variable for raw dyno input
float currenttime; // define current time variable
const float time1 = 10000; // define time for first test loop ms 10s
const float time2 = 20000; // define time for second test loop ms 20s
const float time3 = 30000; // define time for third test loop ms 30s
const float endtime = 40000; // define test end time ms 40s
float desV; // global desired voltage
const float desV1 = 2; // define desired voltage for speed
const float desV2 = 4;
const float desV3 = 5;
float oldV; // old voltage variable for speed
float pos = 0; // angle of servo position value
float oldpos = 0; // angle of servo old position
int del = 500; // delay time in ms
float error; // actual voltage subtracted from desired voltage


void loop()
{
  while(currenttime < endtime) {
    currenttime = (float)millis();  // defines time in milliseconds
    //Serial.write(currenttime); // write current time to labview
    if (currenttime < time1) {
       desV = desV1;
       Serial.print('Test 1');
       Serial.println(desV);
       break;
    }
    if (currenttime < time2 && currenttime > time1) {
      desV = desV2;
      Serial.print('Test 2');
      Serial.println(desV,2);
      break;
    }
    if (currenttime < time3 && currenttime > time2) {
      desV = desV3;
      Serial.print('Test 3');
      Serial.println(desV,2);
      break;
    }
    
     
// test sequence
    V = analogRead(dyno); // read voltage value from dyno output 0-5V and assign to variable
    if (V == desV) {
      Serial.println(V,2); // prints voltage of dyno input
      Serial.println(desV,2); // prints desired voltage of dyno
      Serial.print(myservo.read());  // prints servo position
      Serial.println(currenttime,3); // prints current time
      break;
    } 
    else {
      pos = map(V, 0, 1023, 0, 180); // scales voltage value to servo position
      if (pos == 180 || pos == 0) {
        break;
      }
      error = V - desV;
      Serial.println(error,2);
      if (V < desV) {
        oldpos = pos; // set the current position to the old position
        pos = oldpos++; // increment the position counter to increase servo angle
        myservo.write(pos); // write servo position
        delay(del); // delay continuation of loop
        break;
      }
      else {
        pos = oldpos; // set current position as old position
        pos = oldpos--; // decrement the position variable to decrease servo angle
        myservo.write(pos); // write servo position
        delay(del); // delay continuation of loop
        break;
      }
     }
 }
 Serial.print('Test Ended');
}
