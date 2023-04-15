/**
* Student name: Ariel Potolski Eilat
* Student number: 5482526
*
*/

const uint32_t motorA1 = PA8;
const uint32_t motorA2 = PB3;

const uint32_t motorB1 = PB15;
const uint32_t motorB2 = PB14;

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  motor();
}

void motor() {
  /*IMPORTANT
  * The diferences in speed between the two motors is to compensate for the diference between their spinning speed.
  * Motor B spins faster than motor A. Therefore, we need to compensate that diference in the code, so that the robot
  * can go in a straight line. This is also applied in the other tasks of the lab.
  */
  analogWrite(motorA1, 190);  // start with 55% of the speed
  analogWrite(motorA2, 0);  
  analogWrite(motorB1, 165);
  analogWrite(motorB2, 0);  
  delay(3500);

  analogWrite(motorA1, 235); // increase to around 80% of the speed
  analogWrite(motorA2, 0);
  analogWrite(motorB1, 205);
  analogWrite(motorB2, 0);
  delay(3500);

  analogWrite(motorA1, 150);  // start slowly decreasing the speed
  analogWrite(motorA2, 0);
  analogWrite(motorB1, 150);
  analogWrite(motorB2, 0);
  delay(800);

  analogWrite(motorA1, 100);
  analogWrite(motorA2, 0);
  analogWrite(motorB1, 100);
  analogWrite(motorB2, 0);
  delay(800);

  analogWrite(motorA1, 50);
  analogWrite(motorA2, 0);
  analogWrite(motorB1, 50);
  analogWrite(motorB2, 0);
  delay(800);

  analogWrite(motorA1, 0); // turn motors off (completely stop)
  analogWrite(motorB1, 0);
  analogWrite(motorA2, 0); // turn motors off (completely stop)
  analogWrite(motorB2, 0);
}

void loop() {
  
}
