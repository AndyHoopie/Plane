#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";

#define outputA 3
#define outputB 4

int x_key = 6;                                               
int y_key = 5;                                               
int x_pos;
int y_pos;
int counter = 0; 
int aState;
int bState;
 
struct Signal {
int x;
int y;
int throttle;
};

Signal data;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode (x_key, INPUT) ;                     
  pinMode (y_key, INPUT) ;  

  Serial.begin (9600); 

  data.x = 0;
  data.y = 0;
  data.throttle = 0;
}


void loop() { 
  aState = digitalRead(outputA);
  bState = digitalRead(outputB);
  
  if (aState == 0 && bState == 1) {
    counter++;
  } else if (aState == 1 && bState == 0) {
    counter--;
  }

  counter = max(min(60, counter), 0);
  
  data.throttle = counter;
  data.x = analogRead (x_key) ;  
  data.y = analogRead (y_key) ; 
  radio.write(&data, sizeof(Signal));
 }
