// --- Servo controller variables ---
float       KpServo;        // P-gain, dynamic (0.75, 10)
const float KdServo = 0.5;  // D-gain

// --- DC-motor controller variables ---
const float KpMotor = 5;    // P-gain
const float KiMotor = 1;    // I-gain
const float KdMotor = 1;    // D-gain

// --- Variables & Settings --- 
float       saveAngleError;     // Last angle error
float       saveMotorError;     // Last motor error
float       intMotorError;      // Accumulate motor error
const int   intLimit = 50;      // Anti-Windup for motor (Limit I-part)
const int   distRef = 30.0;     // Reference distance

// PD & PID CONTROLLER
void getControl() {
  // --- Discretize time --- 
  unsigned long currentTime = millis();                         // Time
  float dt = (float)(currentTime - lastTime) / 1000.0;          // Time derivative
  if (dt <= 0) dt = 0.1;                                       // Safety marginal

  // --- Servo controller (PD) --- 
  float AngleError  = distFilt[0] - distFilt[1];                // Servo P-error
  float dAngleError = (AngleError - saveAngleError) / dt;       // Servo D-error

  float rawKp = 0.05 + 6.9 / (abs(AngleError)/5 + 1.0);
  // KpServo = constrain(rawKp, 0.5, 3); 
  // if (abs(AngleError) < 10) KpServo = 7;
  // if (abs(AngleError) > 10) KpServo = 0.275;
  ctrl[0] = int((rawKp * AngleError) + (KdServo * dAngleError));

  // --- Motor controller (PID) --- 
  float MotorError  = distRef - min(distFilt[0], distFilt[1]);  // Motor P-error
  intMotorError += MotorError * dt;                             // Motor I-error
  float dMotorError = (MotorError - saveMotorError) / dt;       // Motor D-error
  if (intMotorError > intLimit) intMotorError = intLimit;       // Limit I-gain forward
  if (intMotorError < -intLimit) intMotorError = -intLimit;     // Limit I-gain backward
  ctrl[1] = int((KpMotor*MotorError) + (KiMotor*intMotorError) + (KdMotor*dMotorError));

  // --- Save data --- 
  saveAngleError = AngleError;
  saveMotorError = MotorError;
  lastTime = currentTime;
}