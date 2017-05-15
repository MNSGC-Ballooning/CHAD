// This code is for the Arduino motor-controller in the Controlled
// Heading Automation Device (CHAD) built by Andrew Kruger and
// students in the Physics 295 Independent Research course at
// Wright College
// Instructions and information found at http://physi.cz/chad
//HI GUYS

// Adafruit Motorshield v2
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <SPI.h>
#include <SD.h>
File myFile;

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
long curPos;
int amtMove;                  //Number of steps motor should move

bool blinkState = false;      //Used to verify program active


void setup()
{
  pinMode(led, OUTPUT);
  pinMode(5, OUTPUT);
  AFMStop.begin();                // Start the top shield
  Serial1.begin(38400);
  Serial.begin(9600);
  // AHRS uses 38400 baud
  stepper.setMaxSpeed(5000.0);    //steps/sec, high value is limited by time
                                  //taken to call run()
  stepper.setAcceleration(200.0); //steps/sec^2
  heading = 0 * DegToSteps;       // In units of motor steps
                                  // if it does not begin with gamma at 0
  startingPosition = (Serial1.parseFloat() * DegToSteps);

  while (!Serial){;}      //  wait for serial port to connect
  
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
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
     
     
      rotSpeed = rotSpeed_str.toInt();
      gammaPos = -Serial1.parseFloat(); 


      myFile = SD.open("test.txt");
      myFile.print("gammaPos: "); 
      myFile.println(gammaPos);
      myFile.close();

      gammaPosSteps = gammaPos * DegToSteps;  //Convert to steps
      curPos = stepper.currentPosition() - startingPosition ;  //Get current position
      amtMove = -gammaPosSteps - (curPos % 200) + rotSpeed*rotSpeedScale; //Calculate ammount moved
                        
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
  serialBFlush();
  stepper.run();  //Move
  
  
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
void xBeeCommand() {
  if (Serial.available()) {

    //  Grab transmission and parse command  //
    int len, num;
    String transmission, id, command;
    transmission = Serial.readStringUntil('!');
    len = transmission.length();
    num = transmission.indexOf('?');
    id = transmission.substring(0, num);
    command = transmission.substring(num + 1, len);
    
    
    Serial.println(transmission);

    //  Verify id and perform valid command //
 


     if (id == "CH") {
      
     Serial.println("CH\n");
      Serial1.println("Neato Bandito");
      if (command == "ZE") {
        int mov = stepper.currentPosition()%200;
        if( mov <-100)
        {mov = -200-mov;}
        else if( mov > 100)
        {mov = 200-mov;}
        else
        {mov = (-1)*mov;}
        stepper.move(mov);
        sendToXbee("ZE RECEIVED");
        for(int i=0; i<200; i++)
        {
          stepper.run();
          }
      }
   if (command == "RZ") {
    sendToXbee("RZ RECEIVED");
    rotSpeedScale =0;
      } 
       if (command == "RS") {
    sendToXbee("RS RECEIVED");
    digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);              // wait for a second
    digitalWrite(5, LOW);
      } 
  if (command == "HW") {
    sendToXbee("HW RECEIVED");
    startingPosition +=50;
      } 
  if (command == "HE") {
    sendToXbee("HE RECEIVED");
    startingPosition -=50;
      }
    
    
    
    
    
    }
}

}
void sendToXbee(String msg){
  if(msg != "LED\n"){
    Serial.println("CH;"+msg + "!");
  }
  else{
    Serial.print(msg);
  }
  Serial1.println("Sent " + msg + " to the xBee");
}



