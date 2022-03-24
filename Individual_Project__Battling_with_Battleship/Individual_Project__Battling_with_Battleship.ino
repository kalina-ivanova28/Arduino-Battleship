//Importing the Servo and defining it and its position
#include <Servo.h>
Servo myservo;
int pos = 0;
//Defining LEDs
int green=12;
int red=13; 
int data = 2; 
int clock = 3;
int latch = 4;
//Defining Board and Ship Size
const int MAX_BOARD_SIZE=8;
const int SHIP_SIZE=3;
int numberOfHits = 0;
//Definig Buttons and their states
int button =8;
int button2=7;
int buttonState = 0;
int buttonState2=0;
int lastButtonState=0;
int count=-1;
int guess = 0;
boolean flag=true;

int locationCells[SHIP_SIZE];

const int LedValues[]={1,2,4,8,16,32,64,128};
int bitsLED=0;

String result;

void setup() {
  // put your setup code here, to run once:
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(button,INPUT);
  pinMode(button2,INPUT);
  Serial.begin(9600);
  myservo.attach(9);
  setLocationCells();
  updateLEDs(0);
  myservo.write(0);
  Pattern();
}


void loop() {
  // put your main code here, to run repeatedly:
   //Call checkYourself() and get result
  result=checkYourself();
  delay(500);
  if (result == "kill"){
    Pattern();
    exit(1);
  }
  
}

//A function of random LED patterns on the screen indicating start of game and win

void Pattern(){
  for (int i=0;i<10;i++){
    digitalWrite(green,LOW);
      digitalWrite(red,LOW);
    int value=random(128,257);
    updateLEDs(value);
    for (pos = 0; pos <= 180; pos += 10) { 
        myservo.write(pos);             
        delay(15);                       
       }
       value=random(128,257);
       updateLEDs(value);
      for (pos = 180; pos >= 0; pos -= 10) { 
        myservo.write(pos);              
        delay(15); 
      }
      digitalWrite(green,HIGH);
      digitalWrite(red,HIGH);
      value=random(128,257);
      updateLEDs(value);
    delay(300);
  }
  updateLEDs(0);
  digitalWrite(green,LOW);
  digitalWrite(red,LOW);
}

//Update the LEDs with a new value
void updateLEDs(int value){
  digitalWrite(latch, LOW);     //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH);   //Pulls the latch high displaying the data
}

/*getGuess is responsible for getting input from the serial monitor.
 * It returns a char to the caller.
 */

 int getGuess(){
  flag=true;
  while(flag){
    buttonState = digitalRead(button);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW) {
        if (count >=7){
          count=0;
        }else{
          count++;
        }
        
      }
      delay(50);
    }
     lastButtonState = buttonState;
     updateLEDs(LedValues[count]);
     buttonState2=digitalRead(button2);
     if (buttonState2 == LOW){
      guess=count+1;
      flag=false;
     }
  }
   return guess;
 }
 /*A function to initialize and create consecutive random places for
  * our ship.
  */
void setLocationCells(){
  //generate a random number between 0 and 5;
  int startLocation=random(0, MAX_BOARD_SIZE - SHIP_SIZE-1);
  //Serial.print("Location Cell generated are: ");
  //assign the locationcells
  for (int i=0;i<SHIP_SIZE;i++){
    locationCells[i]=startLocation+i;
    //Serial.print(locationCells[i]);
  }
  //Serial.println();
}

/*A function that checks the users guess and returns hit, miss or kill.
 * Note this function assumes the guess is valid.
 */
String checkYourself(){
  //get the users guess
  //char userGuess=getGuess();
  String result="miss";
  //convert the user guess to int.
  guess = getGuess();
  //guess-=1;
  //Serial.println(guess);
  //check the guess with the locationCells array
  for (int i=0;i<SHIP_SIZE;i++){
    //if the user's guess matches
    if (guess==locationCells[i]){
      numberOfHits++;
      locationCells[i]=-1;
      bitsLED=bitsLED+LedValues[guess-1];
      updateLEDs(bitsLED);
      if (numberOfHits ==SHIP_SIZE){
        result="kill";
      }else result="hit";
      digitalWrite(green,HIGH);
      for (pos = 0; pos <= 180; pos += 5) { 
        myservo.write(pos);             
        delay(15);                       
       }
      for (pos = 180; pos >= 0; pos -= 5) { 
        myservo.write(pos);              
        delay(15); 
      }
      digitalWrite(green,LOW); 
      break;
    }
    //if it doesn't
  }
  if (result=="miss"){
    digitalWrite(red,HIGH);
    for (int t=0;t<2;t++){
      for (pos = 0; pos <= 180; pos += 5) { 
        myservo.write(pos);             
        delay(15);                       
       }
      for (pos = 180; pos >= 0; pos -= 5) { 
        myservo.write(pos);              
        delay(15);                       
      }
    }
    digitalWrite(red,LOW);
  }
  return result;
}
