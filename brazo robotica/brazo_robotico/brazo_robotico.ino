#include <Servo.h>

int xInput[] = {A0};
int yInput[] = {A1};
int xOutput[] = {3};
int yOutput[] = {5};
int x[] = {0};
int y[] = {0};

const int number = 1;

Servo servosX[number];
Servo servosY[number];

void setUpServos(){
  for(int i = 0; i<number; i++){
    servosX[i].attach(xOutput[i]);
    servosY[i].attach(yOutput[i]);
  }
}
void sendAllServos(){
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
    x[i]=xVal;
    Serial.println(yVal);
    y[i]=yVal;
  }
  delay(100);
  sendAllServos();

  
 
}


void checkAllOverValues(){
  for(int i = 0; i<number; i++){
    if(x[i] < 0){x[i] = 0;}
    if(x[i] > 180){x[i] = 180;}
    if(y[i] < 0){y[i] = 0;}
    if(y[i] > 180){y[i] = 180;}
  }
}
