char reading;

void setup () {

    Serial.begin (9600);

}

void loop () {

    if (Serial.available ()) {

        reading = Serial.read ();

        Serial.println (reading);

    }

}
