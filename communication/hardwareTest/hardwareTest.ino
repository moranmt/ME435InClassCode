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
#define Flag_BUTTON_GREEN 0x01
#define Flag_BUTTON_BLUE  0x02

uint8_t  RedState = HIGH;     
uint8_t  lastRedState = HIGH;
uint8_t  YellowState = HIGH;      
uint8_t  lastYellowState = HIGH;

int rgbCounter = 0;


void setup()
{
  Serial.begin(9600);
  
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
  
  attachInterrupt(digitalPinToInterrupt(GREEN_BUTTON), green_ButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BLUE_BUTTON), blue_ButtonISR, FALLING);
  
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
  
  Serial.println("Start Loop");
}

void loop()
{
  potValue = analogRead(pot);
  photoValue = analogRead(photo);
  
  Serial.print("Potentiometer = ");
  Serial.print(potValue);
  Serial.print("  Photoresistor = ");
  Serial.println(photoValue);
  
  if(MainEventFlags & Flag_BUTTON_GREEN){
    delay(30);
    MainEventFlags &= ~Flag_BUTTON_GREEN;
    if (!digitalRead(GREEN_BUTTON)){
      digitalWrite(GREEN_LED,!digitalRead(GREEN_LED));
      delay(500);
      digitalWrite(GREEN_LED,!digitalRead(GREEN_LED));
      rgbCounter = 0;
    }
  }
  
  
  if(MainEventFlags & Flag_BUTTON_BLUE){
    delay(30);
    MainEventFlags &= ~Flag_BUTTON_BLUE;
    if (!digitalRead(BLUE_BUTTON)){
      digitalWrite(BLUE_LED,!digitalRead(BLUE_LED));
      delay(500);
      digitalWrite(BLUE_LED,!digitalRead(BLUE_LED));  
      if(rgbCounter < 7){
        rgbCounter++;
      }else{
        rgbCounter = 0;
      }
    }
  }
  
  RedState = digitalRead(RED_BUTTON);
  YellowState = digitalRead(YELLOW_BUTTON);
  
  if (RedState != lastRedState) {
    if (!RedState) {
    digitalWrite(RED_LED,!digitalRead(RED_LED));
      delay(500);
      digitalWrite(RED_LED,!digitalRead(RED_LED));
    } 
    delay(50);
  }
  
  if (YellowState != lastYellowState) {
    if (!YellowState) {
      digitalWrite(YELLOW_LED,!digitalRead(YELLOW_LED));
      delay(500);
      digitalWrite(YELLOW_LED,!digitalRead(YELLOW_LED));
    } 
    delay(50);
  }
  
  lastRedState = RedState;
  lastYellowState = YellowState;
  
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

void green_ButtonISR(){
  MainEventFlags |= Flag_BUTTON_GREEN;
  
}

void blue_ButtonISR(){
  MainEventFlags |= Flag_BUTTON_BLUE;
  
}
