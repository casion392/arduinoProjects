//code for client 1st floor of my building guidance system in independent study

#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "UCB Wireless"; //Enter SSID
const char* password = ""; //Enter Password
const char* websockets_server_host = "10.201.4.237"; //Enter server adress
const uint16_t websockets_server_port = 3001; // Enter server port
String sendr;
String receiver = "soIstartedBlasting";
String memory = "soIstartedBlasting";
int StringCount = 0;
int splitNum = 2;
String strs[10];
int left[] = {100,102};
int right[] = {113,109,105,104,101};
int rooms[] = {100,102,113,109,105,104,101};
int self = 100;
int next;

//leds for direction
int LL = 14;
int UL = 27;
int RL = 26;
int DL = 25;


//leds for display
int LEDs[] = {15, 2, 4, 16, 17, 5, 18, 19};  

//button/debounce stuff
bool actionBool = false;
bool key = false;
int button = 13;


//arrays for segment display digits
int numbs[10][7] = {{0, 0, 0, 0, 1, 1, 0},
                    {1,1,0,1,1,0,1,},
                    {1,1,1,1,0,0,1},
                    {0,1,1,0,0,1,1},
                    {1,0,1,1,0,1,1},
                    {1,0,1,1,1,1,1},
                    {1,1,1,0,0,0,0},
                    {1, 1, 1, 1, 1, 1, 1},
                    {1,1,1,0,0,1,1},
                    {1,1,1,1,1,1,0}
                    };

//room send and change check
int room =0;
int prev = 14;




using namespace websockets;

WebsocketsClient client;
void setup() {
    Serial.begin(9600);

//digit display
for (int i = 0; i<7; i++) pinMode(LEDs[i], OUTPUT);
  //potentiometer
  pinMode(34, INPUT);
  //button
  pinMode(button, INPUT);

  pinMode(LL, OUTPUT);
  pinMode(UL, OUTPUT);
  pinMode(RL, OUTPUT);
  pinMode(DL, OUTPUT);
    
    // Connect to wifi
    WiFi.begin(ssid, password);
   

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
    if(connected) {
        Serial.println("Connected!");
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message){

      
      receiver = message.data(); 
      
    });
}

void loop() {

/////////
if(digitalRead(button) && !key){
  actionBool = ! actionBool;
  key = true;
}
if(!digitalRead(button)){
  key = false;
}

//action bool controls actions

if(actionBool){
  
  sendr = String(self) + "/" + String(room);
  client.send(sendr);

  actionBool = false;
}

if(!actionBool){
  //potentiometer control
  room = map(analogRead(34),0,4095,0,9);
  //display update
  if(room != prev){
    for (int i = 0; i<7; i++) digitalWrite(LEDs[i], numbs[room][i]);
    prev = room;
    }

// let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
    }


    if(receiver[0]=='@'){
          for(int j = 0; j < 10; j++){
            strs[j] = "0";
          }
          
    StringCount = 0;
   
         while (receiver.length() > 0)
        {
          int index = receiver.indexOf('/');
          if (index == -1) // No space found
            {
              //should never happen
              strs[StringCount++] = receiver;
              break;
            }
          else
            {
              strs[StringCount++] = receiver.substring(0, index);
              receiver = receiver.substring(index+1);
            }
        }
        
        //loops through received message, (not including the "@" symbol at index 0, finds self and selects next for comparison
        for(int i = 1; i < 10; i ++){
          if(strs[i].toInt() == self){
            next = strs[i+1].toInt();
            break;
          }
          else{next = 420;}
        }

//this node is not involved
if(next == 420){
    digitalWrite(LL,LOW);
      digitalWrite(RL,LOW);
      digitalWrite(UL,LOW);
      digitalWrite(DL,LOW);
  Serial.println("not me");
}

//node is involved
//Logic for turning the message into a point direction of light
else{
      digitalWrite(LL,LOW);
      digitalWrite(RL,LOW);
      digitalWrite(UL,LOW);
      digitalWrite(DL,LOW);
        //checks if destination is on current floor
        if((abs(next)-abs(self))<100 && (abs(next)-abs(self))>0){

        //dumb for loop that uses index as a relational 
         for(int b = 0; b < sizeof(rooms); b++){
          if(next == rooms[b]){
            if(b<splitNum){
              Serial.println("L");
              digitalWrite(LL, HIGH);
              break;
            }
            else{
              Serial.println("R");
              digitalWrite(RL, HIGH);
              break;
            }
          }
         }
         
        }

        //checks if destination is below current floor
        else if(next < self){
           //do lights pointing down
           Serial.println("DOWN");
           digitalWrite(DL, HIGH);
        }

        //checks if destination is above current floor
        else if(next > self){
            //do lights pointing up
            Serial.println("UP");
            digitalWrite(UL, HIGH);
        }

        else{
          //do nothing
        }

  }
        
}
     
    delay(500);
    
}
/////////
  
}
