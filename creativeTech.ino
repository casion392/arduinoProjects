//custom security system for my residency desk on campus. Kept people from putting stuff on it. Could be disarmed with a key

#define echoPin 2
#define trigPin 3
#define yellPin 4
#define keyPin 5
#define funPin 7
//              0    1        2     3       4     5
int songs[] = {0, 152000, 208000, 255000, 7000, 5000};
//                    0        1      2      3
int16_t trigger[] = {0X1E02,0X1E03,0X1E04,0X1E05};

long duration;
int distance; 
bool play = true;

int timez;

#include <SoftwareSerial.h>

#define ARDUINO_RX 5//should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 6//connect to RX of the module
SoftwareSerial mySerial(ARDUINO_RX, ARDUINO_TX);//init the serial protocol, tell to myserial wich pins are TX and RX

////////////////////////////////////////////////////////////////////////////////////
static int8_t Send_buf[8] = {0} ;//The MP3 player undestands orders in a 8 int string
                                 //0X7E FF 06 command 00 00 00 EF;(if command =01 next song order) 
#define NEXT_SONG 0X01 
#define PREV_SONG 0X02 

#define CMD_PLAY_W_INDEX 0X03 //DATA IS REQUIRED (number of song)

#define VOLUME_UP_ONE 0X04
#define VOLUME_DOWN_ONE 0X05
#define CMD_SET_VOLUME 0X06//DATA IS REQUIRED (number of volume from 0 up to 30(0x1E))
#define SET_DAC 0X17
//#define CMD_PLAY_WITHVOLUME 0X22 //data is needed  0x7E 06 22 00 xx yy EF;(xx volume)(yy number of song)
#define CMD_PLAY_W_VOL 0X22

#define CMD_SEL_DEV 0X09 //SELECT STORAGE DEVICE, DATA IS REQUIRED
                #define DEV_TF 0X02 //HELLO,IM THE DATA REQUIRED
                
#define SLEEP_MODE_START 0X0A
#define SLEEP_MODE_WAKEUP 0X0B

#define CMD_RESET 0X0C//CHIP RESET
#define CMD_PLAY 0X0D //RESUME PLAYBACK
#define CMD_PAUSE 0X0E //PLAYBACK IS PAUSED

#define CMD_PLAY_WITHFOLDER 0X0F//DATA IS NEEDED, 0x7E 06 0F 00 01 02 EF;(play the song with the directory \01\002xxxxxx.mp3

#define STOP_PLAY 0X16

#define PLAY_FOLDER 0X17// data is needed 0x7E 06 17 00 01 XX EF;(play the 01 folder)(value xx we dont care)

#define SET_CYCLEPLAY 0X19//data is needed 00 start; 01 close

#define SET_DAC 0X17//data is needed 00 start DAC OUTPUT;01 DAC no output
////////////////////////////////////////////////////////////////////////////////////

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(yellPin, OUTPUT);
  pinMode(keyPin, INPUT);
  pinMode(funPin, INPUT);
  Serial.begin(9600); 
  mySerial.begin(9600);//Start our Serial coms for THE MP3
  delay(500);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card 
  delay(200);//wait for 200ms
  //randomSeed(analogRead(0));
}
void loop() {

  sendCommand(CMD_PLAY_W_VOL, 0X1E02);
          delay(5000);
          Serial.println("wtf");
   
//system is armed
  if(digitalRead(keyPin)==HIGH){
 
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      duration = pulseIn(echoPin, HIGH);
    
      distance = duration * 0.034 / 2;
      delay(500);



      //yikes this is the nastiest conditional of all time shoot me in the face
        //if(distance!=128&&distance!=129&&distance!=130&&distance!=131&&distance!=132&&distance!=144&&distance!=145&&distance!=146&&distance!=147){
        
       if(distance!=144&&distance!=145&&distance!=146&&distance!=147){
        
          
           timez = random(2, 6);

          Serial.println(timez);
          
            if(play){
          sendCommand(CMD_PLAY_W_VOL, trigger[timez-2]);
           //Serial.println("buzz");
          //delay(songs[timez]);
            play = false;
            }
          }

          else{
    sendCommand(CMD_PAUSE, 0X0E);
     play = true;
          }
      Serial.println(distance);
    }

//system unarmed
    else{
      delay(1000);
      Serial.println("keyIn?");
      timez = random(2, 6);

          Serial.println(timez);
    }
}

//presumably the trigger for audio
void sendCommand(int8_t command, int16_t dat)
{
 delay(20);
 Send_buf[0] = 0x7e; //starting byte
 Send_buf[1] = 0xff; //version
 Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
 Send_buf[3] = command; //
 Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
 Send_buf[5] = (int8_t)(dat >> 8);//datah
 Send_buf[6] = (int8_t)(dat); //datal
 Send_buf[7] = 0xef; //ending byte
 for(uint8_t i=0; i<8; i++)//
 {
   mySerial.write(Send_buf[i]) ;//send bit to serial mp3
   Serial.print(Send_buf[i],HEX);//send bit to serial monitor in pc
 }
 Serial.println();
}
