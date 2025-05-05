float tempcelc;
int ldr_value;
int ldr_percent;

void setup() {
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL,CLOCK,TEMPERATURE,LIGHT");
}

void loop() {
  int lm_value = analogRead(A0);
  tempcelc = (lm_value / 1023.0) * 5000.0;
  tempcelc = tempcelc / 10.0;

  ldr_value = analogRead(A1);
  ldr_percent = map(ldr_value, 0, 1023, 0, 100);

  Serial.print("DATA,TIME,");
  Serial.print(tempcelc);
  Serial.print(",");
  Serial.println(ldr_percent);

  delay(1500);
}
