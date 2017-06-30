float EclipseBearing(){
  float bearing;
  float x;
  float y;
  UpdateEclipsePos();
  x = sin(current->Long-GPS.latitudeDegrees) * cos(current->Lat);
  y = cos(GPS.latitudeDegrees*(pi/180))* sin(current->Lat)-sin(GPS.latitudeDegrees*(pi/180)) * cos(current->Lat) * cos(GPS.longitudeDegrees*(pi/180));
  bearing = atan2(y, x);
  return bearing * (180/pi);
}
eclipseLoc::eclipseLoc(float lat, float longi, String tim){
  Lat = lat;
  Long = longi;
  Time = tim;
}

void UpdateEclipsePos(){
  String currentTime = (String(GPS.hour) + ":" + String(GPS.minute));
  if(current->Next!=NULL && current->Next->Time == currentTime){
    current= current->Next;
  }
}

