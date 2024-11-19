#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); // RX, TX

int value;
const int buzzerPin = 8; // Buzzer connected to digital pin 8 (D8)
const char phoneNumber[] = "+919493818880"; // Phone number to call when water level is low

void setup() {
  pinMode(A0, INPUT); // Sensor input
  pinMode(13, OUTPUT); // Green LED
  pinMode(12, OUTPUT); // Yellow LED
  pinMode(11, OUTPUT); // Red LED
  pinMode(buzzerPin, OUTPUT); // Buzzer

  Serial.begin(9600); // Start serial communication for debugging
  mySerial.begin(9600); // Start serial communication with GSM module
  
  delay(1000);
  initializeGSM();
}

void loop() {
  value = analogRead(A0); // Read the sensor value from analog pin A0
  Serial.print("Sensor Value: "); 
  Serial.println(value); // Print the sensor value to the serial monitor

  // Determine which LED and buzzer state based on the sensor value
  if (value <= 300) {
    digitalWrite(13, LOW);  // Turn off Green LED
    digitalWrite(12, LOW);  // Turn off Yellow LED
    digitalWrite(11, HIGH); // Turn on Red LED
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    Serial.println("Red LED ON, Buzzer ON");
    makeCall();
  } else if (value <= 500) {
    digitalWrite(13, LOW);   // Turn off Green LED
    digitalWrite(12, HIGH);  // Turn on Yellow LED
    digitalWrite(11, LOW);   // Turn off Red LED
    digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
    Serial.println("Yellow LED ON, Buzzer OFF");
  } else {
    digitalWrite(13, HIGH);  // Turn on Green LED
    digitalWrite(12, LOW);   // Turn off Yellow LED
    digitalWrite(11, LOW);   // Turn off Red LED
    digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
    Serial.println("Green LED ON, Buzzer OFF");
  }

  delay(2000); // Wait 2 seconds before the next reading
}

void initializeGSM() {
  Serial.println("Initializing GSM module...");
  
  mySerial.println("AT"); // Check GSM module
  delay(1000);
  if (mySerial.available()) {
    Serial.println("GSM module responded");
  } else {
    Serial.println("No response from GSM module");
  }

  mySerial.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Configure SMS settings
  delay(1000);

  Serial.println("GSM module initialized");
}

void makeCall() {
  Serial.println("Making a call...");
  mySerial.print("ATD");
  mySerial.print(phoneNumber);
  mySerial.println(";"); // Command to dial the specified phone number
  delay(10000); // Wait for the call to go through
  mySerial.println("ATH"); // Command to hang up the call
  Serial.println("Call ended");
}

