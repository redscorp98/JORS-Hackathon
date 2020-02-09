#define X_JOY_ANALOG_PIN A0
#define Y_JOY_ANALOG_PIN A1
#define ULTRASOUND_TRIG_PIN 5
#define ULTRASOUND_ECHO_PIN 2

uint32_t microsEchoBegin;
uint32_t microsEchoEnd;
uint32_t millisTrigEnd;
uint8_t echoPulse = false;
uint32_t distance_cm;
bool waitingForEchoPulse = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
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
    uint32_t distance_cm_temp = ((microsEchoEnd - microsEchoBegin)/2) * (0.0343);
    if (distance_cm_temp < 400)
      distance_cm = distance_cm_temp;
  }
}

void loop() {
  uint16_t xJoyVal = analogRead(X_JOY_ANALOG_PIN);  // read the input pin
  uint16_t yJoyVal = analogRead(Y_JOY_ANALOG_PIN);  // read the input pin

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

  /*
  Serial.print("Distance ");
  Serial.print(distance_cm);
  Serial.print(" X ");
  Serial.print(xJoyVal);
  Serial.print(" Y ");
  Serial.println(yJoyVal);
  */
  
  if (Serial.available())
  {
    while (Serial.available())
      Serial.read();
    Serial.write((char*)&distance_cm, 4);
    Serial.write((char*)&xJoyVal, 2);
    Serial.write((char*)&yJoyVal, 2);
  }
}
