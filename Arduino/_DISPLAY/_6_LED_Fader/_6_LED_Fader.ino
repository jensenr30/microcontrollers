// defines the LEDs /////////////////////
#define led0 11
#define led1 10
#define led2 9
#define led3 6
#define led4 5
#define led5 3

#define time 100

//int brightness[] = {255, 100, 50, 5, 1, 0, 1, 5, 50, 100, 255, 100, 50, 5, 1, 0};
int i = 0;


void setup() {
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  
  digitalWrite(led0, 0);
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);
  digitalWrite(led3, 0);
  digitalWrite(led4, 0);
  digitalWrite(led5, 0);
}
void loop() {
  digitalWrite(led0, 1);
  delay(time);
  digitalWrite(led5, 0);
  digitalWrite(led0, 0);
  digitalWrite(led1, 1);
  delay(time);
  digitalWrite(led1, 0);
  digitalWrite(led2, 1);
  delay(time);
  digitalWrite(led2, 0);
  digitalWrite(led3, 1);
  delay(time);
  digitalWrite(led3, 0);
  digitalWrite(led4, 1);
  delay(time);
  digitalWrite(led4, 0);
  digitalWrite(led5, 1);
}
