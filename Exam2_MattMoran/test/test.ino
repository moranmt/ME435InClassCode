#define YELLOW_LED_PORT PORTD
#define YELLOW_LED_BIT 7
#define GREEN_LED_PORT PORTD
#define GREEN_LED_BIT 6

#define YELLOW_BUTTON_REGPORT PORTD
#define YELLOW_BUTTON_BIT 3
#define YELLOW_BUTTON_PINREG PIND
#define GREEN_BUTTON_REGPORT PORTD
#define GREEN_BUTTON_BIT 2
#define GREEN_BUTTON_PINREG PIND
#define BLUE_BUTTON_REGPORT PORTD
#define BLUE_BUTTON_BIT 4
#define BLUE_BUTTON_PINREG PIND


/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define GREEN_BUTTON_FLAG 0x01
#define YELLOW_BUTTON_FLAG 0x02
#define Flag_BUTTON_BLUE  0x04

volatile uint8_t portdhistory = 0xFF;



void setup() {
  // put your setup code here, to run once:

  DDRD = _BV(GREEN_LED_BIT) | _BV(YELLOW_LED_BIT);
  
  YELLOW_BUTTON_REGPORT |= _BV(YELLOW_BUTTON_BIT);
  GREEN_BUTTON_REGPORT |= _BV(GREEN_BUTTON_BIT);
  BLUE_BUTTON_REGPORT |= _BV(BLUE_BUTTON_BIT);

  PCICR = _BV(PCIE2);   //enable pin change innerupt for port D
  PCMSK2 = _BV(PCINT20) | _BV(PCINT19) | _BV(PCINT18);
  sei();                        // Enable global interrupts

  
  GREEN_LED_PORT |= _BV(GREEN_LED_BIT);
  YELLOW_LED_PORT |= _BV(YELLOW_LED_BIT);
  delay(100);
  GREEN_LED_PORT &= ~_BV(GREEN_LED_BIT);
  YELLOW_LED_PORT &= ~_BV(YELLOW_LED_BIT);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  
if (mainEventFlags & GREEN_BUTTON_FLAG) {
    delay(20);
    mainEventFlags &= ~GREEN_BUTTON_FLAG;
    if(bit_is_clear(GREEN_BUTTON_PINREG,GREEN_BUTTON_BIT)){
      // do stuff
        GREEN_LED_PORT ^= _BV(GREEN_LED_BIT);    
    }
  }
  if (mainEventFlags & YELLOW_BUTTON_FLAG) {
    delay(20);
    mainEventFlags &= ~YELLOW_BUTTON_FLAG;
    if(bit_is_clear(YELLOW_BUTTON_PINREG,YELLOW_BUTTON_BIT)){
      // do stuff
      YELLOW_LED_PORT ^= _BV(YELLOW_LED_BIT);   
    }
  }
  if (mainEventFlags & Flag_BUTTON_BLUE) {
    delay(20);
    mainEventFlags &= ~Flag_BUTTON_BLUE;
    if(bit_is_clear(BLUE_BUTTON_PINREG,BLUE_BUTTON_BIT)){
      // do stuff
        GREEN_LED_PORT ^= _BV(GREEN_LED_BIT); 
        YELLOW_LED_PORT ^= _BV(YELLOW_LED_BIT);   
    }
  }

}

ISR(PCINT2_vect){
  uint8_t changedBits;
  changedBits = PIND ^ portdhistory;
  portdhistory = PIND;

  if(changedBits & _BV(YELLOW_BUTTON_BIT)){
    if(bit_is_clear(YELLOW_BUTTON_PINREG,YELLOW_BUTTON_BIT)){
      mainEventFlags |= YELLOW_BUTTON_FLAG;
    }
  }
  if(changedBits & _BV(GREEN_BUTTON_BIT)){
   if(bit_is_clear(GREEN_BUTTON_PINREG,GREEN_BUTTON_BIT)){
      mainEventFlags |= GREEN_BUTTON_FLAG;
    } 
  }
  if(changedBits & _BV(BLUE_BUTTON_BIT)){
    if(bit_is_clear(BLUE_BUTTON_PINREG,BLUE_BUTTON_BIT)){
      mainEventFlags |= Flag_BUTTON_BLUE;
    }
  }
}
