const byte ledPin = 13;
const byte relayPin1 = 10;
const byte controlPin = A0;
const byte buttonPin = A1;
const unsigned long debounce = 750;
const char splashText[16] = "Spot Welder 1.0";
const int durationLOW = 5;
const int durationHIGH = 251;

volatile byte buttonState = HIGH;
volatile unsigned long lastPress;
volatile unsigned long startTime;
volatile int state = 0;
volatile int duration = 80;

void clearScreen() {
  Serial.write(0xFE); // Control Character
  Serial.write(0x01); // Clear Display
  delay(10);
}

void setBrightness() {
  Serial.write(0xFE); // Control Character
  Serial.write(0x80); // Brightness
  delay(10);
}

void sendText(const char *msg) {
  clearScreen();
  for(int i=0; i<strlen(msg); i++) {
    Serial.write(msg[i]);
  }
  delay(50);
}

int getDuration() {
  int raw_value = analogRead(controlPin);
  float duty = (float) raw_value / 1024;
  int offset = (int) (duty*(durationHIGH-durationLOW));
  return durationLOW + offset;
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(relayPin1, LOW);
  digitalWrite(ledPin, LOW);
  lastPress = millis();
  Serial.begin(9600);
  delay(500);
  while(!Serial) {}; // Wait for serial to begin
  setBrightness();
  clearScreen();
}

void loop()
{
  duration = getDuration();
  const char durationText[16];
  sprintf(durationText, "%i ms", duration);
  sendText(durationText);

  switch (state)
  {
    case 0:  // waiting for button press
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW && ((millis()-lastPress) > debounce))
      {
        digitalWrite(ledPin, HIGH);  // turn on the LED
        digitalWrite(relayPin1, HIGH);  // turn on relay1
        startTime = millis();
        state = 1;
      }
      break;

    case 1:
      if (millis() - startTime >= duration)  // Debounce the button press
      {
        digitalWrite(relayPin1, LOW);  // turn off relay 1
        digitalWrite(ledPin, LOW);  // turn on the LED
        lastPress = millis();
        state = 0;
      }
      break;
  }
}
