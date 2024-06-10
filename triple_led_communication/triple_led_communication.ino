int pins[] = {1, 2, 3};
String reading;

void setup(){
  Serial.begin(19200);
  for (int pin : pins) {
    pinMode(pin, OUTPUT);
  }
  for (int pin : pins) {
    analogWrite(pin, 255);
  }
}

void loop(){
  if(Serial.available()){
    reading = Serial.readString(); // Read the characters into a String
    String parsedReading = reading.substring(0,reading.length() - 1);
    Serial.println(parsedReading);
    if(parsedReading.equals("A")){
      for (int pin : pins) {
        analogWrite(pin, 100);
      }
    }
  }
}
