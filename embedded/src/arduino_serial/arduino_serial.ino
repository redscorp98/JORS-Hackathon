#define X_JOY_ANALOG_PIN A0
#define Y_JOY_ANALOG_PIN A1
#define BUTTON_PIN 3
#define ULTRASOUND_TRIG_PIN 5
#define ULTRASOUND_ECHO_PIN 2

unsigned long microsEchoBegin;
unsigned long microsEchoEnd;
unsigned long millisTrigEnd;
bool echoPulse = false;
unsigned long distance_cm;
bool waitingForEchoPulse = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(ULTRASOUND_TRIG_PIN, OUTPUT);
  pinMode(ULTRASOUND_ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ULTRASOUND_ECHO_PIN), echo_change, CHANGE);
}

void echo_change() {
  echoPulse = !echoPulse;
  microsEchoBegin = microsEchoEnd;
  microsEchoEnd = micros();
  if (!echoPulse)
  {
    waitingForEchoPulse = false;
    unsigned long distance_cm_temp = ((microsEchoEnd - microsEchoBegin)/2) * (0.0343);
    if (distance_cm_temp < 400)
      distance_cm = distance_cm_temp;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  bool buttonVal = !digitalRead(BUTTON_PIN);
  int xJoyVal = analogRead(X_JOY_ANALOG_PIN);  // read the input pin
  int yJoyVal = analogRead(Y_JOY_ANALOG_PIN);  // read the input pin

  if (!waitingForEchoPulse || (millis() - millisTrigEnd) > 5000)
  {
    echoPulse = false;
    //Serial.println("Sending Trig Pulse");
    digitalWrite(ULTRASOUND_TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(ULTRASOUND_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASOUND_TRIG_PIN, LOW);
    millisTrigEnd = millis();
    waitingForEchoPulse = true;
  }
  
  Serial.print("Button ");
  Serial.print(buttonVal);
  Serial.print(" Distance ");
  Serial.print(distance_cm);
  Serial.print(" X ");
  Serial.print(xJoyVal);
  Serial.print(" Y ");
  Serial.println(yJoyVal);
}
