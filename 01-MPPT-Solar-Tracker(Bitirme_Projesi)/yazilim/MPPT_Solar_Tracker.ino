#include <Servo.h>

Servo myServo1;
Servo myServo2;

// ===== 1. MOTOR =====
const int servoPin1 = 9;
const int ldr1Ust = A0;
const int ldr1Alt = A1;

const int startAngle1 = 0;
const int endAngle1 = 30;
int currentAngle1 = 0;

// ===== 2. MOTOR =====
const int servoPin2 = 10;
const int ldr2Ust = A3;
const int ldr2Alt = A2;

const int startAngle2 = 0;
const int endAngle2 = 180;
int currentAngle2 = 90; // orta nokta

// Ortak ayarlar
const int esikDeger = 40;
const int delayTime = 1000; // ÇOK YAVAŞ

void setup() {
  // 1. motor
  myServo1.attach(servoPin1);
  myServo1.write(currentAngle1);

  // 2. motor
  myServo2.attach(servoPin2);
  myServo2.write(currentAngle2);

  delay(1000);
}

void loop() {
  // ===== 1. MOTOR OKUMA =====
  int ustDeger1 = analogRead(ldr1Ust);
  int altDeger1 = analogRead(ldr1Alt);
  int fark1 = ustDeger1 - altDeger1;

  if (fark1 > esikDeger && currentAngle1 > startAngle1) {
    currentAngle1--;
    myServo1.write(currentAngle1);
    delay(delayTime);
  }
  else if (fark1 < -esikDeger && currentAngle1 < endAngle1) {
    currentAngle1++;
    myServo1.write(currentAngle1);
    delay(delayTime);
  }

  // ===== 2. MOTOR OKUMA =====
  int ustDeger2 = analogRead(ldr2Ust);
  int altDeger2 = analogRead(ldr2Alt);
  int fark2 = ustDeger2 - altDeger2;

  if (fark2 > esikDeger && currentAngle2 > startAngle2) {
    currentAngle2--;
    myServo2.write(currentAngle2);
    delay(delayTime);
  }
  else if (fark2 < -esikDeger && currentAngle2 < endAngle2) {
    currentAngle2++;
    myServo2.write(currentAngle2);
    delay(delayTime);
  }

  delay(100);
}

