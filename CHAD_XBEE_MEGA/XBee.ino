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
  if (command == "RZ") {
    rotSpeedScale =0;
  } 
  if (command == "RS") {
    xbee.send("RS RECEIVED");
    digitalWrite(5, HIGH);   // turn the RESET on (HIGH is the voltage level)
    delay(500);              // wait for a 0.5 second
    digitalWrite(5, LOW);
  }
  if(command == "DIE"){   //And now, the end is near
    xbee.send("why...");  //and so I face the final curtain
    digitalWrite(6,HIGH); //My friend, I'll say it clear
    delay(1000);          //(dramatic pause)
    digitalWrite(6, LOW); //I'll state my case, of which I'm certain
  }
  if(command == "SP"){
    stepper.moveTo(stepper.currentPosition() + 200);
    stepper.runToPosition();
  }
  if (command == "TL") {
    offset +=10;
  } 
  if (command == "TR") {
    offset -=10;
  }
  if (command == "TLL") {
    offset +=50;
  } 
  if (command == "TRR") {
    offset -=50;
  }
  if(command == "OF"){
    String mystring = String(offset);
    xbee.send(mystring);
  }
  if(command != ""){
    logxbee("CH;"+command + "!");
  }
}
