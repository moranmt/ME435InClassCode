
String inputString = "";         // a String to hold incoming data
volatile bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("READY");
}

void loop() {
  if (stringComplete) {
    //Serial.print("From Board: ");
    //Serial.println(inputString);
    if(inputString.equalsIgnoreCase("ON")){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(inputString.equalsIgnoreCase("OFF")){
      digitalWrite(LED_BUILTIN, LOW);
    }
  
    
    inputString = "";
    stringComplete = false;
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
