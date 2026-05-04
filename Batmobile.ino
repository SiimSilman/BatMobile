// Ultrasonic Sensor (USS)
#define trigR A0
#define echoR A1
#define trigL A2 
#define echoL A3 

// H-Bridge (MOTOR / DC-MOTOR)
#define motorApin1 5
#define motorApin2 4
#define motorBpin1 3
#define motorBpin2 2
#define ENA 11
#define ENB 6

// Servo (SERVO)
#include <Servo.h>
#define servoPin 9
Servo myServo;

// Sensor.ino global-vars
float distRaw[2];           // Raw distance [0=left 1=right]
float distFilt[2];          // Filtered distance [0=left 1=right]

// Control.ino global-vars
int           ctrl[2];      // Controller values [0=servo 1=motor]    
unsigned long lastTime;     // Last measured controller time

// Actuator.ino global-vars
int           PWM;          // Motor variable PWM
int           newPos;       // Servo variable position

// Time measurement global-vars
int           thisLoop = 0; // Time for main loop
unsigned long lastLoop = 0; // Time for previous main loop

void setup() {
  // TIME-SETUP
  Serial.begin(9600);
  lastTime = millis();

  // USS-SETUP
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);

  // MOTOR-SETUP
  pinMode(motorApin1, OUTPUT);
  pinMode(motorApin2, OUTPUT);
  pinMode(motorBpin1, OUTPUT);
  pinMode(motorBpin2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // SERVO-SETUP
  myServo.attach(servoPin);
  initServo();                  // Initiate starting position (Actuator.ino)
}

void loop() {
  unsigned long now = millis(); // Measure current run time
  thisLoop = now - lastLoop;    // Time difference from last loop
  lastLoop = now;               // Save current-run-time for next iteration

  getDistance();                // Get raw distance       (Sensor.ino)
  filterDistance();             // Filter raw distance    (Sensor.ino)
  int STOP = emergencyStop();   // Check fail-safe        (Sensor.ino)
  getControl();                 // Get PID/PD controls    (Control.ino)
  servo();                      // Change servo position  (Actuator.ino)
  if (STOP == 0) motor();       // Change motor speed     (Actuator.ino)
  plotData();                   // Send data to port      (Batmobile.ino)
}

void plotData() {
  Serial.print(distRaw[0]);     Serial.print(",");  // 1. Left raw distance
  Serial.print(distRaw[1]);     Serial.print(",");  // 2. Right raw distance
  Serial.print(distFilt[0]);    Serial.print(",");  // 3. Left filtered distance
  Serial.print(distFilt[1]);    Serial.print(",");  // 4. Right filtered distance
  Serial.print(ctrl[0]);        Serial.print(",");  // 5. Servo control
  Serial.print(ctrl[1]);        Serial.print(",");  // 6. Motor control
  Serial.print(newPos);         Serial.print(",");  // 7. New servo position
  Serial.print(PWM);            Serial.print(",");  // 8. New motor pulse width modulation
  Serial.println(thisLoop);                         // 9. Time to run current main loop
}