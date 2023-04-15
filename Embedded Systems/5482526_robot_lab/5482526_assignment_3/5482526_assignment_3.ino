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

// sensor pins (orientation by looking from the back)
const uint32_t RSP = PA0; // sensor on the same side as motorA
const uint32_t LSP = PA1; // sensor on the same side as motorB

// threashold between white and black
uint32_t thre = 50;  // less than or equal to this is white, more is black

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(RSP, INPUT);
  pinMode(LSP, INPUT);
}

void loop() {
  int LSA = analogRead(LSP);  // read the values received by the left sensor
  int RSA = analogRead(RSP);  // read the values received by the right sensor

  if (LSA <= thre && RSA <= thre) {  // both are on white
    // straight 
    analogWrite(motorA1, 185);
    analogWrite(motorA2, 0);  
    analogWrite(motorB1, 165);
    analogWrite(motorB2, 0);
    delayMicroseconds(2);
  } else if (LSA > thre && RSA <= thre) { // left sensor is on the line
    // adjust to the left
    analogWrite(motorA1, 185);
    analogWrite(motorA2, 0);  
    analogWrite(motorB1, 0);
    analogWrite(motorB2, 165);
    delayMicroseconds(2);
  } else if (RSA > thre && LSA <= thre) { // right sensor is on the line
    //adjust to the right
    analogWrite(motorA1, 0);
    analogWrite(motorA2, 165);  
    analogWrite(motorB1, 175);
    analogWrite(motorB2, 0);
    delayMicroseconds(2);
  }
}
