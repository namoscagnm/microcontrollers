
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void start_pwm(double dutyCycle) {
  /*
   * This generates PWM signal. Its will be on PORTD6, which is linked to OCR0A.
   * It seems that 0A imposes specific Timer Control Register
   */

  // Will blink on Port B1
  DDRD = 1 << PORTD6;

  // Sets Timer/Counter Control Register at to generate fast PWM
  TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

  // Prepares to create an interrupt if time matches
  TIMSK0 = 1 << TOIE0;
  // Turns off power after reaching 50 pct
  sei();

  OCR0A = dutyCycle * 255;
  // Sets pre-scaler to /1024
  TCCR0B = (1 << CS02) | (1 << CS00);

  /// Period is 16.32ms with 16/1024 Mhz, 8bit counter
  //
}

ISR(TIMER0_OVF_vect) {}

int main() {

  double dutyCycle = 0.2;
  start_pwm(dutyCycle);

  while (1) {
    _delay_ms(100);
    dutyCycle += 0.005;
    if (dutyCycle >= 1) {
      break;
    }
    start_pwm(dutyCycle);
  }
  start_pwm(0.2);
}
