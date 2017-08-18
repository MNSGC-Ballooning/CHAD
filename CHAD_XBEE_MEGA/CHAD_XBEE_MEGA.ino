// This code is for the Arduino motor-controller in the Controlled
// Heading Automation Device (CHAD) built by Andrew Kruger and
// students in the Physics 295 Independent Research course at
// Wright College
// Instructions and information found at http://physi.cz/chad


// Adafruit Motorshield v2
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <SD.h>
#include <Adafruit_GPS.h>
#include <Relay_XBee.h>

#define encoderPinA 2
#define encoderPinB 3

volatile int encoderPos = 0;

//XBEE STUFF
XBee xbee = XBee(&Serial, "CH");

Adafruit_GPS GPS(&Serial2); //Constructor for GPS object
File compassfile;
File gpsfile;
File Xbeelog;
unsigned long flightStart = 0;

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
long curPos;
int amtMove;                  //Number of steps motor should move

bool blinkState = false;      //Used to verify program active

unsigned long updateTime = 0;
unsigned long timer = 0;
unsigned long turner = 0;

void setup()
{
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(led, OUTPUT);
  pinMode(5, OUTPUT);           // set-up for RESET pin
  pinMode(6, OUTPUT);           // set-up for MEGA RESET pin
  pinMode(4, OUTPUT);            //Pin for SD logging
  AFMStop.begin();                // Start the top shield

  Serial1.begin(38400);         // Ard-to-Ard serial rate

//XBEE STUFF
  xbee.begin(9600);             // XBee serial rate

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

  //enable encoder interrupts
  attachInterrupt(digitalPinToInterrupt(encoderPinA), ISR_encoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), ISR_encoderB, CHANGE);
  
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
      updateTime = millis();
     
      rotSpeed = rotSpeed_str.toInt();
      gammaPos = -Serial1.parseFloat(); 

     
      if(millis() - timer > 10000){
        timer = millis();
        compassfile = SD.open("test.txt", FILE_WRITE);
        if(compassfile){
          int encoder;
          noInterrupts();
          encoder = encoderPos;
          interrupts();
          compassfile.print(encoder);
          compassfile.print("stepper");
          compassfile.print(stepper.currentPosition());
          compassfile.print(flightTimeStr());
          compassfile.print("rotspeed: "); 
          compassfile.print(rotSpeed_str); 
          compassfile.print("  gammaPos: "); 
          compassfile.println(gammaPos);
          compassfile.close();
        }
      }
       
      gammaPosSteps = gammaPos * DegToSteps;  //Convert to steps
      curPos = stepper.currentPosition() - startingPosition;  //Get current position
      amtMove = -gammaPosSteps - (curPos % 200) + rotSpeed*rotSpeedScale;// + offset; //Calculate ammount moved
      
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

  //Every minute, the payload should spin 360 degrees
  if(millis() - turner > 600000){
    stepper.moveTo(stepper.currentPosition() - 200);
      stepper.runToPosition();
      turner = millis();
    }

  xBeeCommand();  //respond to XBee commands if any
  stepper.run();  //Move
  updateGPS();
  
  if( millis() - updateTime > 10000)
  {
    logxbee("CH;error,RESET!");
    digitalWrite(5, HIGH);   // turn the RESET on (HIGH is the voltage level)
    delay(500);              // wait for a 0.5 second
    digitalWrite(5, LOW);
    delay(2000);            // wait 2 sec for system to start back up
                            //Statement will repeat until serial connects
  }
}
