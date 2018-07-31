const byte ledPin = 13;
const byte relayPin1 = 10;
const byte controlPin = A0;
const byte buttonPin = A1;
const unsigned long duration = 80;
const unsigned long debounce = 1000;
const char splashText[16] = "Spot Welder 1.0";
const int durationLOW = 10;
const int durationHIGH = 250;

volatile byte buttonState = HIGH;
volatile unsigned long lastPress;
volatile unsigned long startTime;
volatile int state = 0;

void clearScreen() {
  Serial.write(0xFE); // Control Character
  Serial.write(0x01); // Clear Display
}

void enableBacklight() {
  Serial.write(0xFE); // Control Character
  Serial.write(0x9D); // Backlight Full On
}

void sendText(const char *msg) {
  clearScreen();
  for(int i=0; i<strlen(msg); i++) {
    Serial.write(msg[i]);
  }
}

void splashScreen() {
  sendText(splashText);
  delay(3000);
}

int getDuration() {
  int offset = (int) analogRead(controlPin)/1024*(durationLOW/durationHIGH);
  return durationHIGH - offset;
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
  while(!Serial) {}; // Wait for serial to begin
  enableBacklight();
  splashScreen();
}

void loop()
{
  const char durationText[16];
  sprintf(durationText, "%i ms", getDuration());
  sendText(durationText);
  delay(50);
  switch (state)
  {
    case 0:  //waiting for button press
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW && ((millis()-lastPress) > debounce))
      {
        digitalWrite(ledPin, HIGH);  //turn on the LED
        digitalWrite(relayPin1, HIGH);  //turn on relay1
        startTime = millis();
        lastPress = startTime;
        state = 1;
      }
      break;

    case 1:
      if (millis() - startTime >= duration)  // Debounce the button press
      {
        digitalWrite(relayPin1, LOW);  //turn off relay 1
        digitalWrite(ledPin, LOW);  //turn on the LED
        state = 0;
      }
      break;
  }
}
