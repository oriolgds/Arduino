int led = 13;
String reading;

void setup(){
  Serial.begin(19200);
  pinMode(led, OUTPUT); // Set the LED pin as an output
  digitalWrite(led, HIGH);
}

void loop(){
  if(Serial.available()){
    reading = Serial.readString(); // Read the characters into a String
    Serial.println(reading);
    if(reading.substring(0, 2).equals("ON")){
      digitalWrite(led, HIGH);
    }
    else if(reading.substring(0,3).equals("OFF")){
      digitalWrite(led, LOW);
    }
  }
}
