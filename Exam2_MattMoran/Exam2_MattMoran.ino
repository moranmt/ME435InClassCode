#include <LiquidCrystal.h>

/***  LCD ***/
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
#define LINE_1 0
#define LINE_2 1

//input port, pin reg, bits
#define YELLOW_BUTTON_REGPORT PORTD
#define YELLOW_BUTTON_BIT 3
#define YELLOW_BUTTON_PINREG PIND
#define GREEN_BUTTON_REGPORT PORTD
#define GREEN_BUTTON_BIT 2
#define GREEN_BUTTON_PINREG PIND
#define BLUE_BUTTON_REGPORT PORTD
#define BLUE_BUTTON_BIT 4
#define BLUE_BUTTON_PINREG PIND

//output ports and bits
#define YELLOW_LED_PORT PORTD
#define YELLOW_LED_BIT 7
#define GREEN_LED_PORT PORTD
#define GREEN_LED_BIT 6

//timer 2 for green use 0.001s
//prescaler timer1, legal values 0-65536
//1-----  1,600,000
//8-----  200,000
//64----  25,000
//256---  6250
//1024--  1562.5
//prescaler timer2, legal values 0-256
//8-----  2000
//32----- 500
//64----  250


// Counter and compare values
#define TIMER1_START  0
#define TIMER1_COMPARE  25000

// Counter and compare values
#define TIMER2_START  0
#define TIMER2_COMPARE  250

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define GREEN_BUTTON_FLAG 0x01
#define YELLOW_BUTTON_FLAG 0x02
#define Flag_BUTTON_BLUE	0x04

volatile uint8_t portdhistory = 0xFF;

unsigned long yellowTimerTenths = 0;
uint8_t isyellowTimerRunning = 0;

unsigned long greenTimerTenths = 0;
uint8_t isgreenTimerRunning = 0;

unsigned long debounceTimerTenths = 0;
uint8_t isdebounceTimerRunning = 0;

int wait = 0;


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);


  YELLOW_BUTTON_REGPORT |= _BV(YELLOW_BUTTON_BIT);
  GREEN_BUTTON_REGPORT |= _BV(GREEN_BUTTON_BIT);
  BLUE_BUTTON_REGPORT |= _BV(BLUE_BUTTON_BIT);
 
  PCICR = _BV(PCIE2);   //enable pin change innerupt for port D
  PCMSK2 = _BV(PCINT20) | _BV(PCINT19) | _BV(PCINT18);

  DDRD = _BV(GREEN_LED_BIT) | _BV(YELLOW_LED_BIT);


  //Timer 1 set up
  TCCR1A = 0;                   // Reset Timer1 Control Reg A
  TCCR1B &= ~_BV(CS12);          // Set to prescaler of 64
  TCCR1B |= _BV(CS11);
  TCCR1B |= _BV(CS10);
  TCNT1 = TIMER1_START;         // Reset Timer1 and set compare value
  OCR1A = TIMER1_COMPARE;
  TIMSK1 = _BV(OCIE1A);         // Enable Timer1 overflow interrupt
  

  //Timer 2 set up
  TCCR2A = 0;                   // Reset Timer2 Control Reg A
  TCCR2B |= _BV(CS22);          // Set to prescaler of 64
  TCCR2B &= ~_BV(CS21);
  TCCR2B &= ~_BV(CS20);
  TCNT2 = TIMER2_START;         // Reset Timer1 and set compare value
  OCR2A = TIMER2_COMPARE;
  TIMSK2 = _BV(OCIE2A);         // Enable Timer1 overflow interrupt
 
  sei();                        // Enable global interrupts

  GREEN_LED_PORT |= _BV(GREEN_LED_BIT);
  YELLOW_LED_PORT |= _BV(YELLOW_LED_BIT);
  delay(100);
  GREEN_LED_PORT &= ~_BV(GREEN_LED_BIT);
  YELLOW_LED_PORT &= ~_BV(YELLOW_LED_BIT);
  delay(100);
  
  
}

void loop() {

  if (mainEventFlags & GREEN_BUTTON_FLAG) {
    delay(20);
    mainEventFlags &= ~GREEN_BUTTON_FLAG;
    if(bit_is_clear(GREEN_BUTTON_PINREG,GREEN_BUTTON_BIT)){
      // do stuff
      isgreenTimerRunning = !isgreenTimerRunning;
      if(isgreenTimerRunning == 1){
        GREEN_LED_PORT ^= _BV(GREEN_LED_BIT);
      } else{
        GREEN_LED_PORT &= ~_BV(GREEN_LED_BIT);
      } 
          
    }
  }
  if (mainEventFlags & YELLOW_BUTTON_FLAG) {
    delay(20);
    mainEventFlags &= ~YELLOW_BUTTON_FLAG;
    if(bit_is_clear(YELLOW_BUTTON_PINREG,YELLOW_BUTTON_BIT)){
      // do stuff
      isyellowTimerRunning = !isyellowTimerRunning;
      if(isyellowTimerRunning == 1){
        YELLOW_LED_PORT ^= _BV(YELLOW_LED_BIT);
      } else{
        YELLOW_LED_PORT &= ~_BV(YELLOW_LED_BIT);
      } 
    }
  }
  if (mainEventFlags & Flag_BUTTON_BLUE) {
    delay(20);
    mainEventFlags &= ~Flag_BUTTON_BLUE;
    if(bit_is_clear(BLUE_BUTTON_PINREG,BLUE_BUTTON_BIT)){
      // do stuff
      GREEN_LED_PORT &= ~_BV(GREEN_LED_BIT);
      YELLOW_LED_PORT &= ~_BV(YELLOW_LED_BIT);
      lcd.clear();
      isyellowTimerRunning = 0;
      yellowTimerTenths = 0;
      isgreenTimerRunning = 0;
      greenTimerTenths = 0;
    }
  }
  

  updateLCD();  
  
}

void debounce(int delayMS){
  isdebounceTimerRunning = !isdebounceTimerRunning;
  while(wait == 0){
    if((debounceTimerTenths) > delayMS){
      wait = 1;
      isdebounceTimerRunning = !isdebounceTimerRunning;
      debounceTimerTenths = 0;
    }
  }

  wait = 0;
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

void updateLCD(){
  lcd.setCursor(0,LINE_1);
  lcd.print(yellowTimerTenths / 10);
  lcd.print(".");
  lcd.print(yellowTimerTenths % 10);
  
  if(isgreenTimerRunning == 1){
    lcd.setCursor(0,LINE_2);
    lcd.print(" ");
    lcd.print(" ");
    lcd.print(" ");
    lcd.print(" ");
    lcd.print(" ");
  }else{
    lcd.setCursor(0,LINE_2);
    lcd.print(greenTimerTenths / 1000);
    lcd.print(".");
    lcd.print(greenTimerTenths / 100 % 10);
  }

}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = TIMER1_START;
    if(isyellowTimerRunning){
    yellowTimerTenths++;
    
  }
}

ISR(TIMER2_COMPA_vect) {
  TCNT2 = TIMER2_START;
  if(isgreenTimerRunning){
    greenTimerTenths++; 
  }
  if(isdebounceTimerRunning){
    debounceTimerTenths++; 
  }
}
