/*
  STEP 2 - MOTOR TEST
  -------------------
  Spins each motor one at a time so you can see which wheel is which and
  which way it turns.

  How to use:
  1. LIFT THE ROBOT so the wheels don't touch anything.
  2. Battery ON (motors need the battery, USB alone is not enough).
  3. Upload this sketch, open Serial Monitor at 9600 baud.
  4. Watch and write down:
       - "MOTOR A" moves the LEFT or RIGHT wheel?
       - Does MOTOR A spin FORWARD (the robot would drive forward) or backward?
       - Does MOTOR B spin FORWARD or backward?
  5. Put your answers in the SETTINGS of step3_line_follower.

  If a wheel does not move at all: check battery, check that Arduino GND and
  L298N GND are connected, and check the ENA/ENB jumpers are removed.
*/

#define ENA 9
#define IN1 8
#define IN2 7
#define ENB 10
#define IN3 12
#define IN4 13

const int TEST_SPEED = 150;

void driveMotor(int en, int inX, int inY, int speed);
void stopAll();

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopAll();
  Serial.begin(9600);
  Serial.println(F("Motor test starts in 3 seconds. Lift the robot!"));
  delay(3000);
}

void loop() {
  Serial.println(F("MOTOR A (ENA, IN1, IN2) -> should spin FORWARD"));
  driveMotor(ENA, IN1, IN2, TEST_SPEED);
  delay(2000);
  stopAll();
  delay(1500);

  Serial.println(F("MOTOR B (ENB, IN3, IN4) -> should spin FORWARD"));
  driveMotor(ENB, IN3, IN4, TEST_SPEED);
  delay(2000);
  stopAll();
  delay(1500);

  Serial.println(F("BOTH motors FORWARD"));
  driveMotor(ENA, IN1, IN2, TEST_SPEED);
  driveMotor(ENB, IN3, IN4, TEST_SPEED);
  delay(2000);
  stopAll();
  Serial.println(F("--- repeating in 3 seconds ---"));
  delay(3000);
}

// speed: positive = "forward" (same as the original code), negative = backward
void driveMotor(int en, int inX, int inY, int speed) {
  speed = constrain(speed, -255, 255);
  if (speed >= 0) {
    digitalWrite(inX, LOW);
    digitalWrite(inY, HIGH);
  } else {
    digitalWrite(inX, HIGH);
    digitalWrite(inY, LOW);
    speed = -speed;
  }
  analogWrite(en, speed);
}

void stopAll() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
