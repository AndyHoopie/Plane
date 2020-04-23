#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo servoX;
Servo servoY;
Servo servoMotor;

RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";
int servo_x_pin = 6;
int servo_y_pin = 5;
int servo_motor_pin = 9;

unsigned long startTime;

struct Signal {
  int x;
  int y;
  int throttle;
};

Signal data;


void setup() {
  Serial.begin(9600);

  servoX.attach(servo_x_pin);
  servoY.attach(servo_y_pin);
  servoMotor.attach(servo_motor_pin, 1000, 2000);

  data.x = 0;
  data.y = 0;
  data.throttle = 0;

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  startTime = millis();
}

void loop() {
  if (!radio.available()) {
    return;
  }

  radio.read(&data, sizeof(Signal));

  data.x = map(data.x, 0, 1023, 0, 180);
  data.y = map(data.y, 0, 1023, 0, 180);

  servoX.write(data.x);
  servoY.write(data.y);

  data.throttle = map(data.throttle, 0, 1023, 0, 180);

  if (data.throttle < 10) {
    data.throttle = 0;
  }

  servoMotor.write(data.throttle);
}
