int redPin = 9;   // Pin for the red LED
int greenPin = 10; // Pin for the green LED

void setup() {
  // Set the LED pins as output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  // Turn on red LED and turn off green LED
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  delay(500); // Wait for 0.5 seconds

  // Turn off red LED and turn on green LED
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  delay(500); // Wait for 0.5 seconds
}
