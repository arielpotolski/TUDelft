/**
* Student name: Ariel Potolski Eilat
* Student number: 5482526
*
*/

// Motor pins
const uint32_t motorA1 = PA8; //move front
const uint32_t motorA2 = PB3; //move back
const uint32_t motorB1 = PB15; //move front
const uint32_t motorB2 = PB14; //move back

// Ultrasonic sensor pins
const uint32_t echoPin = PB9; 
const uint32_t trigPin = PA10;

//Gobal variables
uint32_t distance; // distance from the object
uint32_t duration; // timeF - timeI
uint32_t timeI; // time when the signal was sent from trigger
uint32_t timeF; // time when the signal is received at echo

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  attachInterrupt(echoPin, echoISR, CHANGE);  // attach interrupt to echoPin, 
  //which is called when the signal of the pin changes
}

void echoISR() {
  switch (digitalRead(echoPin)) {
    case HIGH:  // start of the echo pulse
      timeF = 0;  // clear the end time
      timeI = micros();  // get starting time
      break;
    
    case LOW:  // end of the echo pulse
      timeF = micros();  // get end time
      duration = timeF - timeI;  // time between emitting the pulse and receiving it
      distance = duration * 0.034 / 2;  // calculation of the distance to the object (in cm)
      break;
  }
}

void loop() {
  digitalWrite(trigPin, LOW); // clear the trigPin

  // Start robot
  analogWrite(motorA1, 185);
  analogWrite(motorA2, 0);  
  analogWrite(motorB1, 165);
  analogWrite(motorB2, 0);

  // trigger the ultrasonic sensor
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  if (distance < 15) {  // if distance to object < 15 cm, stop motors
    analogWrite(motorA1, 0);  
    analogWrite(motorA2, 0);  
    analogWrite(motorB1, 0);
    analogWrite(motorB2, 0);
    delay(200);
  }

}
