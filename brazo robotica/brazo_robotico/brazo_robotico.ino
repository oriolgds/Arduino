#include <Servo.h>

int xInput[] = {A0};
int yInput[] = {A1};
int xOutput[] = {3};
int yOutput[] = {5};

const int center = 88;
const int deadZone = 10;
const int sprintV = 10;
const int slowV = 3;
int x[] = {center};
int y[] = {center};

const int number = 1;

Servo servosX[number];
Servo servosY[number];

void setUpServos(){
  for(int i = 0; i<number; i++){
    servosX[i].attach(xOutput[i]);
    servosY[i].attach(yOutput[i]);
  }
}

void checkAllOverValues(){
  for(int i = 0; i<number; i++){
    if(x[i] < 0){x[i] = 0;}
    if(x[i] > 180){x[i] = 180;}
    if(y[i] < 0){y[i] = 0;}
    if(y[i] > 180){y[i] = 180;}
  }
}
void sendAllServos(){
  checkAllOverValues();
  for(int i = 0; i<number; i++){
    servosX[i].write(x[i]);
    servosY[i].write(y[i]);
  }
}
void setup() {
  Serial.begin(9600);
  setUpServos();

}

void loop() {
  for(int i = 0; i<number; i++){
    int xR = analogRead(xInput[i]);
    int yR = analogRead(yInput[i]);
    int xVal = map(xR, 0, 1023, 0, 179);
    int yVal = map(yR, 0, 1023, 0, 179);
    if(xVal < center - deadZone){
      x[i] = x[i] - sprintV;
    }
    else if(xVal >= center - deadZone && xVal < center){
      x[i] = x[i] - slowV;
    }
    if(xVal > center + deadZone){
      x[i] = x[i] + sprintV;
    }
    else if(xVal <= center + deadZone && xVal > center){
      x[i] = x[i] + slowV;
    }
    if(yVal < center - deadZone){
      y[i] = y[i] - sprintV;
    }
    else if(yVal >= center - deadZone && yVal < center){
      y[i] = y[i] - slowV;
    }
    if(yVal > center + deadZone){
      y[i] = y[i] + sprintV;
    }
    else if(yVal <= center + deadZone && yVal > center){
      y[i] = y[i] + slowV;
    }
  }
  delay(10);
  sendAllServos();
  delay(10);

  
 
}
