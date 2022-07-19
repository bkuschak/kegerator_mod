// Workaround to prevent short cycling on the Komos V2 kegerator.
// For some unknown reason the temperature controller randomly
// turns the compressor on for 1 second, then shuts it back off.
// This happens several times per hour. It tends to happen more
// often when the temperature setpoint is above 45 deg F.
// 
// This software implements a workaround using a small MCU inline
// on the compressor enable signal, to filter out these transients
// and enforce minimum on-time and minimum off-time for the 
// compressor.
//
// Device:
// ATTINY212 8-pin package.
// 5V operation. 1 MHz internal osc.
// ULPI programming using PA0.
// I/O using PA2, PA1.
// 
// https://github.com/bkuschak/kegerator_mod

#include <arduino-timer.h>

#define COMPRESSOR_INPUT_PIN    2   // PA1 - pin 4
#define COMPRESSOR_OUTPUT_PIN   3   // PA2 - pin 5
#define FAN_PWM_OUTPUT_PIN      1   // PA7 - pin 3
#define FAN_PWM_INPUT_PIN       4   // PA3 - pin 7
#define DEBUG_PIN               0   // PA6 - pin 2

// If input is asserted for > 5 seconds then turn the output on.
#define ON_DELAY 5

// Once output is turned on, keep it on for at least 60 seconds.
#define MIN_ON_TIME 60

// If input is deasserted for > 5 seconds then goto OFF state.
#define OFF_DELAY 5

// Once output is turned off, keep it on for at least 120 seconds.
#define MIN_OFF_TIME 120

// TODO - 10 HZ PWM for fan on PA7 using TCD0/WOB. Using PA3 to detect FAN_PWM_IN

enum state_e { OFF = 0, ON } state;
uint16_t timeout;
uint16_t counter;
Timer<1> timer;   // msec timer resolution.

inline void compressor_on() {
  digitalWrite(COMPRESSOR_OUTPUT_PIN, HIGH);
}

inline void compressor_off() {
  digitalWrite(COMPRESSOR_OUTPUT_PIN, LOW);
}

// Call this function once per second, using the timer.
bool fsm(void* none) {

  // Toggle the debug pin.
  digitalWrite(DEBUG_PIN, !digitalRead(DEBUG_PIN)); 
  
  // Update timeout.
  if (timeout > 0)
    timeout--;

  switch (state) {
    case OFF:
      // Keep track of how long input has been continously asserted.
      if (digitalRead(COMPRESSOR_INPUT_PIN) == HIGH)
        counter++;
      else
        counter = 0;

      // Keep off for minimum amount of time.
      // Afterwards, turn on if the input has been continously
      // asserted for ON_DELAY seconds.
      if (timeout == 0) {
        if (counter >= ON_DELAY) {
          compressor_on();
          timeout = MIN_ON_TIME;
          counter = 0;
          state = ON;
        }
      }
      break;

    case ON:
      // Keep track of how long input has been continously deasserted.
      if (digitalRead(COMPRESSOR_INPUT_PIN) == LOW)
        counter++;
      else
        counter = 0;

      // Keep on for minimum amount of time.
      // Afterwards, turn off if the input has been continously
      // deasserted for OFF_DELAY seconds.
      if (timeout == 0) {
        if (counter >= OFF_DELAY) {
          compressor_off();
          counter = 0;
          timeout = MIN_OFF_TIME;
          state = OFF;
        }
      }
      break;
  }
  return true;    // run again next time
}

void setup() {
  pinMode(COMPRESSOR_INPUT_PIN, INPUT_PULLUP);
  pinMode(FAN_PWM_INPUT_PIN, INPUT);
  pinMode(FAN_PWM_OUTPUT_PIN, INPUT);   // Ignored for now.
  pinMode(DEBUG_PIN, OUTPUT);
  digitalWrite(COMPRESSOR_OUTPUT_PIN, LOW);
  pinMode(COMPRESSOR_OUTPUT_PIN, OUTPUT);
  digitalWrite(DEBUG_PIN, LOW);
  pinMode(DEBUG_PIN, OUTPUT);
  
  state = OFF;
  timeout = 0;
  counter = 0;

  // Run the FSM once per second.
  timer.every(1000, fsm);
}

void loop() {
  timer.tick();
}
