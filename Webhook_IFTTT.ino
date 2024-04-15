#include <WiFiNINA.h>
#define SUNTHRESHOLD 200   // Light level we consider "sunlight"

// Wifi Information
WiFiClient client;
char ssid[] = "wifi1";
char pass[] = "P@ssword123456789";

// IFTTT Webhook Trigger
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/sunlight_threshold/with/key/bUkKVtlxaqyi1w77_9x8fE";

// Sunlight variables
int sensorPin = A0;
int sensorValue;
bool sunLight;

void setup() {
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
}

void loop() {

  sensorValue = analogRead(sensorPin);
  
  if(sunLight == false && sensorValue > SUNTHRESHOLD) {
    sunLight = true;
    SendWebhook();
  } else if(sunLight == true && sensorValue < SUNTHRESHOLD){
    sunLight = false;
    SendWebhook();
  }
  //Delay 1 second
  delay(1000);
}

void SendWebhook() {
  String queryString = "?value1=" + String(sunLight) + "&value2=" + String(sensorValue);
  while (!Serial);
  if (client.connect(HOST_NAME, 80)) {
    // if connected
    Serial.println("Connected to IFTTT server");
  }
  // Shows if the connection has failed or not
  else {
    // if not connected:
    Serial.println("connection failed");
  }

  client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header

  while (client.connected()) {
    if (client.available()) {
      // read information sent from the server and output to the serial monitor
      char client = client.read();
      Serial.print(client);
    }
  }

  // Stops the client:
  client.stop();
  Serial.println();
  Serial.println("Client Disconnected");
}
