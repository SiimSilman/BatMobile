const float alpha = 0.15;   // Filter: 1 = aggresive, 0 = passive
int   distMax     = 100;    // Fail-safe: Out-Of-Bounds distance
int   oobMax      = 5;      // Fail-safe: Out-Of-Bounds counter
int   oobCounter[2];        // Out-of-bounds counter [0=left 1=right]

void getDistance() {
  int echoTimeOut = 2*7500; // Time-out 7500us, reflection distance 1.25m

  // LEFT Sensor
  digitalWrite(trigL, LOW);                          // Initiate a LOW-signal
  delayMicroseconds(2);                              // Hold for 2ms
  digitalWrite(trigL, HIGH);                         // Initiate a HIGH-signal (send pulse)
  delayMicroseconds(10);                             // Hold for 10ms
  digitalWrite(trigL, LOW);                          // Initiate a LOW-signal
  long duration = pulseIn(echoL, HIGH, echoTimeOut); // Recieve pulse
  distRaw[0] = duration * 0.034 / 2;                 // Convert pulse-time-delay to distance (m/s)

  // RIGHT Sensor
  digitalWrite(trigR, LOW);
  delayMicroseconds(2);
  digitalWrite(trigR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigR, LOW);
  duration = pulseIn(echoR, HIGH, echoTimeOut); 
  distRaw[1] = duration * 0.034 / 2;
}

void filterDistance() { 
  // Overwrite No-Reflection-Values as distMax
  if (distRaw[0] == 0) distRaw[0] = distMax;
  if (distRaw[1] == 0) distRaw[1] = distMax;

  // Exponential Moving Average (EMA) filter
  distFilt[0] = (alpha * distRaw[0]) + (1.0 - alpha) * distFilt[0];
  distFilt[1] = (alpha * distRaw[1]) + (1.0 - alpha) * distFilt[1];

  // Out-of-Bounds (OoB) counter and counter-reset
  if (distRaw[0] > 0 && distRaw[0] < distMax) oobCounter[0] = 0;
  else oobCounter[0]++;
  if (distRaw[1] > 0 && distRaw[1] < distMax) oobCounter[1] = 0;
  else oobCounter[1]++;
}

int emergencyStop() {
  if (oobCounter[0] >= oobMax && oobCounter[1] >= oobMax) {
    motorOff();
    ctrl[0] = 0;
    ctrl[1] = 0;
    PWM = 0;
    return 1;
  }
  else return 0;
}