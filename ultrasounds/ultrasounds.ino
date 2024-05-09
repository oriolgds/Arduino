// Define the pins for the ultrasonic sensor
const int trigPin = 12;
const int echoPin = 11;

// Define the pin for the LED
const int ledPin = 13;

// Define variables for storing distance and duration
long duration;
int distance;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set trigPin as OUTPUT and echoPin as INPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set ledPin as OUTPUT
  pinMode(ledPin, OUTPUT);
}

void loop() {
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

  // Turn on LED if an object is detected within 50 cm
  if (distance < 50) {
    digitalWrite(ledPin, HIGH); // Turn on LED
  } else {
    digitalWrite(ledPin, LOW); // Turn off LED
  }

  // Wait a short delay before taking the next reading
  delay(100);
}
