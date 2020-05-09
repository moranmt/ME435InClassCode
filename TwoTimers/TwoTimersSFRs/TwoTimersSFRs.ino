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


/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define GREEN_BUTTON_FLAG 0x01
#define YELLOW_BUTTON_FLAG 0x02

unsigned long yellowTimerMs = 0;
uint8_t isyellowTimerRunning = 0;

unsigned long greenTimerMs = 0;
uint8_t isgreenTimerRunning = 0;

unsigned long priorTimeMs = 0;

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
    yellowTimerMs = 0;
    isgreenTimerRunning = 0;
    greenTimerMs = 0;
  }
  

  unsigned long currentTimeMs = millis();
  unsigned long elapsedTimeMs = currentTimeMs - priorTimeMs;
  priorTimeMs = currentTimeMs;
  if(isyellowTimerRunning){
    yellowTimerMs += elapsedTimeMs;
    
  }
  if(isgreenTimerRunning){
    greenTimerMs += elapsedTimeMs;
    
  }
  updateLCD();  
  delay(10);
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
  lcd.print(yellowTimerMs / 1000);
  lcd.print(".");
  lcd.print(yellowTimerMs / 100 % 10);
  
  lcd.setCursor(0,LINE_2);
  lcd.print(greenTimerMs / 1000);
  lcd.print(".");
  lcd.print(greenTimerMs / 100 % 10);
}
