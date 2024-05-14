// ESP32 Wrover Cam

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  Serial.write("Setup complete\n\n");
}

void blink_fast(int n) {
  // n to string
  static char n_char[10];
  itoa(n, n_char, 10);

  Serial.write("Blinking ");
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
