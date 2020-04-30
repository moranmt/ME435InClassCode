#define	RED_LED				10
#define	YELLOW_LED			9
#define	GREEN_LED			6
#define BLUE_LED			5

#define BUTTON_RED 			3
#define BUTTON_YELLOW 		2
#define BUTTON_GREEN 		1
#define BUTTON_BLUE 		0


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

uint8_t typeLED[4] = {RED_LED,
                      YELLOW_LED,
                      GREEN_LED,
                      BLUE_LED};  
  


volatile uint32_t	MainEventFlags = 0;
#define Flag_BUTTON_YELLOW	0x01
#define Flag_BUTTON_RED		0x02

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(BUTTON_YELLOW), yellow_ButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_RED), red_ButtonISR, FALLING);
  
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
    if (!digitalRead(BUTTON_RED)){
      //Do the action
      addLED(RED_LED);
  	}
  }
  
  
  if(MainEventFlags & Flag_BUTTON_YELLOW){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_YELLOW;
    if (!digitalRead(BUTTON_YELLOW)){
      //Do the action
      addLED(YELLOW_LED);
  	}
  }
  
  
  
  BlueState = digitalRead(BUTTON_BLUE);
  GreenState = digitalRead(BUTTON_GREEN);
  
  if (BlueState != lastBlueState) {
    
    if (!BlueState) {
      for (int i = 0; i < sizeof(savedLEDs); i++){
        digitalWrite(savedLEDs[i], !digitalRead(savedLEDs[i]));
        delay(500);
        digitalWrite(savedLEDs[i], !digitalRead(savedLEDs[i]));
        delay(500);
        savedLEDs[i] = BLUE_LED;
      }	
      LEDindex = 0;
    } 
        delay(50);
  }
  
  if (GreenState != lastGreenState) {
    
    if (!GreenState) {
      //button HIGH
     	addLED(GREEN_LED);
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


void yellow_ButtonISR(){
  MainEventFlags |= Flag_BUTTON_YELLOW;
  
}

void red_ButtonISR(){
  MainEventFlags |= Flag_BUTTON_RED;
}