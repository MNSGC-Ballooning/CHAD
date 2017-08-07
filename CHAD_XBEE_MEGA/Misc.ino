void serialFlush()
{
  while (Serial1.available() > 0) {
    char t = Serial1.read();
  }
}

int getGPStime() {    //returns GPS time as seconds since 0:00:00 UTC. Note that comparisons crossing that time will be inaccurate
  return GPS.hour * 3600 + GPS.minute * 60 + GPS.seconds;
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
