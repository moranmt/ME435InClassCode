#include <LiquidCrystal.h>

/***  LCD ***/
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
#define LINE_1 0
#define LINE_2 1

#define GREEN_BUTTON 2
#define YELLOW_BUTTON 3
#define BLUE_BUTTON 4

#define GREEN_LED 6
#define YELLOW_LED 7
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

unsigned long yellowTimerTenths = 0;
uint8_t isyellowTimerRunning = 0;

unsigned long greenTimerTenths = 0;
uint8_t isgreenTimerRunning = 0;

//unsigned long priorTimeMs = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(GREEN_BUTTON), green_pushbutton_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(YELLOW_BUTTON), yellow_pushbutton_isr, FALLING);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);


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

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  delay(100);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  
  
}

void loop() {

  if (mainEventFlags & GREEN_BUTTON_FLAG) {
    delay(20);
    mainEventFlags &= ~GREEN_BUTTON_FLAG;
    if (!digitalRead(GREEN_BUTTON)) {
      // do stuff
      isgreenTimerRunning = !isgreenTimerRunning;
      digitalWrite(GREEN_LED, isgreenTimerRunning);      
    }
  }
  if (mainEventFlags & YELLOW_BUTTON_FLAG) {
    delay(20);
    mainEventFlags &= ~YELLOW_BUTTON_FLAG;
    if (!digitalRead(YELLOW_BUTTON)) {
      // do stuff
      isyellowTimerRunning = !isyellowTimerRunning;
      digitalWrite(YELLOW_LED, isyellowTimerRunning);
    }
  }
  if (!digitalRead(BLUE_BUTTON)) {
    // do stuff
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    lcd.clear();
    isyellowTimerRunning = 0;
    yellowTimerTenths = 0;
    isgreenTimerRunning = 0;
    greenTimerTenths = 0;
  }
  
/*
  unsigned long currentTimeMs = millis();
  unsigned long elapsedTimeMs = currentTimeMs - priorTimeMs;
  priorTimeMs = currentTimeMs;
  if(isyellowTimerRunning){
    yellowTimerMs += elapsedTimeMs;
    
  }
  if(isgreenTimerRunning){
    greenTimerMs += elapsedTimeMs;
    
  }
  */
  updateLCD();  
  //delay(10);
}


// Simple ISRs that set flags only
void green_pushbutton_isr() {
  mainEventFlags |= GREEN_BUTTON_FLAG;
}

void yellow_pushbutton_isr() {
  mainEventFlags |= YELLOW_BUTTON_FLAG;
}

void updateLCD(){
  lcd.setCursor(0,LINE_1);
  lcd.print(yellowTimerTenths / 10);
  lcd.print(".");
  lcd.print(yellowTimerTenths % 10);
  
  lcd.setCursor(0,LINE_2);
  lcd.print(greenTimerTenths / 1000);
  lcd.print(".");
  lcd.print(greenTimerTenths / 100 % 10);
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
}
