#include <EnableInterrupt.h>
#include <LiquidCrystal.h>
#include <math.h>

/***  LCD ***/
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
#define LINE_1 0
#define LINE_2 1

#define YELLOW_LED    7
#define GREEN_LED     6
#define BLUE_LED      5

#define photo     A0
int photoValue = 0;

#define YELLOW_BUTTON 4
#define GREEN_BUTTON  3
#define BLUE_BUTTON   2

volatile uint32_t MainEventFlags = 0;
#define Flag_BUTTON_YELLOW	0x01
#define Flag_BUTTON_GREEN		0x02
#define Flag_BUTTON_BLUE	0x04

String inputString = "";         // a String to hold incoming data
volatile bool stringComplete = false;  // whether the string is complete

unsigned long priorTimeMs = 0;

unsigned long TimerMs = 0;
uint8_t isTimerRunning = 0;
double lastSendTime;

int difficulty = 1;
int player = 1;
int previousScore = -1;
int tempScore = 0;
String lastPrint = " ";
String toPrint;
String GUIprint;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,LINE_1);
  lcd.print("Score: ");
  
  
  inputString.reserve(200);
  
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  pinMode(YELLOW_BUTTON,INPUT_PULLUP);
  pinMode(GREEN_BUTTON,INPUT_PULLUP);
  pinMode(BLUE_BUTTON,INPUT_PULLUP);
  
    //NEED to call library before you can use these from greygnome external interrupt
  enableInterrupt(YELLOW_BUTTON, yellowButtonISR, FALLING);
  enableInterrupt(GREEN_BUTTON, greenButtonISR, FALLING);
  enableInterrupt(BLUE_BUTTON, blueButtonISR, FALLING);
  sei();                        // Enable global interrupts
  
  
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(100); 

  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  delay(100);

  Serial.println("READY");
}

void loop() {
  checkPhoto();
  if(MainEventFlags & Flag_BUTTON_YELLOW){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_YELLOW;
    if (!digitalRead(YELLOW_BUTTON)){
      Serial.println("PressedJump");
      digitalWrite(YELLOW_LED, HIGH);
      delay(100);
      digitalWrite(YELLOW_LED, LOW);
  	}
  }
  if(MainEventFlags & Flag_BUTTON_GREEN){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_GREEN;
    if (!digitalRead(GREEN_BUTTON)){
      Serial.println("PressedDuck");
      digitalWrite(GREEN_LED, HIGH);
      delay(100);
      digitalWrite(GREEN_LED, LOW);
  	}
  }
  if(MainEventFlags & Flag_BUTTON_BLUE){
  	delay(30);
    MainEventFlags &= ~Flag_BUTTON_BLUE;
    if (!digitalRead(BLUE_BUTTON)){
      Serial.println("PressedStart");
      isTimerRunning = !isTimerRunning;
      digitalWrite(BLUE_LED, HIGH);
      delay(100);
      digitalWrite(BLUE_LED, LOW);
  	}
  }
 

  if (stringComplete) {

    if(inputString.equalsIgnoreCase("Stop")){
      isTimerRunning = 0;
      previousScore = tempScore;    
      Serial.print("tempScore ");
      Serial.println(previousScore);
      TimerMs = 0;
      lcd.setCursor(0,LINE_1);
      lcd.print("Score:");
      lcd.print("      ");
      lcd.setCursor(0,LINE_2);
      lcd.print("      ");
      updateLCD();

      
      
    }
    else if(inputString.equalsIgnoreCase("Update on")){
      GUIprint = "on";
      checkPhoto();
    }
    else if(inputString.equalsIgnoreCase("Update off")){
      GUIprint = "off";
      checkPhoto();
    }
    else if(inputString.equalsIgnoreCase("Player1")){
      player = 1;
    }
    else if(inputString.equalsIgnoreCase("Player2")){
      player = 2;
    }
    inputString = "";
    stringComplete = false;
  }

  unsigned long currentTimeMs = millis();
  unsigned long elapsedTimeMs = currentTimeMs - priorTimeMs;
  priorTimeMs = currentTimeMs;
  if(isTimerRunning){
    TimerMs += elapsedTimeMs;
    tempScore = round(TimerMs / 100);
    double sendTime =   (TimerMs / 100)*.1;
    if(((TimerMs / 100 % 10) / 5) == 1 || ((TimerMs / 100 % 10) / 5) == 0){
      if(sendTime != lastSendTime){
        if((TimerMs/ 100 % 10) == 5 || (TimerMs/ 100 % 10) == 0){
          if(difficulty == 1){
            Serial.print("Score ");
            Serial.println(tempScore);
            delay(30);
          }
          
        }
        if(difficulty == 2){
          Serial.print("Score ");
          Serial.println(tempScore);
          delay(30);
        }

      }

      lastSendTime = sendTime;
      
    }
  }
  updateLCD();
    
}



void updateLCD(){
  lcd.setCursor(0,LINE_1);
  lcd.print("Score:");
  lcd.print((TimerMs / 100));
  if(previousScore > -1){
    lcd.setCursor(0,LINE_2);
    lcd.print("Prev:");
    lcd.print(previousScore);
  }


}


void checkPhoto(){
    photoValue = analogRead(photo);
    if( photoValue < 500){
      toPrint = "Light";
    }
    else{
      toPrint = "Dark";
    }
    if(toPrint != lastPrint){

        Serial.println(toPrint);
        delay(100);
        lastPrint = toPrint;
      

    }
    if(GUIprint == "off"){
        if(toPrint == "Dark"){
          Serial.println(toPrint);
          delay(100);
          lastPrint = toPrint;
        }
    
    }
    else if(GUIprint == "on"){
      if(toPrint == "Light"){
          Serial.println(toPrint);
          delay(100);
          lastPrint = toPrint;
      }
    }
    
    
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }else{
      inputString += inChar;
    }
  }
}

void yellowButtonISR(){
   MainEventFlags |= Flag_BUTTON_YELLOW;
}
void greenButtonISR(){
  MainEventFlags |= Flag_BUTTON_GREEN;
}
void blueButtonISR(){
  MainEventFlags |= Flag_BUTTON_BLUE;
}
