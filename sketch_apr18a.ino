#include <IRremote.h>
#include <Servo.h>

const int irPin = 12;
const int servoPin = 9;
const int servoPin2 = 10;
const int touchPin = 11;   
const unsigned long openCode = 0xFF52AD;
const unsigned long closeCode = 0xFF4AB5;   

// Define each segment pin, excluding 'f'
int segA = 2;
int segB = 3;
int segC = 4; 
int segD = 5; 
int segE = 8;
int segG = 13;

Servo Servo1, Servo2;              // Create servo objects
IRrecv irrecv(irPin);              // Create instance of IRrecv
decode_results results;            // Create instance of decode_results
int attemptsLeft = 3;              // Counter for incorrect button presses

bool gateOpen = false;         // Gate state variable
bool lastTouchState = LOW;

void setup() {
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segG, OUTPUT);

  Servo1.attach(servoPin);
  Servo2.attach(servoPin2);
  irrecv.enableIRIn();
  pinMode(touchPin, INPUT);
  Serial.begin(9600);        
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Received IR code: "); // Debugging line to see what's received
    Serial.println(results.value, HEX);

    if (results.value == openCode) {
      Servo1.write(180); 
      clearDisplay();      // Open gate
      Serial.println("Gate opened via remote");
       delay(5000);                    // Wait for 5 seconds
      Servo1.write(90);        
      
      // No changes to attempts or display here
    } else {
      // Handling all other IR codes
      if (attemptsLeft > 0) {
        attemptsLeft--; // Decrement attempts only for other codes
        updateDisplay(attemptsLeft); // Update display based on remaining attempts
      }
      if (attemptsLeft <= 0) {
        attemptsLeft = 3; // Reset attempts after reaching zero
        clearDisplay(); // Clear the display after third incorrect press
      }
    }
    irrecv.resume(); // Receive the next value
  }

  bool currentTouchState = digitalRead(touchPin);
  if (currentTouchState == HIGH && lastTouchState == LOW) {
    gateOpen = !gateOpen;
    if (gateOpen) {
      Servo2.write(90);
      Serial.println("Gate manually opened");
    } else {
      Servo2.write(0);
      Serial.println("Gate manually closed");
    }
    delay(500); // Debounce delay
  }
  lastTouchState = currentTouchState;
}



void updateDisplay(int number) {
  // Assume segments are wired directly to control each number uniquely
  // Implementations should turn off all segments, then turn on only the needed ones
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segG, LOW);

  switch (number) {
    case 3:
      digitalWrite(segA, HIGH);
      digitalWrite(segB, HIGH);
      digitalWrite(segC, LOW);
      digitalWrite(segD, HIGH);
      digitalWrite(segE, HIGH);
      digitalWrite(segG, HIGH);
      break;
    case 2:
      digitalWrite(segA, HIGH);
      digitalWrite(segB, HIGH);
      digitalWrite(segC, HIGH);
      digitalWrite(segD, HIGH);
      digitalWrite(segE, LOW);
      digitalWrite(segG, HIGH);
      break;
    case 1:
      digitalWrite(segA, LOW);
      digitalWrite(segB, HIGH);
      digitalWrite(segC, LOW);
      digitalWrite(segD, LOW);
      digitalWrite(segE, HIGH);
      digitalWrite(segG, LOW);
      break;
    delay(1000); // Delay for 1 second to display "1" before turning off
    clearDisplay(); 
  }
}



void clearDisplay() {
  // Turns off all segments
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segG, LOW);
}