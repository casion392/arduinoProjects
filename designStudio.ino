//code to send sensor data to a raspberry pi through serial communication
#include <AudioAnalyzer.h>

#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

Analyzer Audio = Analyzer(4,5,0);//Strobe pin ->4  RST pin ->5 Analog Pin ->0

int FreqVal[7];//

String x,y,combine, button;

LSM9DS1 imu;

#define PRINT_CALCULATED

#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time


void printGyro();


void setup()
{
  Serial.begin(9600);	//Init the baudrate
//Serial.println("fuck");
  pinMode(13, INPUT_PULLUP);
  
  Audio.Init();//Init module 

   Wire.begin();

    if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                   "work for an out of the box LSM9DS1 " \
                   "Breakout, but may need to be modified " \
                   "if the board jumpers are.");
    while (1);
  }
}

void loop()
{

  button = String(digitalRead(13));
  
  Audio.ReadFreq(FreqVal);

   if ( imu.gyroAvailable() )
  {
    imu.readGyro();
  }

  
//   if ((lastPrint + PRINT_SPEED) < millis())
//  {
delay(100);
    combine = String(max((FreqVal[0]-100),0))+'n' + String(max((FreqVal[1]-100),0))+'n' 
    +String(max((FreqVal[2]-100),0))+'n'+String(max((FreqVal[3]-100),0))+'n'
    +String(max((FreqVal[4]-100),0))+'n'+String(max((FreqVal[5]-100),0))+'n'
    +String(max((FreqVal[6]-100),0))+ 'n' +String(imu.calcGyro(imu.gy), 2) + 'n' + 
    String(imu.calcGyro(imu.gz), 2) + 'n' + button ; //add button here still
  Serial.println(combine);
//    lastPrint = millis(); // Update lastPrint time
//  }
}
