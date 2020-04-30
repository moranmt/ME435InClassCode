//#define	RED_LED		10
//#define	YELLOW_LED	5
//#define	GREEN_LED	6

#define BUTTON 0

#define PRESSED 0
#define UNPRESSED 1

void setup() {
  //pinMode(13, OUTPUT);

  DDRB |= _BV(DDB2);  //replaces pinMode(RED_LED, OUTPUT);
  //pinMode(YELLOW_LED, OUTPUT);
  DDRB |= _BV(DDB1);  //replaces pinMode(YELLOW_LED, OUTPUT);
  //pinMode(GREEN_LED, OUTPUT);
  DDRD |= _BV(DDD6);  //replaces pinMode(GREEN_LED, OUTPUT);

  //pinMode(BUTTON, INPUT_PULLUP);
  DDRD &= ~_BV(DDD0);
  PORTD |= _BV(DDD0);

  //digitalWrite(GREEN_LED, HIGH);
  PORTD |= _BV(PORTD6);  //replaces digitalWrite(GREEN_LED, HIGH);
  //digitalWrite(RED_LED, HIGH);
  PORTB |= _BV(PORTB2);  // replaces digitalWrite(RED_LED, HIGH);
  //digitalWrite(YELLOW_LED, HIGH);
  PORTB |= _BV(PORTB1);  // replaces digitalWrite(YELLOW_LED, HIGH);
  delay(100);
  //digitalWrite(RED_LED, LOW);
  PORTB &= ~_BV(PORTB2);  //replaces digitalWrite(RED_LED, LOW); or use PORTB = PORTB & B11111011;
  //digitalWrite(GREEN_LED, LOW);
  PORTD &= ~_BV(PORTD6);  //replaces digitalWrite(GREEN_LED, LOW);
  //digitalWrite(YELLOW_LED, LOW);
  PORTB &= ~_BV(PORTB1);  // replaces digitalWrite(YELLOW_LED, LOW);
  delay(100);
}

void loop() {
  //if(digitalRead(BUTTON) == PRESSED)
  //if(!digitalRead(BUTTON))
  //if(!(PIND & _BV(PIND0)))
  if (bit_is_clear(PIND, PIND0)) {
    PORTD |= _BV(PORTD6);  //replaces digitalWrite(GREEN_LED, HIGH);

    delay(2000);
    PORTD &= ~_BV(PORTD6);  //replaces digitalWrite(GREEN_LED, LOW);
    PORTB |= _BV(PORTB1);   //replaces digitalWrite(YELLOW_LED, HIGH);
    delay(1000);
    PORTB &= ~_BV(PORTB1);  // replaces digitalWrite(YELLOW_LED, LOW);

    PORTB |= _BV(PORTB2);  // replaces digitalWrite(RED_LED, HIGH); or use PORTB = PORTB | B00000100;
    delay(5000);

    PORTB &= ~_BV(PORTB2);  //replaces digitalWrite(RED_LED, LOW); or use PORTB = PORTB & B11111011;

  } else {
    //digitalWrite(RED_LED, LOW);
    PORTB &= ~_BV(PORTB2);  //replaces digitalWrite(RED_LED, LOW); or use PORTB = PORTB & B11111011;
    //digitalWrite(GREEN_LED, LOW);
    PORTD &= ~_BV(PORTD6);  //replaces digitalWrite(GREEN_LED, LOW);
    //digitalWrite(YELLOW_LED, LOW);
    PORTB &= ~_BV(PORTB1);  // replaces digitalWrite(YELLOW_LED, LOW);
  }
}