#include <Servo.h>

Servo servoScan;
Servo servoCam;
Servo servoAction;

#define TRIG 2
#define ECHO 3
#define IR_LEFT  A0
#define IR_RIGHT A1

#define SERVO_SCAN_PIN   5
#define SERVO_CAM_PIN    6
#define SERVO_ACTION_PIN 7

int distanceThreshold = 25; // in cm, adjust based on your setup

long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); // timeout after 30ms
  if (duration == 0) return -1; // nothing detected or out of range

  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  servoScan.attach(SERVO_SCAN_PIN);
  servoCam.attach(SERVO_CAM_PIN);
  servoAction.attach(SERVO_ACTION_PIN);

  // start everything at neutral position
  servoScan.write(90);
  servoCam.write(90);
  servoAction.write(0);
}

void loop() {
  int detectedAngle = -1;

  // sweep from 20 to 160 degrees, step 5
  for (int angle = 20; angle <= 160; angle += 5) {
    servoScan.write(angle);
    delay(60); // give servo time to reach position

    long d = getDistance();
    int irL = digitalRead(IR_LEFT);
    int irR = digitalRead(IR_RIGHT);

    // send angle and distance to Python for radar display
    Serial.print(angle);
    Serial.print(",");
    Serial.println(d);

    // object too close, or both IR sensors triggered
    if ((d > 0 && d < distanceThreshold) || (irL == LOW && irR == LOW)) {
      detectedAngle = angle;
      break;
    }
  }

  if (detectedAngle != -1) {
    int safeAngle = constrain(detectedAngle, 0, 180);

    servoCam.write(safeAngle); // point camera toward object
    delay(300);

    servoAction.write(90); // trigger action servo
    delay(800);
    servoAction.write(0); // reset
  }
}