int lightValue;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
 lightValue = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  lightValue = analogRead(A0);
  Serial.println(lightValue);
  delay(1000);
}
