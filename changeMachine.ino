//Build a program to calculate change in proper denominations after accepting a starting USD$ value. For example, a starting value of $11.00 would result in (1) $10 and (1) $1 bills.
//
//Design specs:
//- Make use of all common USD$ denominations
//- Calculate change using the minimum number of bills and coins necessary
//- Assume infinite amounts of each denomination in change machine
//- Use a language of your choice and provide source code for review
//- Functioning program must be demonstrable to reviewers in some fashion



//how to

//1. take input as a float
//2. convert to int for convenience
//3. initialize 3 arrays, one that contains USD increments, one that saves how many of each increment is used, and one that stores string names of increments
//4. run a for loop on input.


//potential problem: float has 5 digits of accuracy :(
//possible solution: read in data as a string, storing each ascii as a digit in an array

#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//array of denominations
long usdVals[10] = {10000,5000,2000,1000,500,100,25,10,5,1};
//array of times denoms used
int usdUsage[10] = {0,0,0,0,0,0,0,0,0,0};
//array of chars for lcd display
String inputChar[12] = {"0","1","2","3","4","5","6","7","8","9",".", " "};
//what char is used where in display line 1
int displayPosition[16] = {11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11};
//initialize display to all 0s
String displayString = "                ";

//array of names for denoms as strings
String usdNames[10] = {"hundred(s)", "fifty(s)", "twenty(s)", "ten(s)", "five(s)", "one(s)", "quarter(s)", "dime(s)", "nickel(s)", "penny(s)"};
//string inputs for serial input to 
String inputString;
String copyString;

//which digit in display you are changing
int cursor = 0;

//counts how many . in string
int dot = 0;

//initialize value for string to int conversion
long inputValConvert = 0;

//bool to only perform operation after you have selected a value
bool entered = false;
//bool if a decimal is in the value or not
bool decimal = false;
//bool if more than one decimal is in value to throw an error
bool dotFound = false;
//bool to continue operation if errors are not thrown
bool trigger = true;

void setup() {

  //setup for joystick and button
  pinMode(8,INPUT);
  pinMode(A1,INPUT);
  pinMode(A0,INPUT);

  //initialize serial comm for debugging and lcd screen
  Serial.begin(9600);
  lcd.begin(16, 2);
  
}
void loop() {
//  Serial.println(analogRead(A0)); //upDown
//  Serial.println(analogRead(A1)); //leftRight
 
//control stick stuff on display
//////////////////////////////////////////////////////////
//LR
if(analogRead(A1)==1023){
  cursor++;
  if(cursor == 16){
    cursor = 0;
  }
  delay(200);
}
if(analogRead(A1)==0){
  cursor--;
  if(cursor == -1){
    cursor = 15;
  }
  delay(200);
}

//UD
if(analogRead(A0)==1023){
  displayPosition[cursor]++;
  if(displayPosition[cursor] == 12){
    displayPosition[cursor] = 0;
  }
  delay(200);
}
if(analogRead(A0)==0){
  displayPosition[cursor]--;
  if(displayPosition[cursor] == -1){
    displayPosition[cursor] = 11;
  }
  delay(200);
}
////////////////////////////////////////////////////////////

//fill in string dislay
displayString = "";

//loops through and fills in display string with editted contents from lcd
for(int b = 0; b < 16; b++){
  displayString = displayString + inputChar[displayPosition[b]];
}
//clear lcd and reprint with refreshed content
lcd.clear();
lcd.print(displayString);

//button triggers operation for change
if(digitalRead(8)==1){
  //inputString placeholder for operations to preserve displayString original
  inputString = displayString;
  entered = true;
}
   
//bool to trigger string inspection
    if(entered){
      lcd.clear();
//        Serial.println(inputString);
        //checks if input string is properly formatted ie no spaces and one "."
        for(int k = 0; k < 16; k++){
          if(inputString[k] == '.'){
            
            dot++;}
        }
        for(int j = 0; j < 15; j++){
          if(isspace(inputString[j]) && !isspace(inputString[j+1])){
//            Serial.println("spacing");
            dot = 2;
          }
        }
        
        if(dot > 1){
          Serial.println("uhOh");
          lcd.clear();
          lcd.print("invalid input");
          delay(2500);
          trigger = false;
        }
        else{

          //preps string to use, reformats to an all integer format
          for(int m = 0; m < 16; m++){
            if(inputString[m] == '.'){
              trigger = true;
              decimal = true;
              inputString[m] = inputString[m+1];
              inputString[m+1] = inputString[m+2];
              for( int n = 0; n < m+2; n++){
                copyString = copyString + inputString[n];
              }
              inputString = copyString;
              copyString = "";
              
            }
          }
        }
        //trigger means all clear is given to perform operation
        if(trigger){
          inputValConvert = inputString.toInt();
  
          //if no decimal assume amounts are in dollars. Convert to understood int values
          if(!decimal){
           inputValConvert = inputValConvert * 100; 
          }
          //reset for next input
          decimal = false;

        //actual calculation of change:
        if(inputValConvert!=0){
          for(int i = 0; i < 10; i++){
            if(inputValConvert >= usdVals[i]){
              usdUsage[i] = floor(inputValConvert /usdVals[i]);
              inputValConvert = inputValConvert  - (usdUsage[i] * usdVals[i]);
            }
          }
          
          //print statements for lcd and serial console
           for(int j = 0; j < 10; j++){
              if(usdUsage[j]>0){
                Serial.println(String(usdUsage[j]) + " " + usdNames[j]);
                lcd.print(String(usdUsage[j]) + " " + usdNames[j]);
                delay(2000);
                lcd.clear();
              }
            }
          }
      }

      //reinitialize variables for another calculation
      dot = 0;
      entered = false;
      trigger = true;
      Serial.println(inputString + " input");
      Serial.println(displayString + " display");
    }
      
}
