// DigiMouse Mouse Wiggler
// Jeff White (jwhite@white.nu)
// MIT License

#include "DigiMouse.h"

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_DOWN 3
#define MOUSE_UP 4

// Milliseconds range to move the mouse
#define MIN_DELAY_TIME 100000
#define MAX_DELAY_TIME 150000

// Pixel range to move the mouse
#define MOUSE_MIN 1
#define MOUSE_MAX 9

unsigned int moveamount = 2;
unsigned int mousemove = MOUSE_LEFT;
unsigned long endtime;
unsigned long currtime;
int rollovers;

void setup() {
  //Do nothing? It seems as if the USB hardware is ready to go on reset
  randomSeed(analogRead(0));
  endtime = millis() + random(MIN_DELAY_TIME, MAX_DELAY_TIME);
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A 
  
}

int millisRollover() {
  // get the current millis() value for how long the microcontroller has been running
  //
  // To avoid any possiblity of missing the rollover, we use a boolean toggle that gets flipped
  //   off any time during the first half of the total millis period and
  //   then on during the second half of the total millis period.
  // This would work even if the function were only run once every 4.5 hours, though typically,
  //   the function should be called as frequently as possible to capture the actual moment of rollover.
  // The rollover counter is good for over 35 years of runtime. --Rob Faludi http://rob.faludi.com
  //
  static int numRollovers=0; // variable that permanently holds the number of rollovers since startup
  static boolean readyToRoll = false; // tracks whether we've made it halfway to rollover
  unsigned long now = millis(); // the time right now
  unsigned long halfwayMillis = 2147483647; // this is halfway to the max millis value (17179868 for earlier versions of Arduino)

  if (now > halfwayMillis) { // as long as the value is greater than halfway to the max
    readyToRoll = true; // you are ready to roll over
  }

  if (readyToRoll == true && now < halfwayMillis) {
    // if we've previously made it to halfway
    // and the current millis() value is now _less_ than the halfway mark
    // then we have rolled over
    numRollovers++; // add one to the count the number of rollovers
    readyToRoll = false; // we're no longer past halfway
    endtime = 0; // prevent rollover
  } 
  return numRollovers;
}

void LEDon() {
  digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, HIGH);
}

void LEDoff() {
  digitalWrite(0, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(1, LOW);
}

void loop() {
  
  DigiMouse.update();//call this at least every 50ms
  //calling more often than that is fine
  //this will actually only send the data every once in a while unless the data is different
  
  if (millis() > endtime) {
    switch (mousemove) {
        case MOUSE_LEFT :
          DigiMouse.moveX(-moveamount);
          LEDon();
          break;
        case MOUSE_RIGHT :
          DigiMouse.moveX(moveamount);
          LEDoff();
          break;
        case MOUSE_DOWN :
          DigiMouse.moveY(moveamount);
          LEDon();
          break;
        case MOUSE_UP :
          DigiMouse.moveY(-moveamount);
          LEDoff();
          break;
        default :
          moveamount = random(MOUSE_MIN, MOUSE_MAX);
          mousemove = 0;
    }
    mousemove++;
    currtime = millis();
    endtime = currtime + random(MIN_DELAY_TIME, MAX_DELAY_TIME);
  }
  delay(10);
  rollovers = millisRollover();
}
