//output ports and bits
#define RED_LED_PORT PORTB
#define RED_LED_BIT 2
#define YELLOW_LED_PORT PORTB
#define YELLOW_LED_BIT 1
#define GREEN_LED_PORT PORTD
#define GREEN_LED_BIT 6
#define BLUE_LED_PORT PORTD
#define BLUE_LED_BIT 5

//input port, pin reg, bits
#define RED_BUTTON_REGPORT PORTD
#define RED_BUTTON_BIT 3
#define RED_BUTTON_PINREG PIND
#define YELLOW_BUTTON_REGPORT PORTD
#define YELLOW_BUTTON_BIT 2
#define YELLOW_BUTTON_PINREG PIND
#define GREEN_BUTTON_REGPORT PORTD
#define GREEN_BUTTON_BIT 1
#define GREEN_BUTTON_PINREG PIND
#define BLUE_BUTTON_REGPORT PORTD
#define BLUE_BUTTON_BIT 0
#define BLUE_BUTTON_PINREG PIND

#define PRESSED				0
#define UNPRESSED			1

uint8_t	 BlueState = HIGH;			
uint8_t  lastBlueState = HIGH;
uint8_t	 GreenState = HIGH;			
uint8_t  lastGreenState = HIGH;

int LEDindex = 0;
uint8_t savedLEDs[10] = {BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT,
                         BLUE_LED_BIT};

  


volatile uint32_t	MainEventFlags = 0;
#define Flag_BUTTON_RED		0x01
#define Flag_BUTTON_YELLOW	0x02
#define Flag_BUTTON_GREEN		0x04
#define Flag_BUTTON_BLUE	0x08



volatile uint8_t portdhistory = 0xFF;

void setup()
{
  //pinModes outputs
  DDRB = _BV(RED_LED_BIT) | _BV(YELLOW_LED_BIT);  //red yellow
  DDRD = _BV(GREEN_LED_BIT) | _BV(BLUE_LED_BIT); //green blue
 //all pins set as inputs if not outputs 
 //enable pullup resistors
  RED_BUTTON_REGPORT |= _BV(RED_LED_BIT);
  YELLOW_BUTTON_REGPORT |= _BV(YELLOW_LED_BIT);
  GREEN_BUTTON_REGPORT |= _BV(GREEN_LED_BIT);
  BLUE_BUTTON_REGPORT |= _BV(BLUE_LED_BIT);

  
  
  
  //attachInterrupt(digitalPinToInterrupt(BUTTON_YELLOW), yellow_ButtonISR, FALLING);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_RED), red_ButtonISR, FALLING);
  //EICRA = 0x0A; //Set INT) & INT1 to falling interrupts
  //EICRA = _BV(ISC11) | _BV(ISC01); //Set INT) & INT1 to falling interrupts
  //EIMSK = 0x03; //turrns on INT1 and INT0
  //EIMSK = _BV(INT0) | _BV(INT1);
  
  PCICR = _BV(PCIE2);   //enable pin change innerupt for port D
  //PCMSK2 = 0x0F;    //RD0-RD3 intterupts
  PCMSK2 = _BV(PCINT19) | _BV(PCINT18) | _BV(PCINT17) | _BV(PCINT16);
  sei();    //turn on interrupts globaly

  GREEN_LED_PORT |= _BV(GREEN_LED_BIT);
  RED_LED_PORT |= _BV(RED_LED_BIT);
  YELLOW_LED_PORT |= _BV(YELLOW_LED_BIT);
  BLUE_LED_PORT |= _BV(BLUE_LED_BIT);
  delay(100);
  GREEN_LED_PORT &= ~_BV(GREEN_LED_BIT);
  RED_LED_PORT &= ~_BV(RED_LED_BIT);
  YELLOW_LED_PORT &= ~_BV(YELLOW_LED_BIT);
  BLUE_LED_PORT &= ~_BV(BLUE_LED_BIT);
  delay(100);
  
  
}

void showFeedback(){
  if(bit_is_clear(RED_BUTTON_PINREG,RED_BUTTON_BIT)){
    RED_LED_PORT |= _BV(RED_LED_BIT);
  }else{
    RED_LED_PORT &= ~_BV(RED_LED_BIT);
  }
  if(bit_is_clear(YELLOW_BUTTON_PINREG,YELLOW_BUTTON_BIT)){
    YELLOW_LED_PORT |= _BV(YELLOW_LED_BIT);
  }else{
    YELLOW_LED_PORT &= ~_BV(YELLOW_LED_BIT);
  }
  if(bit_is_clear(GREEN_BUTTON_PINREG,GREEN_BUTTON_BIT)){
    GREEN_LED_PORT |= _BV(GREEN_LED_BIT);
  }else{
    GREEN_LED_PORT &= ~_BV(GREEN_LED_BIT);
  }
  if(bit_is_clear(BLUE_BUTTON_PINREG,BLUE_BUTTON_BIT)){
    BLUE_LED_PORT |= _BV(BLUE_LED_BIT);
  }else{
    BLUE_LED_PORT &= ~_BV(BLUE_LED_BIT);
  }
  delay(50);
}

void oldtemptestloop(){
//temp loop
  RED_LED_PORT |= _BV(RED_LED_BIT);
  YELLOW_LED_PORT |= _BV(YELLOW_LED_BIT);
  GREEN_LED_PORT |= _BV(GREEN_LED_BIT);
  BLUE_LED_PORT |= _BV(BLUE_LED_BIT);
  delay(1000);
  RED_LED_PORT &= ~_BV(RED_LED_BIT);
  YELLOW_LED_PORT &= ~_BV(YELLOW_LED_BIT);
  GREEN_LED_PORT &= ~_BV(GREEN_LED_BIT);
  BLUE_LED_PORT &= ~_BV(BLUE_LED_BIT);
  delay(1000);
}

void loop()
{
  
  //showFeedback();
  
  
  
  if(MainEventFlags & Flag_BUTTON_RED){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_RED;
    if(bit_is_clear(RED_BUTTON_PINREG,RED_BUTTON_BIT)){
      //Do the action
      addLED(RED_LED_BIT);
  	}
  }
  
  
  if(MainEventFlags & Flag_BUTTON_YELLOW){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_YELLOW;
    if(bit_is_clear(YELLOW_BUTTON_PINREG, YELLOW_BUTTON_BIT)){
      //Do the action
      addLED(YELLOW_LED_BIT);
  	}
  }
  if(MainEventFlags & Flag_BUTTON_GREEN){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_GREEN;
    if(bit_is_clear(GREEN_BUTTON_PINREG, GREEN_BUTTON_BIT)){
      //Do the action
      addLED(GREEN_LED_BIT);
  	}
  }

  if(MainEventFlags & Flag_BUTTON_BLUE){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_BLUE;
    if(bit_is_clear(BLUE_BUTTON_PINREG, BLUE_BUTTON_BIT)){
      //Do the action
      runLEDs(); //old code previously here being moved to a function
      //addLED(YELLOW_LED);
  	}
  }
  
  
}

void runLEDs(){
  for (int i = 0; i < sizeof(savedLEDs); i++){
    uint8_t activeLED = savedLEDs[i];
    if(activeLED == RED_LED_BIT || activeLED == YELLOW_LED_BIT){
      RED_LED_PORT |= _BV(activeLED);
    }else{
      GREEN_LED_PORT |= _BV(activeLED);
    }
    delay(500);
    if(activeLED == RED_LED_BIT || activeLED == YELLOW_LED_BIT){
      RED_LED_PORT &= ~_BV(activeLED);
    }else{
      GREEN_LED_PORT &= ~_BV(activeLED);
    }
    delay(500);
    savedLEDs[i] = BLUE_LED_BIT;
  }	
  LEDindex = 0;
}

void addLED(uint8_t LEDtoAdd){
  if(LEDindex < sizeof(savedLEDs)){
    savedLEDs[LEDindex] = LEDtoAdd;
  	LEDindex += 1;
  }

}



ISR(PCINT2_vect){
  uint8_t changedBits;
  changedBits = PIND ^ portdhistory;
  portdhistory = PIND;

  if(changedBits & _BV(RED_BUTTON_BIT)){
    if(bit_is_clear(RED_BUTTON_PINREG,RED_BUTTON_BIT)){
      MainEventFlags |= Flag_BUTTON_RED;
    }
  }
  if(changedBits & _BV(YELLOW_BUTTON_BIT)){
    if(bit_is_clear(YELLOW_BUTTON_PINREG,YELLOW_BUTTON_BIT)){
      MainEventFlags |= Flag_BUTTON_YELLOW;
    }
  }
  if(changedBits & _BV(GREEN_BUTTON_BIT)){
   if(bit_is_clear(GREEN_BUTTON_PINREG,GREEN_BUTTON_BIT)){
      MainEventFlags |= Flag_BUTTON_GREEN;
    } 
  }
  if(changedBits & _BV(BLUE_BUTTON_BIT)){
    if(bit_is_clear(BLUE_BUTTON_PINREG,BLUE_BUTTON_BIT)){
      MainEventFlags |= Flag_BUTTON_BLUE;
    }
  }

}