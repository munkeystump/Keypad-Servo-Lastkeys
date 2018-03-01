#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip  //tells to use keypad library
#include <Servo.h> //use servo library


Servo myservo; //declares servo
const int passSize = 4;
char password[passSize] = {'1','5','3','*'}; //password to unlock, can be changed
char currentPass[passSize] = {'x','x','x','x'};
int amber = 11;
int green = 13;
int servoPin=10;

const byte ROWS = 4; // Keypad rows
const byte COLS = 3; // Keypad columns

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 9, 8, 7, 6 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 5, 4, 3 };


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  pinMode(green, OUTPUT);  
  pinMode(amber, OUTPUT);
  myservo.attach(servoPin); //initialize servo
  keypad.addEventListener(keypadEvent); //add an event listener for keypad
}

void loop() {
  digitalWrite(amber, HIGH);//turn on amber
  keypad.getKey();
  myservo.write(90);        //makes sure servo is always at 90 degrees
  if(checkPassword()){
    unlock();
  }
}
//take care of some special events
void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:
      appendKey(eKey); digitalWrite(amber, LOW); delay(50); digitalWrite(amber, HIGH); //blinks amber led on keypress
  }
}

//keeps a track of the last (passSize) keys pressed
void appendKey(char k){
  for(byte i=1; i<passSize;i++){
    currentPass[i-1]=currentPass[i];
  }
  currentPass[passSize-1]=k;
}

//Checks to see if password has been entered
bool checkPassword() {
  for(byte i=0;i<passSize;i++){
    if(currentPass[i]!=password[i]){
      return false;
    }
  }
  return true;
}

void unlock(){
  Serial.println("Accepted");
  myservo.write(130); //Rotate servo deg
  digitalWrite(green, HIGH);//turn on green
  digitalWrite(amber, LOW);// turn off amber
  delay(500); //wait 0.5 seconds
  digitalWrite(green, LOW);// turn off green
  Serial.println("Switching");
  clearPass();
}

//clears last keypresses after correct password entered to prevent multiple triggers
void clearPass(){
  for(byte i=0;i<passSize;i++){
    currentPass[i]='x';
  }
}
