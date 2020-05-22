#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


const int x_key = A1;
const int y_key = A2;
const int a_key = 3;
const int b_key = 4;

int counter = 0;
int aState;
int bState;
//int speakerPin = 9;
// 
//int numTones = 3;
//int tones[] = {522, 698, 784};

struct Signal {
  int x;
  int y;
  int throttle;
};

Signal data;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  pinMode (a_key, INPUT);
  pinMode (b_key, INPUT);
  pinMode (x_key, INPUT);
  pinMode (y_key, INPUT);

  Serial.begin (9600);

  data.x = 0;
  data.y = 0;
  data.throttle = 0;
}

void loop() {
  aState = digitalRead(a_key);
  bState = digitalRead(b_key);

  int sensorValue = analogRead(A0);

  data.throttle = sensorValue;
  data.x = analogRead (x_key) ;
  data.y = analogRead (y_key) ;

Serial.println (data.x);
Serial.println (data.y);
Serial.println (data.throttle);

  radio.write(&data, sizeof(Signal));
}
