// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

// Adapted by B. Frank, 2021/09/23 for Mod 1
// This sketch will check the IR sensor connected to A0 once per second; if it detects
// an object in front of the sensor it will drive the motor for one second. Otherwise it will
// turn the motor off for one second.

#include <AFMotor.h>
#define pushButtonPin A5
#define dipOnePin A2
#define dipTwoPin A3
#define dipThreePin A4
#define pushButtonPin A5

bool dipOneState = LOW;
bool dipTwoState = LOW;
bool dipThreeState = LOW;

int sensorPassesNeeded = 0;
int floorSelection;
int currentFloor;
int sensorPassesDone = 0;

AF_DCMotor DCmotor(1, MOTOR12_1KHZ); // DC motor connected to M1 on motor control board
bool IRstate = false;   // this holds the state of the IR sensor
bool pushButtonState = HIGH; // this holds the state of the push button
void setup() {

  pinMode(pushButtonPin, INPUT_PULLUP);  // set the pushButtonPin to being input, with a pullup resistor connected
  // Setup DIP switch switches to be outputs
  pinMode(dipOnePin, OUTPUT);
  pinMode(dipTwoPin, OUTPUT);
  pinMode(dipThreePin, OUTPUT);
  Serial.begin (115200);    // set up Serial library at 115200 bps
  Serial.println("Motor test!");

  // Turn motor off

  DCmotor.run(RELEASE);

}

void loop() {
  DCmotor.setSpeed(255);
  pushButtonState = digitalRead(pushButtonPin);

  // Check which floor was selected by the user
  if (dipOneState == LOW) {
    floorSelection = 1;
  } else if (dipTwoState == LOW) {
    floorSelection = 2;
  } else if (dipThreeState == LOW) {
    floorSelection = 3;
  }

  // Calculate number of IR sensor passes needed

  // Check that user isnt asking t otravle to current floor
  if (!(floorSelection == currentFloor)) {
    sensorPassesNeeded = floorSelection - currentFloor;
    pushButtonState = digitalRead(pushButtonPin);
    if (pushButtonState == HIGH) {
      while (true) {
        // Run motor once button is pressed
        DCmotor.run(FORWARD);
        // Check whether or not the IR sensor has detected anything
        IRstate = digitalRead(A0);
        if (IRstate == false) {
          // If anything is detected, increase sensor pass count
          sensorPassesDone += 1;
        }
        // Check if needed sensor passes are completed
        if (sensorPassesDone == sensorPassesNeeded) {
          DCmotor.run(RELEASE);
          // Reset counter
          sensorPassesDone = 0;
          break;
        }
      }
    }
  }
}
