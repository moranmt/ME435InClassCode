#include <EnableInterrupt.h>

#define RED_LED      10
#define YELLOW_LED    9
#define GREEN_LED   6
#define BLUE_LED    5

#define RGB_RED_LED   11
#define RGB_GREEN_LED 12
#define RGB_BLUE_LED  13

#define pot       A0
#define photo     A1

int potValue = 0;
int photoValue = 0;

#define RED_BUTTON    7
#define YELLOW_BUTTON 4
#define GREEN_BUTTON  3
#define BLUE_BUTTON   2

volatile uint32_t MainEventFlags = 0;
#define Flag_BUTTON_RED		0x01
#define Flag_BUTTON_YELLOW	0x02
#define Flag_BUTTON_GREEN		0x04
#define Flag_BUTTON_BLUE	0x08

int rgbCounter = 0;

String inputString = "";         // a String to hold incoming data
volatile bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  pinMode(RGB_RED_LED, OUTPUT);
  pinMode(RGB_GREEN_LED, OUTPUT);
  pinMode(RGB_BLUE_LED, OUTPUT);
  
  pinMode(RED_BUTTON,INPUT_PULLUP);
  pinMode(YELLOW_BUTTON,INPUT_PULLUP);
  pinMode(GREEN_BUTTON,INPUT_PULLUP);
  pinMode(BLUE_BUTTON,INPUT_PULLUP);
  
  enableInterrupt(RED_BUTTON, redButtonISR, FALLING);       //NEED to call library before you can use these from greygnome external interrupt
  enableInterrupt(YELLOW_BUTTON, yellowButtonISR, FALLING);
  enableInterrupt(GREEN_BUTTON, greenButtonISR, FALLING);
  enableInterrupt(BLUE_BUTTON, blueButtonISR, FALLING);
  
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(100); 
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  delay(100);

  Serial.println("READY");
}

void loop() {

  potValue = analogRead(pot);
  photoValue = analogRead(photo);

  if (stringComplete) {

    if(inputString.equalsIgnoreCase("Update")){
      for(int k = 0; k < 5; k++){
        digitalWrite(RED_LED, HIGH);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, HIGH);
        delay(50); 
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        delay(50);
      }

      Serial.println(potValue);
      delay(500);
      Serial.println(photoValue);
    }
    else if(inputString.equalsIgnoreCase("Off")){
      rgbCounter = 0;
    }
    else if(inputString.equalsIgnoreCase("Blue")){
      rgbCounter = 1;
    }
    else if(inputString.equalsIgnoreCase("Green")){
      rgbCounter = 2;
    }
    else if(inputString.equalsIgnoreCase("Cyan")){
      rgbCounter = 3;
    }
    else if(inputString.equalsIgnoreCase("Red")){
      rgbCounter = 4;
    }
    else if(inputString.equalsIgnoreCase("Purple")){
      rgbCounter = 5;
    }
    else if(inputString.equalsIgnoreCase("Yellow")){
      rgbCounter = 6;
    }
    else if(inputString.equalsIgnoreCase("White")){
      rgbCounter = 7;
    }
    else if(inputString.equalsIgnoreCase("ROFF")){
      digitalWrite(RED_LED, LOW);
    }
    else if(inputString.equalsIgnoreCase("RON")){
      digitalWrite(RED_LED, HIGH);
    }
    else if(inputString.equalsIgnoreCase("YOFF")){
      digitalWrite(YELLOW_LED, LOW);
    }
    else if(inputString.equalsIgnoreCase("YON")){
      digitalWrite(YELLOW_LED, HIGH);
    }
    else if(inputString.equalsIgnoreCase("GOFF")){
      digitalWrite(GREEN_LED, LOW);
    }
    else if(inputString.equalsIgnoreCase("GON")){
      digitalWrite(GREEN_LED, HIGH);
    }
    else if(inputString.equalsIgnoreCase("BOFF")){
      digitalWrite(BLUE_LED, LOW);
    }
    else if(inputString.equalsIgnoreCase("BON")){
      digitalWrite(BLUE_LED, HIGH);
    }
    
    
  
    
    inputString = "";
    stringComplete = false;
  }
  updateRGB();
}

void updateRGB(){
  if(rgbCounter == 0){
    digitalWrite(RGB_RED_LED,LOW);
    digitalWrite(RGB_GREEN_LED,LOW);
    digitalWrite(RGB_BLUE_LED,LOW);
  }
  if(rgbCounter == 1){
    digitalWrite(RGB_RED_LED,LOW);
    digitalWrite(RGB_GREEN_LED,LOW);
    digitalWrite(RGB_BLUE_LED,HIGH);
  }
  if(rgbCounter == 2){
    digitalWrite(RGB_RED_LED,LOW);
    digitalWrite(RGB_GREEN_LED,HIGH);
    digitalWrite(RGB_BLUE_LED,LOW);
  }
  if(rgbCounter == 3){
    digitalWrite(RGB_RED_LED,LOW);
    digitalWrite(RGB_GREEN_LED,HIGH);
    digitalWrite(RGB_BLUE_LED,HIGH);
  }
  if(rgbCounter == 4){
    digitalWrite(RGB_RED_LED,HIGH);
    digitalWrite(RGB_GREEN_LED,LOW);
    digitalWrite(RGB_BLUE_LED,LOW);
  }
  if(rgbCounter == 5){
    digitalWrite(RGB_RED_LED,HIGH);
    digitalWrite(RGB_GREEN_LED,LOW);
    digitalWrite(RGB_BLUE_LED,HIGH);
  }  
  if(rgbCounter == 6){
    digitalWrite(RGB_RED_LED,HIGH);
    digitalWrite(RGB_GREEN_LED,HIGH);
    digitalWrite(RGB_BLUE_LED,LOW);
  }  
  if(rgbCounter == 7){
    digitalWrite(RGB_RED_LED,HIGH);
    digitalWrite(RGB_GREEN_LED,HIGH);
    digitalWrite(RGB_BLUE_LED,HIGH);
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

void redButtonISR(){
  MainEventFlags |= Flag_BUTTON_RED;
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
