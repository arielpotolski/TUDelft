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

// infrared sensor pins (orientation by looking from the back)
const uint32_t RSP = PA0; // sensor on the same side as motorA
const uint32_t LSP = PA1; // sensor on the same side as motorB

// Ultrasonic sensor pins
const uint32_t EPF = PB9; // echoPin front sensor
const uint32_t TPF = PA10; // trigPin front sensor

//Gobal variables
uint32_t distanceFront; // distance from front sensor to the object
uint32_t durationFront;  // How long does it take from the time the sensor emits the wave to the time it receives the wave back

// threashold between white and black (less or equal is white, more is black)
const uint32_t thre = 50;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(RSP, INPUT);
  pinMode(LSP, INPUT);
  pinMode(EPF, INPUT);
  pinMode(TPF, OUTPUT);
}

void loop() {
  digitalWrite(TPF, LOW); // clear the front ultrasonic sensor trigPin
  // trigger the front ultrasonic sensor
  delayMicroseconds(10);
  digitalWrite(TPF, HIGH);
  delayMicroseconds(10);
  digitalWrite(TPF, LOW);

  durationFront = pulseIn(EPF, HIGH);  // time passed between emitting and receiving the pulse
  distanceFront = durationFront * 0.034 / 2;  // equation to calculate the distance to the object

  int LSA = analogRead(LSP);  // read the values received by the left sensor
  int RSA = analogRead(RSP);  // read the values received by the right sensor

  if (distanceFront < 15) {  // then robot needs to avoid the object
    goAround();
  }
  
  if (LSA <= thre && RSA <= thre) {  // both are on white
    // straight 
    analogWrite(motorA1, 180);
    analogWrite(motorA2, 0);  
    analogWrite(motorB1, 160);
    analogWrite(motorB2, 0);
    delayMicroseconds(2);
  } else if (LSA > thre && RSA <= thre) { // left sensor is on the line
    // adjust to the left
    analogWrite(motorA1, 180);
    analogWrite(motorA2, 0);  
    analogWrite(motorB1, 0);
    analogWrite(motorB2, 160);
    delayMicroseconds(2);
  } else if (RSA > thre && LSA <= thre) { // right sensor is on the line
    //adjust to the right
    analogWrite(motorA1, 0);
    analogWrite(motorA2, 165);  
    analogWrite(motorB1, 170);
    analogWrite(motorB2, 0);
    delayMicroseconds(2);
  }
}

/**
* Function that handles the object avoidance, once the object is detected.
*/
void goAround() {
  stop();
  delay(100);
  
  turnLeft();
  delay(450);

  goStraight();
  delay(1200);

  turnRight();
  delay(590);

  goStraight();
  delay(1950);

  turnRight();
  delay(420);

  goStraight();
  delay(300);
}

void turnLeft() {
  analogWrite(motorA1, 185);  
  analogWrite(motorA2, 0);  
  analogWrite(motorB1, 0);
  analogWrite(motorB2, 160);
}

void goStraight() {
  analogWrite(motorA1, 185);
  analogWrite(motorA2, 0);  
  analogWrite(motorB1, 165);
  analogWrite(motorB2, 0);
}

void turnRight() {
  analogWrite(motorA1, 0);
  analogWrite(motorA2, 160);  
  analogWrite(motorB1, 185);
  analogWrite(motorB2, 0);
}

void stop() {
  analogWrite(motorA1, 0);
  analogWrite(motorA2, 0);  
  analogWrite(motorB1, 0);
  analogWrite(motorB2, 0);
}
