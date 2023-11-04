#define BLYNK_TEMPLATE_ID "TMPL3vKOSZW1a"
#define BLYNK_TEMPLATE_NAME "Heart Health Monitoring"
#define BLYNK_AUTH_TOKEN "3Q5tC7xGt2O4_Q-BJyzfZgH0eErPzyYe"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Shashank"; // Replace with your WiFi SSID
char pass[] = "Shashank123"; // Replace with your WiFi password

void setup() {
  pinMode(1, OUTPUT); // Initialize pin 1 as an output
  Serial.begin(115200); // Start serial communication
  Blynk.begin(auth, ssid, pass); // Connect to the Blynk server

  // Initialize ECG sensor-related setup here, if necessary
  pinMode(7, INPUT); // Setup for leads off detection LO +
  pinMode(6, INPUT); // Setup for leads off detection LO -
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(1, value); // Set pin 1 HIGH or LOW based on the Blynk app value
}

void loop() {
  Blynk.run(); // Keep Blynk running

  if ((digitalRead(7) == 1) || (digitalRead(6) == 1)) {
    Serial.println("Leads off detected"); // Debug message
    Blynk.virtualWrite(V2, 0); // If leads off detected, set V2 in Blynk app to 0
  } else {
    // Read ECG sensor value
    int ecgValue = analogRead(A0); // Read the analog value from pin A0
    Blynk.virtualWrite(V2, ecgValue); // Update Blynk app with ECG value (assumes V2 is used for ECG display)

    if (ecgValue >= 400) {
      Serial.println("High ECG value detected"); // Debug message
      Blynk.virtualWrite(V1, 0); // If ECG value is high, set V1 in Blynk app to 0
      delay(500);
      Blynk.virtualWrite(V1, 1); // After a delay, set V1 back to 1
    }
  }
}
