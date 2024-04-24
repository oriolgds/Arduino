int led = 13;
String reading;

void setup(){
  Serial.begin(19200);
  pinMode(led, OUTPUT); // Set the LED pin as an output
}

void loop(){
  if(Serial.available()){
    reading = Serial.readString(); // Read the characters into a String
    Serial.println(reading);
    if(reading == "ON"){
      digitalWrite(led, HIGH);
    }
    else if(reading == "OFF"){
      digitalWrite(led, LOW);
    }
  }
}
