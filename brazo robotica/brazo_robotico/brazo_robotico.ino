#include <Servo.h>
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//      If any comment is removed the program will blow up 💣💣💣      //
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Updates of this code are available at: 
https://github.com/oriolgds/Arduino/blob/main/brazo%20robotica/brazo_robotico/brazo_robotico.ino


*************************************************************************
Code made by: Oriol Giner Díaz
Cable connection made by: Oriol Giner Díaz

If you want to see more Arduino projects about me head to:
https://github.com/oriolgds/Arduino/

To see more projects made by me that aren't Arduino head to:
https://github.com/oriolgds/


*/


int xInput[] = {A1, A2};
int yInput[] = {A0, A3};
int xOutput[] = {3, 10};
int yOutput[] = {5, 9};

const int center = 88;
const int deadZone = 50;
const int centerDeadZone = 5;
const int sprintV = 10;
const int slowV = 1;
int x[] = {center, center};
int y[] = {center, center};

const int number = 2;

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
    Serial.print("x Val: "); Serial.print(x[i]); Serial.print(", ");
    //Serial.print("y Val: "); Serial.println(yVal);
    if(!(xVal > center - centerDeadZone && xVal < center + centerDeadZone)){
      if(xVal < center - deadZone){
        x[i] = x[i] - sprintV;
      }
      if(xVal >= center - deadZone && xVal < center){
        x[i] = x[i] - slowV;
      }
      if(xVal > center + deadZone){
        x[i] = x[i] + sprintV;
      }
      if(xVal <= center + deadZone && xVal > center){
        x[i] = x[i] + slowV;
      }
    }
    if(!(yVal > center - centerDeadZone && yVal < center + centerDeadZone)){
      if(yVal < center - deadZone){
        y[i] = y[i] - sprintV;
      }
      if(yVal >= center - deadZone && yVal < center){
        y[i] = y[i] - slowV;
      }
      if(yVal > center + deadZone){
        y[i] = y[i] + sprintV;
      }
      if(yVal <= center + deadZone && yVal > center){
        y[i] = y[i] + slowV;
      }
    }    
  }
  Serial.println(" ");
  sendAllServos();
  delay(100); 
 
}
