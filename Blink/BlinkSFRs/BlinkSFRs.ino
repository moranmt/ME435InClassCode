#define REG_PORT_LED_BUILTIN  PORTB
#define BIT_LED_BUILTIN       5


// Counter and compare values
#define TIMER1_START  0
#define TIMER1_COMPARE  31250




void setup() {

  
  DDRB |= _BV(BIT_LED_BUILTIN); // Set LED pin to be output
  TCCR1A = 0;                   // Reset Timer1 Control Reg A
  TCCR1B |= _BV(CS12);          // Set to prescaler of 256
  TCCR1B &= ~_BV(CS11);
  TCCR1B &= ~_BV(CS10);
  TCNT1 = TIMER1_START;         // Reset Timer1 and set compare value
  OCR1A = TIMER1_COMPARE;
  TIMSK1 = _BV(OCIE1A);         // Enable Timer1 overflow interrupt
  sei();                        // Enable global interrupts
}

void loop() {
  delay(500);
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = TIMER1_START;
  REG_PORT_LED_BUILTIN ^= _BV(BIT_LED_BUILTIN);
}