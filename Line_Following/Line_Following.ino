/**
  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see < https : //www.gnu.org/licenses/>.
**/


/* Includes -------------------------------------------------- */
#include <Servo.h>

/* Defines ------------------------------------------------------------------ */
#define button_pin      2
#define right_servo_pin 5
#define left_servo_pin  6
#define right_led       7
#define left_led        8
#define left_qti        A0
#define middle_qti      A1
#define right_qti       A2
#define min_pulse       1300
#define max_pulse       1700
#define standstill      1500
#define qti_threshold   150 // 150 if sensors are white, 400 if sensors are red

/* Global variables ------------------------------------------ */
Servo g_left_wheel;
Servo g_right_wheel;

/* Private functions ------------------------------------------------- */
byte readQti (byte qti) {                               // Function to read current position on map
  digitalWrite(qti, HIGH);                              // Send an infrared signal
  delayMicroseconds(1000);                               // Wait for 1000uS, very important!
  digitalWrite(qti, LOW);                               // Set the pin low again
  return ( analogRead(qti) > qti_threshold ? 1 : 0);    // Return the converted result: if analog value more then 100 return 1, else 0
}

void setWheels(int delay_left = 1500, int delay_right = 1500) {
  g_left_wheel.writeMicroseconds(delay_left);
  g_right_wheel.writeMicroseconds(delay_right);
  delay(20);
}
void setLed(byte value_left = LOW, byte value_right = LOW) {
  digitalWrite(right_led, value_right);
  digitalWrite(left_led, value_left);
}

/* Arduino functions ---------------------------------------------------------------- */
void setup() {
  /* Start serial monitor */
  Serial.begin(9600);

  /* Set the pin mode of LED pins as output */
  pinMode(right_led, OUTPUT);
  pinMode(left_led, OUTPUT);

  /* Attach servos to digital pins defined earlier */
  g_left_wheel.attach(left_servo_pin);
  g_right_wheel.attach(right_servo_pin);

  setWheels();

  setLed(HIGH, HIGH);
  delay(500);
  setLed();
  delay(500);

}

void loop() {
  
    if (readQti(left_qti) && readQti(right_qti)) {
      setWheels();
      setLed(HIGH, HIGH);
    } else if (readQti(left_qti)) {
      setWheels(1300, 1300);
      setLed(HIGH, LOW);
    } else if (readQti(right_qti)) {
      setWheels(1700, 1700);
      setLed(LOW, HIGH);
    } else {
      setWheels(1700, 1300);
      setLed(LOW, LOW);
    }


}
