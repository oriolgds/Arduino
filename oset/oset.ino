const int trigPin = 12;
const int echoPin = 11;

const int redPin = 6;
const int bluePin = 5;
const int greenPin = 3;

const int motorPin = 2;
long duration;
int distance;

void setup() {
  Serial.begin(19200);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void ultra(){
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 microsecond pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the duration of the pulse from echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  // Print distance to serial monitor
  Serial.println("Distance: ");
  Serial.println(distance);
  Serial.println(" cm");
}
void checkDistance(){
  ultra();
  while(distance < 60){
    ultra();
    if(distance < 20){
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 255);
    }
    else if(distance < 40){
      analogWrite(redPin, 0);
      analogWrite(greenPin, 255);
      analogWrite(bluePin, 0);
    }
    else {
      analogWrite(redPin, 255);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
    }
    
  }
}
void loop() {
  digitalWrite(motorPin, 1);

  // Fade in rojo
  for (int brightness = 0; brightness < 256; brightness++) {
    analogWrite(redPin, brightness);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    checkDistance();    
    delayMicroseconds(1);  // Ajusta la velocidad del fade in aquí
  }
  checkDistance();
  
  // Fade out rojo
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(redPin, brightness);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    checkDistance();    
    delayMicroseconds(1);  // Ajusta la velocidad del fade out aquí
  }
  checkDistance();
  
  // Repite el proceso para verde y azul
  // Fade in verde
  for (int brightness = 0; brightness < 256; brightness++) {
    analogWrite(greenPin, brightness);
    analogWrite(redPin, 0);
    analogWrite(bluePin, 0);
    checkDistance();    
    delayMicroseconds(1);  // Ajusta la velocidad del fade in aquí
  }
  checkDistance();
  
  // Fade out verde
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(greenPin, brightness);
    analogWrite(redPin, 0);
    analogWrite(bluePin, 0);
    checkDistance();    
    delayMicroseconds(1); // Ajusta la velocidad del fade out aquí
  }
  checkDistance();
  
  // Fade in azul
  for (int brightness = 0; brightness < 256; brightness++) {
    analogWrite(bluePin, brightness);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    checkDistance();    
    delayMicroseconds(1); // Ajusta la velocidad del fade in aquí
  }
  checkDistance();
  
  // Fade out azul
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(bluePin, brightness);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    checkDistance();    
    delayMicroseconds(1); // Ajusta la velocidad del fade out aquí
  }
  checkDistance();  
}
