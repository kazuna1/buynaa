/*
  STEP 1 - SENSOR TEST
  --------------------
  Shows what each of the 5 sensors reads (0 or 1) in the Serial Monitor.

  How to use:
  1. Upload this sketch.
  2. Open Tools > Serial Monitor, set speed to 9600 baud.
  3. Hold the robot at its normal height over the track and slide it slowly
     across the black line by hand.
  4. Write down:
       - What does BLACK print? 0 or 1?
       - When the line is under the LEFT-most sensor, which S number changes?
*/

// Sensor bar OUT1..OUT5 wires.
// If your brother plugged them into A0..A4 instead, change this line to:
// const int SENSOR_PIN[5] = {A0, A1, A2, A3, A4};
const int SENSOR_PIN[5] = {2, 3, 4, 5, 6};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    pinMode(SENSOR_PIN[i], INPUT);
  }
  Serial.println(F("Sensor test. Slide the robot across the black line."));
  Serial.println(F("S1 S2 S3 S4 S5"));
}

void loop() {
  for (int i = 0; i < 5; i++) {
    Serial.print(' ');
    Serial.print(digitalRead(SENSOR_PIN[i]));
    Serial.print(F("  "));
  }
  Serial.println();
  delay(200);
}
