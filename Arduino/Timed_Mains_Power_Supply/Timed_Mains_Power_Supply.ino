/*
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

#define debug false // debugging console message
#define debugTimer false // debugging counting things
 
 
// these is the column positions of the "^^" cursor 
#define timerslot_seconds 14
#define timerslot_minutes 11
#define timerslot_hours   8
#define timerslot_days    5


#define m_seconds ((unsigned long int)1000)
#define m_minutes ((unsigned long int)1000*60)
#define m_hours   ((unsigned long int)1000*60*60)
#define m_days    ((unsigned long int)1000*60*60*24)

#define MAX_TIME ((unsigned long int)1000*60*60*24*31) // a month

//active low INPUT
#define pin_countup   7 // this is a button that, when pushed, will counts up
//active low INPUT
#define pin_countdown 6 // this is the button that counts down
// active on falling edge INPUT
#define pin_startstop 8 // this pin toggles the start/stop state.
//active high OUTPUT
#define pin_control   9 // this is the output to the transistor that controls the current flowing through the control relay's coil




//long int timer; // this is how much time the timer has (in seconds)
short int timerSlot = timerslot_minutes; // this holds which time slot the user has the cursor in.
unsigned long int timerEnd = 0; // this is the time in ms (relative to the beginning of the arduino sketch) when the timer will end.
short int counting = 0; // this stores if the timer is currently counting down. 1 = counting down.
//buttons keeps track of user input
// buttons[0][] is the UP button
// buttons[1][] is the DOWN button
// buttons[2][] is the START/STOP button
// buttons[][0] is a button's current state
// buttons[][1] is a button's last state
int buttons[3][2];




// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);





//this adjusts the timerEnd time to be absolute.
//this starting the timer counting down.
void timer_start(){
  timerEnd += millis();
  counting = 1;
}
//this adjusts the timerEnd time to be relative
//this stops the counter; the counter is paused.
void timer_stop(){
  timerEnd -=millis();
  counting = 0;
}

//this function resets the counter to 0 and turns off the counter.
void timer_reset(){
  counting = 0;
  timerEnd = (unsigned long int)30*m_minutes;
}

//this returns how much time there is left in the timer.
long unsigned int time_left(){
  if(counting){
    return timerEnd - millis();
  }
  else{
    return timerEnd;
  }
}



void timer_up(int steps){
  long unsigned int addedTime;
  
       if(timerSlot == timerslot_seconds) addedTime = ((unsigned long int)m_seconds*steps);
  else if(timerSlot == timerslot_minutes) addedTime = ((unsigned long int)m_minutes*steps);
  else if(timerSlot == timerslot_hours) addedTime   = ((unsigned long int)m_hours*steps);
  else if(timerSlot == timerslot_days) addedTime    = ((unsigned long int)m_days*steps);
  
  //only add time if it will not go above MAX_TIME and also only add it if it will not cause an overflow.
  if(addedTime + timerEnd > timerEnd && addedTime + time_left() < MAX_TIME) timerEnd += addedTime;
}

void timer_down(int steps){
  long unsigned int removedTime;
  
       if(timerSlot == timerslot_seconds) removedTime = ((unsigned long int)m_seconds*steps);
  else if(timerSlot == timerslot_minutes) removedTime = ((unsigned long int)m_minutes*steps);
  else if(timerSlot == timerslot_hours) removedTime   = ((unsigned long int)m_hours*steps);
  else if(timerSlot == timerslot_days) removedTime    = ((unsigned long int)m_days*steps);
  
  //only add time if it will not "underflow" 
  if(timerEnd - removedTime < timerEnd && time_left() > 0) timerEnd -= removedTime;
}



void switch_timer_slot(){
  if(timerSlot == timerslot_days) timerSlot = timerslot_hours;
  else if(timerSlot == timerslot_hours) timerSlot = timerslot_minutes;
  else if(timerSlot == timerslot_minutes) timerSlot = timerslot_seconds;
  else if(timerSlot == timerslot_seconds) timerSlot = timerslot_days;
}






//this prints the timer information to the LCD
void printTimer(){
  
  //this is the current time (as of the beginning of the program.
  //use this instead of calling millis() each time.
  //that makes sure there are no inconsistancies.
  unsigned long int currentTime = millis();
  //this is the time left. this is the time that gets printed to the LCD
  unsigned long int timeLeft;
  
  //get the absolute value of the difference between the two times.
  if(timerEnd > currentTime)
    timeLeft = timerEnd - currentTime;
  else
    timeLeft = 0;
    //if the timer is not on, then timerEnd represents the time left on the timer (relative. not absolute)
  if(counting == 0) timeLeft = timerEnd;
  
  
  //calculate each component of the time.
  unsigned long int timeDays    = timeLeft/((unsigned long int)1000*60*60*24);
  unsigned long int timeHours   = timeLeft/((unsigned long int)1000*60*60)   - timeDays*24;
  unsigned long int timeMinutes = timeLeft/((unsigned long int)1000*60)      - timeDays*24*60    - timeHours*60;
  unsigned long int timeSeconds = timeLeft/((unsigned long int)1000)         - timeDays*24*60*60 - timeHours*60*60 - timeMinutes*60;
  
  #if(debug)
    //this handles printing information at regular intervals
    static long unsigned int lastInfoPrint = 0;
    
    if(currentTime - lastInfoPrint > m_seconds){
      Serial.print("currentTime = ");
      Serial.println(currentTime);
      Serial.print("timerEnd = ");
      Serial.println(timerEnd);
      Serial.print("timeLeft = ");
      Serial.println(timeLeft);
      
      Serial.print("timeDays = ");
      Serial.println(timeDays);
      Serial.print("timeHours = ");
      Serial.println(timeHours);
      Serial.print("timeMinutes = ");
      Serial.println(timeMinutes);
      Serial.print("timeSeconds = ");
      Serial.print(timeSeconds);
      Serial.print("\n\n");//newline
      //out with the old, in with the current.
      lastInfoPrint = currentTime;
    }
  #endif
  
  char printColon;
  if(counting && timeLeft%1000 < 500) // if you are halfway through a second
    printColon = ' '; // no colon
  else
    printColon = ':'; //colon
  
  //print out time left
  lcd.setCursor(0,0); // reset cursor position
  lcd.print("t =  ");
  if(timeDays < 10) lcd.print("0");
  lcd.print(timeDays);
  lcd.print(printColon);
  if(timeHours < 10) lcd.print("0");
  lcd.print(timeHours);   
  lcd.print(printColon);
  if(timeMinutes < 10) lcd.print("0");
  lcd.print(timeMinutes);
  lcd.print(printColon);
  if(timeSeconds < 10) lcd.print("0");
  lcd.print(timeSeconds);
  
  //print cursor
  if(timerSlot >= 0 && timerSlot <= 14){
    lcd.setCursor(0,1);//get cursor on bottom line
    if(timerSlot == timerslot_days){
      lcd.print("     ^^         ");
    }
    else if(timerSlot == timerslot_hours){
      lcd.print("        ^^      ");
    }
    else if(timerSlot == timerslot_minutes){
      lcd.print("           ^^   ");
    }
    else if(timerSlot == timerslot_seconds){
      lcd.print("              ^^");
    }
  }
}




void setup(){
  pinMode(pin_countup, INPUT); // inputs
  pinMode(pin_countdown, INPUT);
  pinMode(pin_startstop, INPUT);
  digitalWrite(pin_countup, HIGH); // pullup resistors
  digitalWrite(pin_countdown, HIGH);
  digitalWrite(pin_startstop, HIGH);
  
  pinMode(pin_control, OUTPUT); //outputs
  
  #if(debug||debugTimer)
    Serial.begin(9600);//setup serial monitor for debugging if need be.
  #endif
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // set timer to 30 minutes by default.
  timer_reset();
  printTimer();
  // Turn on the display:
  lcd.display();
  //pause the counter.
  counting = 0;
  
  //reset buttons
  int i,j;
  for(j=0; j<3; j++){
    for(i=0; i<2; i++){
      buttons[j][i] = false;
    }
  }
}




void loop() {
  
  //read the button states in to memory.
  buttons[0][0] = digitalRead(pin_countup);
  buttons[1][0] = digitalRead(pin_countdown);
  buttons[2][0] = digitalRead(pin_startstop);
  
  //increase the counter
  if(buttons[0][0] == LOW && buttons[0][1] == HIGH){ // falling edge trigger and the timer is valid
    timer_up(1);
    #if(debugTimer)
      Serial.println("Time++/\\");
    #endif
  }
  //decrease the counter
  if(buttons[1][0] == LOW && buttons[1][1] == HIGH){ // falling edge trigger and the timer is valid
    timer_down(1);
    #if(debugTimer)
      Serial.println("Time--\\/");
    #endif
  }
  
  //if the user starts/stops the count, start/stop the count.
  if(buttons[2][0] == LOW && buttons[2][1] == HIGH){
    if(counting == 1){
      digitalWrite(pin_control, LOW); // disengage relay
      timer_stop();
    }
    else if(time_left() > 0 && time_left() < MAX_TIME){
      timer_start();
      digitalWrite(pin_control, HIGH);
    }
    else{
      timer_reset(); // reset the timer if junk's messed up.
    }
    #if(debugTimer)
      if(counting == 1)Serial.println("Relay engaged.\nTimer counting down...\n");
      else Serial.println("Relay disengaged.\n");
    #endif
  }
  
  //this handles switching timer slots. you have to press both up and down to move timer slots.
  if(buttons[0][0] == LOW && buttons[1][0] == LOW && buttons[1][1] == HIGH)
    switch_timer_slot();
  else if(buttons[0][0] == LOW && buttons[1][0] == LOW && buttons[0][1] == HIGH)
    switch_timer_slot();
  
  
  //print the timer information
  printTimer();
  
  
  
  //if we are not in the right time area...
  if(time_left() >= MAX_TIME || time_left() <= 0){
    digitalWrite(pin_control, LOW); // disengage relay
    counting = false; // stop counting.
    #if(debugTimer)
      Serial.println("Relay disengaged: Timer ran out.\n");
    #endif
    timer_reset(); // reset timer.
  }
  
  
  //push current button values into the previous slot. the new states become the old states.
  int j;
  for(j=0; j<3; j++){
    buttons[j][1] = buttons[j][0];
  }
  
  //wait for a few milliseconds
  delay(20);
}

