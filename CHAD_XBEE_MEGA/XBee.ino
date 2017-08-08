void logxbee(String msg){
  Xbeelog = SD.open("xbee.txt", FILE_WRITE);
  if(Xbeelog){
    Xbeelog.print(flightTimeStr());
    Xbeelog.print("\t");
    Xbeelog.println(msg);
    Xbeelog.close();
  }
}

void xBeeCommand(){
  String command = xbee.receive();
  if (command.equals("")) return; //empty string means no command received

  //log command
  logxbee("CH;"+command + "!");
  
  if (command == "RZ") {
    rotSpeedScale =0;
  } 
  else if (command == "RS") {
    xbee.send("RS RECEIVED");
    digitalWrite(5, HIGH);   // turn the RESET on (HIGH is the voltage level)
    delay(500);              // wait for a 0.5 second
    digitalWrite(5, LOW);
  }
  else if (command == "DIE") { //And now, the end is near
    xbee.send("why...");       //and so I face the final curtain
    digitalWrite(6,HIGH);      //My friend, I'll say it clear
    delay(1000);               //(dramatic pause)
    digitalWrite(6, LOW);      //I'll state my case, of which I'm certain
  }
  else if (command == "SP") {
    stepper.moveTo(stepper.currentPosition() + 200);
    stepper.runToPosition();
  }
  else if (command == "TL") {
    offset +=10;
  } 
  else if (command == "TR") {
    offset -=10;
  }
  else if (command == "TLL") {
    offset +=50;
  } 
  else if (command == "TRR") {
    offset -=50;
  }
  else if (command == "OF") {
    String mystring = String(offset);
    xbee.send(mystring);
  }
  else if (command == "CP") {
    int encoder;
    noInterrupts();
    encoder = encoderPos;
    interrupts();
    String message = String(encoder) + "stepper" + String(stepper.currentPosition());
    xbee.send(message);
  }
}
