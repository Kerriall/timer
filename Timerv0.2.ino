#include <SoftwareSerial.h>

SoftwareSerial Serial7Segment(7, 5);

int ttime;
int buttonPin = 2;
int stime;
int startstop = 0;
int stime2;
int current_state;
int counter;
int debounce_count = 10;
int dtime;

void setup() {
  Serial.begin(19200);
  Serial.println("Hello World");
  Serial7Segment.begin(9600);
  Serial7Segment.write('v');

}

void loop() {
  ttime = 0;
  Serial7Segment.write('v');
  while (startstop == 0) {
    if (debounce(digitalRead(buttonPin)) == HIGH) {
      while (startstop == 0) {
        if (debounce(digitalRead(buttonPin)) == LOW) {
          startstop = 1;
        }
      }
    }
  }
  while (startstop == 1) {
    if (stime2 != millis()) {
      ttime = ttime + 1;
      dtime = ttime / 100;
      char tempString[10]; 
      sprintf(tempString, "%4d", dtime); 
      Serial7Segment.print(tempString);
      stime2 = millis();
    }
    if (debounce(digitalRead(buttonPin)) == HIGH) {
      startstop = 0;
    }
  }
  delay(2000); //FIXME
}

int debounce(int reading) { //the debouncing function mentioned above. Essentially, this makes sure a button press is that way for 10 milliseconds or more
  //before accepting it. See my written bit about why this exists for more details.
  if (stime != millis()) { //if stime isn't equivalent to millis, run this
    reading = digitalRead(buttonPin); //read the button
    if (reading == current_state and counter > 0) { //if the reading is what it already is, take the counter down
      counter--;
    }
    if (reading != current_state) { //if the reading isn't what it already is, take the counter up
      counter++;
    }
    if (counter >= debounce_count) { //if the counter becomes more than 10, or if a state change has happened for more than 10 cycles:
      counter = 0; //reset the counter
      current_state = reading; //switch the official output
      return current_state; //return the official output
    }
    stime = millis(); //in any case, stime becomes millis. This is so that it can only run once per millisecond -- computers go fast!
  }
}
