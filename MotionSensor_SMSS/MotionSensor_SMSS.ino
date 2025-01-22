#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
// Rough Code by MarkMequits (C++), ArduinoIDE 2.3.5-nightly
MPU6050 mpu;
SoftwareSerial SIM800(7, 8); // RX, TX

void setup() {
  Serial.begin(9600);
  SIM800.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
  Serial.println("MPU6050 connection successful");
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  Serial.print("Ax: "); Serial.print(ax_g);
  Serial.print(" Ay: "); Serial.print(ay_g);
  Serial.print(" Az: "); Serial.println(az_g);

  float threshold = 0.5; // Adjust this threshold if needed (or idk as you want)
  if (abs(ax_g) > threshold || abs(ay_g) > threshold || abs(az_g) > threshold) {
    Serial.println("Earthquake detected!");
    sendSMS("Earthquake detected! Please take precautions.");
  }

  delay(500);
}

void sendSMS(String message) {
  SIM800.print("AT+CMGF=1\r"); // Set SMS mode (idkwhat this is tutorial lng)
  delay(100);
  SIM800.print("AT+CMGS=\"+xxxxxxxxxxxxxx\"\r"); // Replace with phone number
  delay(100);
  SIM800.print(message);
  delay(100);
  SIM800.write(26); // Send Ctrl+Z to send the message (idk bro,but yeah)
  delay(5000);
}