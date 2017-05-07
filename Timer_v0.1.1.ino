unsigned long time = 0;
unsigned long time2 = 0;
int buttonPin = 2;
int timedtime;
int leave = 0;
int reading;
int counter = 0;
int current_state = LOW;
long stime = 0;
int debounce_count = 10;


void setup() { //runs once at beginning of program
  Serial.begin(19200); //begin communications with computer at 19200 baud
  pinMode(buttonPin, INPUT); //set buttonPin (2) to be an input
}

void loop() { //runs repeatedly after setup
  time = 0;
  time2 = 0; //set time and time2 to 0
  stime = millis(); //set stime (system time) to millis(), milliseconds since program started
  
  //inside this loop, we wait for the button to be pressed and then depressed. Afterwards, we take the current time.
  while (time == 0) {  // do this until time != 0.
    if  (debounce(digitalRead(buttonPin)) == HIGH) { //if button is pressed according to debounce, continue
      while (time == 0) { //do this until time != 0
        if (debounce(digitalRead(buttonPin)) == LOW) { //if button isn't pressed according to debounce, continue
          time = millis(); //time becomes millis()
          Serial.println("Time started"); //prints to computer screen 'time started'
        }
      }
    }
  }
  //this loop is equivalent to the last one, except for that time2 replaces time, and that we don't say 'time started.'
  while (time2 == 0) {
    if (debounce(digitalRead(buttonPin)) == HIGH) {
      while (time2 == 0) {
         if (debounce(digitalRead(buttonPin)) == LOW) {
          time2 = millis();
        }
      }
    }
  }
  long timedtime = time2 - time; //a long integer expressing time2 - time, the difference in milliseconds between the two times, or the amount timed.
  int modtime = timedtime % 60000; //modtime = # of seconds in finished time. We use modulo to chop off the minute-sized bits.
  float displaytime = (modtime / 1000.0); //remember, this is milliseconds. Divide by 1000.0 to get seconds.
  int displaymins = (timedtime / 60000); //1 minute per 60,000 milliseconds. Because this isn't a float, we don't need to worry about the decimal bit.
  Serial.print(displaymins); //print how many minutes
  Serial.print(":"); //print a colon
  if (displaytime < 10) { //if there are less than 10 seconds, print a 0 so time appears as 1:04.201 instead of 1:4.201
     Serial.print('0');
  }
  Serial.println(displaytime,3); //print the seconds rounded to 3 decimal places.
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

