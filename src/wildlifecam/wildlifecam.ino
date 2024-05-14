// ESP32 Wrover Cam

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Forsen";
const char* password = "9999999999999999";
const char* serverName = "192.168.0.3";
const int serverPort = 8000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println("Connected to the WiFi network");
  
  Serial.write("Setup complete\n\n");
}

void sendMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) { // Check the current connection status
    HTTPClient http;
    String serverPath = String("http://") + serverName + ":" + String(serverPort) + "/printmessage/";
    
    http.begin(serverPath.c_str()); // Specify the URL
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    // json with message="message"
    String json = "{\"message\":\"" + message + "\"}";
    int httpResponseCode = http.POST(json); // Send the request
    String response = http.getString(); // Get the response payload

    Serial.println("Response code: " + String(httpResponseCode)); // Print HTTP return code
    Serial.println("Response: " + response); // Print request response payload
  }
    
}

void blink_fast(int n) {
  // n to string
  static char n_char[10];
  itoa(n, n_char, 10);

  Serial.write("Blinking ");
  sendMessage("Blinking!!!");
  Serial.write(n_char);
  Serial.write(" times\n");
  while(n>0) {
    // to turn on the onboard LED, it actually needs to be set to LOW
    digitalWrite(2, LOW);
    delay(200);
    // same as above, to turn off the onboard LED, it actually needs to be set to HIGH
    digitalWrite(2, HIGH);
    delay(200);

    n--;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // blink the onboard LED
  blink_fast(5);

  delay(2000);

}
