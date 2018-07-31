const byte ledPin = 13;
const byte relayPin1 = 10;
const byte buttonPin = A1;
const unsigned long duration = 80;
const unsigned long debounce = 1000;

volatile byte buttonState = HIGH;
volatile unsigned long lastPress;
volatile unsigned long startTime;
volatile int state = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(relayPin1, LOW);
  digitalWrite(ledPin, LOW);
  lastPress = millis();
}

void loop()
{
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
