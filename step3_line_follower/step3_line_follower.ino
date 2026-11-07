/*
  STEP 3 - PID LINE FOLLOWER
  --------------------------
  Arduino UNO + L298N + 5-channel TCRT5000 sensor bar + 2 TT motors.

  Start-up:
  1. Put the robot ON the track with the black line under the MIDDLE sensor.
  2. Turn it on / press reset. It waits 3 seconds, learns which value means
     "black", waits 1 more second, then drives.
  3. If you lift the robot up, it stops (all sensors see "black" in the air).

  Only change things inside the SETTINGS block.
*/

// ======================= SETTINGS =======================

// L298N pins
#define ENA 9
#define IN1 8
#define IN2 7
#define ENB 10
#define IN3 12
#define IN4 13

// Sensor bar OUT1..OUT5 wires.
// If they are on A0..A4, change to: {A0, A1, A2, A3, A4}
const int SENSOR_PIN[5] = {2, 3, 4, 5, 6};

// --- From STEP 2 (motor test) ---
const bool MOTOR_A_IS_LEFT  = true;   // MOTOR A moved the LEFT wheel? true / false
const bool MOTOR_A_REVERSED = false;  // MOTOR A spun BACKWARD? set true
const bool MOTOR_B_REVERSED = false;  // MOTOR B spun BACKWARD? set true

// --- If the robot turns AWAY from the line, set this to true ---
const bool REVERSE_STEERING = false;

// --- Line color: 0 = auto-detect at start (recommended)
//                 1 = black reads HIGH (1)    2 = black reads LOW (0)
const int LINE_MODE = 0;

// --- Speeds (0..255) ---
int baseSpeed   = 130;  // straight-line speed. If motors only hum, raise it.
int maxSpeed    = 200;  // fastest a wheel may go
int minSpeed    = -60;  // negative = inner wheel may go backward on sharp turns
int searchSpeed = 110;  // turning speed when the line is lost

// --- PID ---
float Kp = 15.0;   // how hard it steers toward the line
float Kd = 25.0;   // calms down wobbling
float Ki = 0.0;    // leave at 0

const unsigned long LOOP_MS = 10;   // control loop every 10 ms
const bool DEBUG = false;           // true = print sensors/error to Serial Monitor (9600)

// ===================== END OF SETTINGS =====================

// Sensor weights OUT1..OUT5 (same as the original GitHub code)
const float WEIGHT[5] = {7, 3, 0, -3, -7};

int lineValue = HIGH;         // the digital value that means "black line"
float lastError = 0;
float integral = 0;
unsigned long lastTime = 0;
unsigned long lastPrint = 0;

int readSensors(float &error);
void detectLineColor();
void setMotors(int left, int right);
void driveMotor(int en, int inX, int inY, int speed);
void stopMotors();
void debugPrint(int count, float error, int l, int r);

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  for (int i = 0; i < 5; i++) pinMode(SENSOR_PIN[i], INPUT);
  stopMotors();

  Serial.begin(9600);
  Serial.println(F("Place the robot with the line under the MIDDLE sensor..."));
  delay(3000);
  detectLineColor();
  delay(1000);
  lastTime = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastTime < LOOP_MS) return;
  lastTime = now;

  float error = lastError;
  int count = readSensors(error);

  // All 5 see black -> robot lifted up (or a big black area): stop.
  if (count == 5) {
    stopMotors();
    integral = 0;
    debugPrint(count, error, 0, 0);
    return;
  }

  // No sensor sees the line -> spin toward the side it was last seen.
  if (count == 0) {
    float dir = REVERSE_STEERING ? -lastError : lastError;
    if (dir > 0)      setMotors(searchSpeed, -searchSpeed);
    else if (dir < 0) setMotors(-searchSpeed, searchSpeed);
    else              setMotors(baseSpeed, baseSpeed);
    debugPrint(count, lastError, 0, 0);
    return;
  }

  // PID
  integral += error;
  integral = constrain(integral, -100, 100);
  float correction = Kp * error + Ki * integral + Kd * (error - lastError);
  lastError = error;
  if (REVERSE_STEERING) correction = -correction;

  int leftSpeed  = constrain((int)(baseSpeed + correction), minSpeed, maxSpeed);
  int rightSpeed = constrain((int)(baseSpeed - correction), minSpeed, maxSpeed);
  setMotors(leftSpeed, rightSpeed);

  debugPrint(count, error, leftSpeed, rightSpeed);
}

// Reads the 5 sensors. Returns how many see the line.
// If at least one sees it, writes the line position into "error".
int readSensors(float &error) {
  int count = 0;
  float sum = 0;
  for (int i = 0; i < 5; i++) {
    if (digitalRead(SENSOR_PIN[i]) == lineValue) {
      sum += WEIGHT[i];
      count++;
    }
  }
  if (count > 0) error = sum / count;
  return count;
}

// Most sensors are over white at start, so the less common value = black.
void detectLineColor() {
  if (LINE_MODE == 1) { lineValue = HIGH; }
  else if (LINE_MODE == 2) { lineValue = LOW; }
  else {
    int highCount = 0, total = 0;
    for (int r = 0; r < 20; r++) {
      for (int i = 0; i < 5; i++) {
        if (digitalRead(SENSOR_PIN[i]) == HIGH) highCount++;
        total++;
      }
      delay(10);
    }
    lineValue = (highCount * 2 > total) ? LOW : HIGH;
  }
  Serial.print(F("Black line reads as: "));
  Serial.println(lineValue == HIGH ? F("HIGH (1)") : F("LOW (0)"));
}

// left/right: -255..255, positive = forward
void setMotors(int left, int right) {
  int a = MOTOR_A_IS_LEFT ? left : right;
  int b = MOTOR_A_IS_LEFT ? right : left;
  driveMotor(ENA, IN1, IN2, MOTOR_A_REVERSED ? -a : a);
  driveMotor(ENB, IN3, IN4, MOTOR_B_REVERSED ? -b : b);
}

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

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void debugPrint(int count, float error, int l, int r) {
  if (!DEBUG) return;
  if (millis() - lastPrint < 200) return;
  lastPrint = millis();
  for (int i = 0; i < 5; i++) Serial.print(digitalRead(SENSOR_PIN[i]) == lineValue ? '#' : '.');
  Serial.print(F("  seen=")); Serial.print(count);
  Serial.print(F("  err=")); Serial.print(error);
  Serial.print(F("  L=")); Serial.print(l);
  Serial.print(F("  R=")); Serial.println(r);
}
