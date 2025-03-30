#include <Servo.h>

Servo servo;
int angle = 90; // Default angle

void setup() {
    Serial.begin(9600);  // Start serial communication
    servo.attach(9);     // Attach the servo to digital pin 9
    servo.write(angle);  // Move servo to default position
    Serial.println("Arduino Ready"); // Send confirmation to Python
}

void loop() {
    if (Serial.available() > 0) {  // If data is available
        String input = Serial.readStringUntil('\n'); // Read until newline
        input.trim(); // Remove any whitespace or newlines
        int receivedAngle = input.toInt(); // Convert to integer
        
        if (receivedAngle >= 0 && receivedAngle <= 180) { // Validate range
            servo.write(receivedAngle); // Move the servo
            Serial.print("Servo moved to: ");
            Serial.println(receivedAngle);
        } else {
            Serial.println("Invalid angle received");
        }
    }
}
