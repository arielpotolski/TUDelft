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

// IR sensors pins (orientation by looking from the back)
const uint32_t RSP = PA0; // IR sensor on the same side as motorA
const uint32_t LSP = PA1; // IR sensor on the same side as motorB

// Speed encoders pins
const uint32_t LEP = PB13;  // Speed encoder on the same side as motorB
const uint32_t REP = PB12;// Speed encoder on the same side as motorA

// threashold between white and black
uint32_t thre = 60;

//current encoder counts
volatile int leftCounter = 0;
volatile int rightCounter = 0;

// distance to run in centimeters
const float distance = 180;

const float wheelCirc = 21.0;// Wheel circumference in centimeters
const int diskslots = 20; //number of slots on the slotted wheel

void setup() {
  // Motor pins as outputs
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  // Ir pins as inputs
  pinMode(RSP, INPUT);
  pinMode(LSP, INPUT);

  // speed encoder pins as inputs
  pinMode(LEP, INPUT);
  pinMode(REP, INPUT);

  // attach interrupts
  attachInterrupt(REP, rightEncoderISR, RISING);
  attachInterrupt(LEP, leftEncoderISR, RISING);
}

// interrupt that counts the number of turns for the right side encoder
void rightEncoderISR() {
  rightCounter++;
}

// interrupt that counts the number of turns for the left side encoder
void leftEncoderISR() {
  leftCounter++;
}

/**
* Function that converts a distance in centimeters to counts.
* Returns the number of counts that the wheels should achieve to travel the target distance.
*/
int getDistanceInCounts (){
  int counts;

  // calculation to get the number of counts
  counts = (distance * 4965) / (wheelCirc * 100);  // 4965 is the CPR of the encoder.
  return counts;
}

void loop() {
  int LSA = analogRead(LSP);  // read the values received by the left sensor
  int RSA = analogRead(RSP);  // read the values received by the right sensor

  // read speed encoders
  digitalRead(REP);
  digitalRead(LEP);

  if (leftCounter >= getDistanceInCounts() && rightCounter >= getDistanceInCounts()) {  // if robot has travelled the target distance, we stop
    analogWrite(motorA1, 0);
    analogWrite(motorA2, 0);  
    analogWrite(motorB1, 0);
    analogWrite(motorB2, 0);
  } else {
    // detach the interrupt. This helps in calculating the distance more precisely
    detachInterrupt(digitalPinToInterrupt(REP));
    detachInterrupt(digitalPinToInterrupt(LEP));

    if (LSA <= thre && RSA <= thre) {  // both are on white
      // straight 
      analogWrite(motorA1, 185);
      analogWrite(motorA2, 0);  
      analogWrite(motorB1, 165);
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
      analogWrite(motorA2, 160);  
      analogWrite(motorB1, 185);
      analogWrite(motorB2, 0);
      delayMicroseconds(2);
    }

    // attach interrupts again
    attachInterrupt(digitalPinToInterrupt(REP), rightEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(LEP), leftEncoderISR, CHANGE);
  }
}
