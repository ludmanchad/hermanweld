#include "sensor.h"

int averageRead(const int pin) {
  int i=0;
  int sum = 0;
  char msg[48];
  while (i < READ_COUNT ) {
    int read = analogRead(pin);
    sprintf(msg, "PIN %i: %i", pin, read);
    sum += read;
    i++;
  }
  const float avg = (float) sum / (float) READ_COUNT;
  return ROUND(avg);
}

