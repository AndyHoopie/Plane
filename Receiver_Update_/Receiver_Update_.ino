#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo servoX;
Servo servoY;
//Servo ESC;
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";
int servo_x_pin = 6;
int servo_y_pin = 5;

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

  data.x = 0;
  data.y = 0;
  data.throttle = 0;
  
  radio.begin(); 
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  //   Serial.println("penis");
 if (!radio.available()) { return; }
// Serial.println("vagina");
 
  radio.read(&data, sizeof(Signal));
  
  data.x = map(data.x, 0, 1023, 0, 180);
  data.y = map(data.y, 0, 1023, 0, 180);
  
  if (data.x<400 || data.x>600) {
    servoX.write(data.x) ;
  }
  
  if (data.y<400 || data.y>600) {
    servoY.write(data.y) ;
  }   
  data.throttle = map(data.throttle, 0, 60, 0, 255); //TODO: update when we have motor
  Serial.print("Throttle:");
  Serial.println(data.throttle);
  analogWrite(4, 255);
}
