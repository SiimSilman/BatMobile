const int centPos = 85;               // (Servo) Center position
const int chgPos  = 45;               // (Servo) Max deviation from center position
int       minPos = centPos - chgPos;  // (Servo) Max position to left
int       maxPos = centPos + chgPos;  // (Servo) Max position to right

void servo() {
  newPos = constrain((centPos + ctrl[0]), minPos, maxPos); // End position
  myServo.write(newPos);
}

void initServo() {
  newPos = centPos;
  myServo.write(newPos);
  delay(500);
}

void motor() {
  PWM = -constrain(ctrl[1], -255, 255);

  if (abs(PWM) <= 85) PWM = 0;

  if (PWM >= 0) {  
    motorForward();
    analogWrite(ENA, PWM);
    analogWrite(ENB, PWM);
  }
  if (PWM < 0) {
    motorBackward();
    analogWrite(ENA, abs(PWM));
    analogWrite(ENB, abs(PWM));
  }
}

void motorForward() {
  digitalWrite(motorApin1, LOW);
  digitalWrite(motorApin2, HIGH); 
  digitalWrite(motorBpin1, LOW);
  digitalWrite(motorBpin2, HIGH); 
}

void motorBackward() {
  digitalWrite(motorApin1, HIGH);
  digitalWrite(motorApin2, LOW); 
  digitalWrite(motorBpin1, HIGH);
  digitalWrite(motorBpin2, LOW); 
}

void motorOff() {
  digitalWrite(motorApin1, LOW);
  digitalWrite(motorApin2, LOW); 
  digitalWrite(motorBpin1, LOW);
  digitalWrite(motorBpin2, LOW); 
}
