int redLight = 3;
int yellowLight = 6;
int greenLight = 11;

void setup() {
    pinMode(redLight, OUTPUT);
    pinMode(yellowLight, OUTPUT);
    pinMode(greenLight, OUTPUT);
}

void loop() {
    // Red light on
    digitalWrite(redLight, HIGH);
    delay(5000); // Wait for 5 seconds
    digitalWrite(redLight, LOW);

    // Green light on
    digitalWrite(greenLight, HIGH);
    delay(5000); // Wait for 5 seconds
    digitalWrite(greenLight, LOW);

    // Yellow light on
    digitalWrite(yellowLight, HIGH);
    delay(2000); // Wait for 2 seconds
    digitalWrite(yellowLight, LOW);
}