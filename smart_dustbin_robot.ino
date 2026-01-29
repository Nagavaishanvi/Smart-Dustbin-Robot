#include <Servo.h> // Include the Servo library
// Defines pin numbers
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 12;
const int ledPin2 = 13;
const int servoPin2 = 7;
const int potPin = A0; // Analog pin connected to soil sensor
// Defines variables
long duration;
int distance;
int safetyDistance = 5; // Safety distance threshold (in cm)
int soil = 0;
int fsoil;
int maxDryValue = 1; // This value decides how much humidity requires to treat waste as wet object
Servo myServo2; // Create a Servo object for controlling the lid
Servo servo1; // Create a Servo object for controlling waste classification
void setup() {
// Set up pins for ultrasonic sensor
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
// Set up pins for LEDs
pinMode(ledPin, OUTPUT); // Sets the LED1 as an Output
pinMode(ledPin2, OUTPUT); // Sets the LED2 as an Output
// Set up the servos
myServo2.attach(servoPin2); // Attaches the servo to pin 7
servo1.attach(8); // Attaches the second servo to pin 8
myServo2.write(0); // Initialize the lid servo to closed position (0 degrees)
servo1.write(90); // Initialize the waste classification servo to neutral position
// Start serial communication
Serial.begin(9600); // Starts the serial communication
Serial.println("Soil Sensor Ultrasonic Servo");
}
void loop() {
// Measure distance using ultrasonic sensor
measureDistance();
if (distance <= safetyDistance) {
// If object is within the safety distance
digitalWrite(ledPin, HIGH); // Turn LED1 ON
digitalWrite(ledPin2, HIGH); // Turn LED2 ON
myServo2.write(90); // Open the lid (90 degrees)
delay(2000); // Keep the lid open for 2 seconds
// Check soil moisture to classify waste
classifyWaste();
} else {
// If object is not detected
digitalWrite(ledPin, LOW); // Turn LED1 OFF
digitalWrite(ledPin2, LOW); // Turn LED2 OFF
myServo2.write(0); // Close the lid (0 degrees)
}
// Print the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
delay(100); // Short delay to avoid too much serial output
}
// Function to measure distance using the ultrasonic sensor
void measureDistance() {
// Send a pulse to trigger the ultrasonic sensor
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Read the echo time
duration = pulseIn(echoPin, HIGH);
// Calculate the distance based on the time taken for the pulse to return
distance = duration * 0.0344 / 2; // Speed of sound is 0.0344 cm/µs
}
// Function to classify waste based on soil moisture
void classifyWaste() {
soil = 0;
fsoil = 0;
for (int i = 0; i < 3; i++) // Take multiple readings for better accuracy
{
soil = analogRead(potPin);
soil = constrain(soil, 500, 100);
fsoil += map(soil, 500,10, 0, 100);
delay(75);
}
fsoil = fsoil / 3; // Calculate the average of soil moisture readings
Serial.print("Humidity: ");
Serial.print(fsoil);
Serial.println("%");
if (fsoil > maxDryValue) // Check if waste is wet
{
delay(1000);
Serial.println(" ==> WET Waste ");
servo1.write(170); // Move servo to indicate wet waste
delay(3000);
} else // Otherwise, treat as dry waste
{
delay(1000);
Serial.println(" ==> Dry Waste ");
servo1.write(10); // Move servo to indicate dry waste
delay(3000);
}
servo1.write(90); // Reset servo to neutral position
delay(1000);
}
