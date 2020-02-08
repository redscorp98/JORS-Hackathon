#define X_JOY_ANALOG_PIN A0
#define Y_JOY_ANALOG_PIN A1
#define BUTTON_PIN 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool buttonVal = !digitalRead(BUTTON_PIN);
  int xJoyVal = analogRead(X_JOY_ANALOG_PIN);  // read the input pin
  int yJoyVal = analogRead(Y_JOY_ANALOG_PIN);  // read the input pin

  Serial.print("Button ");
  Serial.print(buttonVal);
  Serial.print(" X ");
  Serial.print(xJoyVal);
  Serial.print(" Y ");
  Serial.println(yJoyVal);
}
