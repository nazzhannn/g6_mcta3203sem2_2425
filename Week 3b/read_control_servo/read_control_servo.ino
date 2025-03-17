#include <Servo.h>

Servo servo;
int potPin = A0;  // Potentiometer connected to A0
int angle = 0;    // Servo position

void setup() {
    Serial.begin(9600);  // Start serial communication
    servo.attach(9);     // Attach the servo to digital pin 9
    Serial.println("Arduino Ready");
}

void loop() {
    int potValue = analogRead(potPin);  // Read potentiometer value (0-1023)
    angle = map(potValue, 0, 1023, 0, 180);  // Convert to servo angle (0-180)
    
    servo.write(angle);  // Move the servo
    Serial.println(angle);  // Send angle to Python
    
    delay(100);  // Small delay to stabilize readings
}
