// This code is for the Arduino motor-controller in the Controlled
// Heading Automation Device (CHAD) built by Andrew Kruger and
// students in the Physics 295 Independent Research course at
// Wright College
// Instructions and information found at http://physi.cz/chad


// Adafruit Motorshield v2
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GPS.h>
#include <math.h>
Adafruit_GPS GPS(&Serial2); //Constructor for GPS object

//File compassfile;
File gpsfile;
File Xbeelog;
unsigned long flightStart = 0;
//eclipse location stuff
class eclipseLoc{
  public:
  float Lat;
  float Long;
  String Time;
  eclipseLoc* Next;
  eclipseLoc(float lat, float longi, String tim);
  void UpdateLocation;
};

eclipseLoc shadowLoc[]= {{44.87,124.84,"17:16"}, {44.81,123.203333,"17:18"}, {44.7333333,121.6316666,"17:20"},
{44.6383333,120.116666,"17:22"}, {44.53, 118.656666,"17:24"},{44.4066666,117.245,"17:26"},{44.2716666,115.878333,"17:28"},
{44.1233333,114.553333,"17:30"},{43.965,113.268333,"17:32"},{43.795,112.02,"17:34"},{43.615,110.806666,"17:36"},
{43.4266666,109.625,"17:38"},{43.2283333,108.473333,"17:40"},{43.0216666,107.35,"17:42"},{42.8083333,106.255,"17:44"},
{42.585,105.185,"17:46"},{42.355,104.138333,"17:48"},{42.1183333,103.115,"17:50"},{41.875,102.113333,"17:52"},{41.625,101.133333,"17:54"},
{41.3683333,100.171666,"17:56"},{41.1066666,99.2283333,"17:58"},{40.8383333,98.3033333,"18:00"},
{40.565,97.3933333,"18:02"},{40.285,96.5,"18:04"},{40.0,95.6216666,"18:06"},{39.71,94.7583333,"18:08"},
{39.415,93.9066666,"18:10"},{39.115,93.0683333,"18:12"},{38.8116666,92.2416666,"18:14"},{38.5016666,91.4266666,"18:16"},
{38.1883333,90.62,"18:18"},{37.87,89.825,"18:20"},{37.5466666,89.0383333,"18:22"},{37.22,88.26,"18:24"},
{36.8883333,87.49,"18:26"},{36.5516666,86.7266666,"18:28"},{36.2116666,85.97,"18:30"},{35.8683333,85.22,"18:32"},
{35.52,84.475,"18:34"},{35.1666666,83.7333333,"18:36"},{34.81,82.9966666,"18:38"},{34.4483333,82.2633333,"18:40"},
{34.0833333,81.5333333,"18:42"},{33.715,80.805,"18:44"},{33.34,80.0783333,"18:46"},{32.9633333,79.3516666,"18:48"}};


Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers
const uint8_t led = 13;
// Connect to stepper on the shield
Adafruit_StepperMotor *myStepper = AFMStop.getStepper(200, 1);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep() { myStepper->onestep(FORWARD, DOUBLE); }
void backwardstep() { myStepper->onestep(BACKWARD, DOUBLE); }

// Wrap the 3 steppers in an AccelStepper object
AccelStepper stepper(forwardstep, backwardstep);

// Set up variables for rotational speed correction
float rotSpeed;               //Input angular speed
String rotSpeed_str;          //For converting incoming string
float rotSpeedScale = .27;    //SCALE ROTATION CORRECTION <-------------------
                              //Found experimentally, this may need to change.
                              //See http://physi.cz/

// Set up variables for heading
float gammaPos;               //Input heading from gamma
int gammaPosSteps;            //Convert units to motor steps
int heading;                  //Desired heading
const float DegToSteps = 0.555556;  //Degree to Motor steps conversion
                                     //Found by (#steps/360deg)
int startingPosition;
int offset = 0;
int Spins =0;             //Rotation tests
bool spin = false;        //Rotation tests
long curPos;
int amtMove;                  //Number of steps motor should move

bool blinkState = false;      //Used to verify program active

String lastcommand = "" ; //Used to keep track of commands, to avoid repeats
unsigned long commandtime = 0;
unsigned long timing = 0;

const float pi = 3.1415926;

void setup()
{
  //settting up eclipse location
  for(int i =0;i<46;i++){
  shadowLoc[i].Next = &shadowLoc[i+1];
  }
  pinMode(led, OUTPUT);
  pinMode(5, OUTPUT);           // set-up for RESET pin
  pinMode(6, OUTPUT);           // set-up for MEGA RESET pin
  pinMode(4, OUTPUT);            //Pin for SD logging
  AFMStop.begin();                // Start the top shield

  Serial1.begin(38400);         // Ard-to-Ard serial rate
  Serial.begin(9600);           // XBee serial rate
  // AHRS uses 38400 baud

//Set up GPS stuff
  GPS.begin(9600);
  Serial2.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
  stepper.setMaxSpeed(5000.0);    //steps/sec, high value is limited by time
                                  //taken to call run()
  stepper.setAcceleration(200.0); //steps/sec^2
  heading = 0 * DegToSteps;       // In units of motor steps
                                  // if it does not begin with gamma at 0
  startingPosition = (Serial1.parseFloat() * DegToSteps);
  
  while (!Serial){;}      //  wait for serial port to connect

  // initialize SD
  if (!SD.begin(4)) {
    Serial1.println("initialization failed!");
    return;
  }
 
  Serial1.println("Setup complete.");
}

void loop()
{
  if (Serial1.available() > 0)
    { 
      // Read serial data from AHRS
      rotSpeed_str = Serial1.readStringUntil(',');
      commandtime = millis();
      
     
      rotSpeed = rotSpeed_str.toInt();
      gammaPos = -Serial1.parseFloat(); 

      // Record position (magnetic field)
     // compassfile = SD.open("test.txt", FILE_WRITE);

       //if(compassfile){
      //compassfile.print("gammaPos: "); 
      //compassfile.println(gammaPos);
      //compassfile.close();
      //}
      

      gammaPosSteps = gammaPos * DegToSteps;  //Convert to steps
      curPos = stepper.currentPosition() - startingPosition;  //Get current position
      amtMove = -gammaPosSteps - (curPos % 200) + rotSpeed*rotSpeedScale + offset -Spins; //Calculate ammount moved

      if(spin){
        stepper.moveTo(stepper.currentPosition() + 200);
        stepper.runToPosition();
        spin = false;
      }
                        
      //if the amount to move is less than -180deg, that is it will rotate more than half way
      if (amtMove < -100) { amtMove+=200; }
      //if the amount to move is greater than 180deg, that is it will rotate more than half way
      if (amtMove >= 100) { amtMove-=200; }
      amtMove = amtMove % 200;

      
      stepper.move(amtMove);  //Set amount to move  
      stepper.run();          //Move
      serialFlush();          //Clear serial memory
    
      //Blink LED to show data is being received and processed
      blinkState = !blinkState; 
      digitalWrite(13, blinkState); 
     

      stepper.run();          //Move
    } 
  xBeeCommand();                              //  respond to XBee commands if any
  stepper.run();  //Move
  updateGPS();
  
  timing = millis();
  timing = timing % 600000;
  
  if( millis() -commandtime > 10000)
  {
    sendToXbee("error,RESET");
    digitalWrite(5, HIGH);   // turn the RESET on (HIGH is the voltage level)
    delay(500);              // wait for a 0.5 second
    digitalWrite(5, LOW);
    delay(2000);            // wait 2 sec for system to start back up
                            //Statement will repeat until serial connects
  } 
  


  //Every minute, the payload should spin 360 degrees
  if(timing >=0 && timing <=5){
    stepper.moveTo(stepper.currentPosition() - 200);
      stepper.runToPosition();
    }
  

    
     
      
  
}


void serialFlush()
{
  while (Serial1.available() > 0) {
    char t = Serial1.read();
  }
}
void serialBFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
void xBeeCommand(){
  if (Serial.available()){

    //  Grab transmission and parse command  //
    int len, num;
    String transmission, id, command;
    transmission = Serial.readStringUntil('!');
    len = transmission.length();

    num = transmission.indexOf('?');
    id = transmission.substring(0, num);
    command = transmission.substring(num + 1, len);

    //  Verify id and perform valid command //
 

     if (id == "CH") {

     // if(command = lastcommand && (millis() - commandtime < 5000)) return;
      
       //  lastcommand = command;
         //commandtime = millis();
      
      
   if (command == "RZ") {
    sendToXbee("RZ RECEIVED");
    rotSpeedScale =0;
      } 
  if (command == "RS") {
    sendToXbee("RS RECEIVED");
    digitalWrite(5, HIGH);   // turn the RESET on (HIGH is the voltage level)
    delay(500);              // wait for a 0.5 second
    digitalWrite(5, LOW);
    
      }
  if(command == "DIE"){   //And now, the end is near
    sendToXbee("why...");       //and so I face the final curtain
    digitalWrite(6,HIGH); //My friend, I'll say it clear
    delay(1000);          //wait for a 1 second
    digitalWrite(6, LOW); //I'll state my case, of which I'm certain
  }
  if(command == "SP"){
    sendToXbee("SP RECIVED");
    spin = true;
  }
  if (command == "TL") {
    sendToXbee("TL RECEIVED");
    offset +=10;
      } 
  if (command == "TR") {
    sendToXbee("TR RECEIVED");
    offset -=10;
      }
  if (command == "TLL") {
    sendToXbee("TLL RECEIVED");
    offset +=50;
      } 
  if (command == "TRR") {
    sendToXbee("TRR RECEIVED");
    offset -=50;
      }
  if(command == "OF"){
    String mystring = String(offset);
    sendToXbee(mystring);
      }
    }

  serialBFlush();

}

}


void sendToXbee(String msg){
  Serial.println("CH;"+msg + "!");
  logxbee("CH;"+msg + "!");
}

void updateGPS(){

  
  while (Serial2.available() > 0) {
    GPS.read();
  }


  
    if (GPS.newNMEAreceived()) {
    int lastTime = getGPStime();
    GPS.parse(GPS.lastNMEA());
   
    
    if (getGPStime() > lastTime) {
      gpsfile = SD.open("gpsstuff.txt", FILE_WRITE);

      String data = "";
      if (GPS.fix) {
        data += (flightTimeStr() + "," + String(GPS.latitudeDegrees) + "," + String(GPS.longitudeDegrees) + ",");
        data += (String(GPS.altitude * 3.28048) + ",");    //convert meters to feet for datalogging
        data += (String(GPS.month) + "/" + String(GPS.day) + "/" + String(GPS.year) + ",");
        data += (String(GPS.hour) + ":" + String(GPS.minute) + ":" + String(GPS.seconds) + ",");
        
        
      }
      else
        data += (flightTimeStr() + ",No fix");
     
      
      gpsfile.println(data);
      gpsfile.close();
  
    }
  }



}



int getGPStime() {    //returns GPS time as seconds since 0:00:00 UTC. Note that comparisons crossing that time will be inaccurate
  return GPS.hour * 3600 + GPS.minute * 60 + GPS.seconds;
}

//returns milliseconds since last flight start command (or bootup)
unsigned long flightTime() {
  return (millis() - flightStart);
}

//returns the above flight time as a usable string for print statements
String flightTimeStr() {
  unsigned long t = flightTime() / 1000;
  String fTime = "";
  fTime += (String(t / 3600) + ":");
  t %= 3600;
  fTime += String(t / 600);
  t %= 600;
  fTime += (String(t / 60) + ":");
  t %= 60;
  fTime += (String(t / 10) + String(t % 10));
  return fTime;
}


void logxbee(String msg){
      Xbeelog = SD.open("xbee.txt", FILE_WRITE);

      if(Xbeelog){
      Xbeelog.print(flightTimeStr());
      Xbeelog.print("\t");
      Xbeelog.println(msg);
      Xbeelog.close();
      }
  
  }

