#define	RED_LED		10
#define	YELLOW_LED	5
#define	GREEN_LED	6

#define BUTTON 		0


#define PRESSED		0
#define UNPRESSED	1




void setup()
{
  pinMode(13, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  pinMode(BUTTON, INPUT_PULLUP);
  
  
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  delay(100);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  delay(100);
  
  
}

void loop()
{
  
  if(digitalRead(BUTTON) == PRESSED){
    digitalWrite(GREEN_LED, HIGH);
    delay(2000);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    delay(1000);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(5000);
    digitalWrite(RED_LED, LOW);
    
  }
  else{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
  }

}