void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

int count = 69;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Hello world: ");
  Serial.println(count);
  delay(1000);
  count++;
}
