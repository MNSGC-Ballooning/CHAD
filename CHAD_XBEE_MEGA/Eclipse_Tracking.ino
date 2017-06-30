float EclipseBearing(){
  float bearing;
  float x;
  float y;
  UpdateEclipsePos();
  y = sin((current->Long*(pi/180))-(GPS.longitudeDegrees*(pi/180))) * cos((current->Lat)*pi/180);
  x = cos(GPS.latitudeDegrees*(pi/180))* sin(current->Lat*(pi/180))-sin(GPS.latitudeDegrees*(pi/180)) * cos(current->Lat*(pi/180)) * cos((current->Long*(pi/180))-(GPS.longitudeDegrees*(pi/180)));
  bearing = atan2(y, x)*(180/pi);
    //make sure that the bearing is always positive
  if(bearing<0){
    bearing +=360;
  }
  return bearing;
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

