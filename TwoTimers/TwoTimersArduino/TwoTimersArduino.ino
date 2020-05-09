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
      Serial.println("Press green");
      greenCounter++;
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(YELLOW_LED, LOW);
    }
  }
  if (mainEventFlags & YELLOW_BUTTON_FLAG) {
    delay(20);
    mainEventFlags &= ~YELLOW_BUTTON_FLAG;
    if (!digitalRead(YELLOW_BUTTON)) {
      // do stuff
      Serial.println("Press yellow");
      yellowCounter++;
	  digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
    }
  }
  if (!digitalRead(BLUE_BUTTON)) {
      // do stuff
	  digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      greenCounter = 0;
      yellowCounter = 0;
  }
  delay(100);
}


// Simple ISRs that set flags only
void green_pushbutton_isr() {
  mainEventFlags |= GREEN_BUTTON_FLAG;
}

void yellow_pushbutton_isr() {
  mainEventFlags |= YELLOW_BUTTON_FLAG;
}

