#define	RED_LED				10
#define	YELLOW_LED			9
#define	GREEN_LED			6
#define BLUE_LED			5

#define BUTTON_RED 			3
#define BUTTON_YELLOW 		2
#define BUTTON_GREEN 		1
#define BUTTON_BLUE 		0

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
uint8_t savedLEDs[10] = {BLUE_LED,
                         BLUE_LED,
                         BLUE_LED,
                         BLUE_LED,
                         BLUE_LED,
                         BLUE_LED,
                         BLUE_LED,
                         BLUE_LED,
                         BLUE_LED,
                         BLUE_LED};

  


volatile uint32_t	MainEventFlags = 0;
#define Flag_BUTTON_YELLOW	0x01
#define Flag_BUTTON_RED		0x02

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
  EICRA = _BV(ISC11) | _BV(ISC01); //Set INT) & INT1 to falling interrupts
  //EIMSK = 0x03; //turrns on INT1 and INT0
  EIMSK = _BV(INT0) | _BV(INT1);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(100);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  delay(100);
  
  
}

void showFeedbackLEDsloop(){
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
  
/*  for sequencing
  digitalWrite(RED_LED, !digitalRead(BUTTON_RED));
  digitalWrite(GREEN_LED, !digitalRead(BUTTON_GREEN));
  digitalWrite(YELLOW_LED, !digitalRead(BUTTON_YELLOW));
  digitalWrite(BLUE_LED, !digitalRead(BUTTON_BLUE));
  
  */
  
  
  
  if(MainEventFlags & Flag_BUTTON_RED){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_RED;
    if(bit_is_clear(RED_BUTTON_PINREG,RED_BUTTON_BIT)){
      //Do the action
      RED_LED_PORT ^= _BV(RED_LED_BIT); //toggle red led
      //addLED(RED_LED);
  	}
  }
  
  
  if(MainEventFlags & Flag_BUTTON_YELLOW){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_YELLOW;
    if(bit_is_clear(YELLOW_BUTTON_PINREG, YELLOW_BUTTON_BIT)){
      //Do the action
      YELLOW_LED_PORT ^= _BV(YELLOW_LED_BIT);
      //addLED(YELLOW_LED);
  	}
  }
  
  
  
  //BlueState = digitalRead(BUTTON_BLUE);
  //GreenState = digitalRead(BUTTON_GREEN);
  BlueState = bit_is_set(BLUE_BUTTON_PINREG, BLUE_BUTTON_BIT);
  GreenState = bit_is_set(GREEN_BUTTON_PINREG, GREEN_BUTTON_BIT);

  if (BlueState != lastBlueState) {
    
    if (!BlueState) {

      BLUE_LED_PORT ^= _BV(BLUE_LED_BIT);
      /*
      for (int i = 0; i < sizeof(savedLEDs); i++){
        digitalWrite(savedLEDs[i], !digitalRead(savedLEDs[i]));
        delay(500);
        digitalWrite(savedLEDs[i], !digitalRead(savedLEDs[i]));
        delay(500);
        savedLEDs[i] = BLUE_LED;
      }	
      LEDindex = 0;
      */

    } 
        delay(50);
  }
  
  if (GreenState != lastGreenState) {
    
    if (!GreenState) {
      //button HIGH
      GREEN_LED_PORT ^= _BV(GREEN_LED_BIT);
     	//addLED(GREEN_LED);
    } 
        delay(50);
  }
  
  lastBlueState = BlueState;
  lastGreenState = GreenState;
}


void addLED(uint8_t LEDtoAdd){
  if(LEDindex <= 9){
    savedLEDs[LEDindex] = LEDtoAdd;
  	LEDindex += 1;
  }

}


//void yellow_ButtonISR(){
ISR(INT0_vect){
  MainEventFlags |= Flag_BUTTON_YELLOW;
  
}

//void red_ButtonISR(){
ISR(INT1_vect){
  MainEventFlags |= Flag_BUTTON_RED;
}