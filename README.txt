LINE FOLLOWER ROBOT - HOW TO USE THESE FILES
============================================

Folders (keep each .ino inside its own folder with the same name - Arduino needs that):

  step1_sensor_test/step1_sensor_test.ino     -> check the 5 sensors
  step2_motor_test/step2_motor_test.ino       -> check the 2 motors
  step3_line_follower/step3_line_follower.ino -> the real robot code

---------------------------------------------
A. ONE-TIME SETUP (on a Windows PC)
---------------------------------------------
1. Install Arduino IDE from https://www.arduino.cc/en/software
2. Unzip this zip anywhere (e.g. Documents\Arduino).
3. Plug the Arduino into the PC with the USB cable.
4. In Arduino IDE: Tools > Board > Arduino AVR Boards > Arduino Uno
5. Tools > Port > pick the COM port that appeared (e.g. COM3).
   - No COM port shows up? The board is probably a clone with a CH340 chip.
     Search "CH340 driver Windows", install it, re-plug the USB.

To upload a sketch: File > Open > pick the .ino file > click the arrow (Upload) button.
"Done uploading" at the bottom = success.

---------------------------------------------
B. TRACK
---------------------------------------------
Do NOT use whiteboard marker. Use BLACK ELECTRICAL TAPE (~18-19 mm wide)
on white paper / matte white poster board. Gentle curves first.

Sensor height: TCRT5000 sensors should be about 3-8 mm above the track.
If they are higher, lower the sensor bar (mount it under the chassis).

---------------------------------------------
C. ORDER
---------------------------------------------
STEP 1 - Sensor test
  - Upload step1. Open Tools > Serial Monitor, 9600 baud.
  - Slide the robot across the tape by hand.
  - Each sensor must change cleanly between 0 and 1 over white vs black.
  - Nothing changes? -> wires may be on A0-A4: edit SENSOR_PIN line,
    or sensors too high, or wrong track.

STEP 2 - Motor test
  - LIFT the robot, battery ON, upload step2, watch Serial Monitor.
  - Note: MOTOR A = left or right wheel? Each motor forward or backward?

STEP 3 - Line follower
  - Open step3, edit the SETTINGS block at the top with your step 2 answers:
       MOTOR_A_IS_LEFT, MOTOR_A_REVERSED, MOTOR_B_REVERSED
  - Upload. Unplug USB. Put the robot on the tape, line under the MIDDLE sensor.
  - Battery ON, press the RESET button on the Arduino. 3 s wait, then it goes.

---------------------------------------------
D. IF IT MISBEHAVES (change in SETTINGS, upload again)
---------------------------------------------
  Turns AWAY from the line         -> REVERSE_STEERING = true
  Motors hum but don't move        -> baseSpeed higher (150, 170) / charge battery
  Too fast, flies off curves       -> baseSpeed lower (110)
  Wobbles left-right a lot         -> raise Kd (35, 45) or lower Kp (12, 10)
  Too lazy, drifts off on curves   -> raise Kp (18, 22)
  Want to see what it thinks       -> DEBUG = true, keep USB in, Serial Monitor 9600

Notes:
  - Arduino GND and L298N GND must be connected together.
  - Remove the jumpers on the L298N ENA and ENB pins (pins 9 and 10 drive them).
  - The robot stops if all 5 sensors see black (e.g. when you lift it up).
