EclipseBearing(){
  float bearing;
  float x;
  float y;
  UpdateEclipsePos();
  x = sin(eclipLong-chadLong) * cos(eclipLat);
  y = cos(chadLat) * sin(eclipLat)-sin(chadLat) * cos(eclipLat) * cos(chadLong);
  bearing = atan2(y, x);
  return bearing * (180/pi);
}

UpdateEclipsePos(){
  string currentTime = (String(GPS.hour) + ":" + String(GPS.minute));
  if (currentTime = "17:16"){
    eclipLat = 44.87 * (pi/180);
    eclipLong = 124.84 * (pi/180);
  }
  if (currentTime = "17:18"){
    eclipLat = 44.81 * (pi/180);
    eclipLong = 123.203333 * (pi/180);
  }
  if (currentTime = "17:20"){
    eclipLat = 44.7333333 * (pi/180);
    eclipLong = 121.6316666 * (pi/180);
  }
  if (currentTime = "17:22"){
    eclipLat = 44.6383333 * (pi/180);
    eclipLong = 120.116666 * (pi/180);
  }
  if (currentTime = "17:24"){
    eclipLat = 44.53 * (pi/180);
    eclipLong = 118.656666 * (pi/180);
  }
  if (currentTime = "17:26"){
    eclipLat = 44.4066666 * (pi/180);
    eclipLong = 117.245 * (pi/180);
  }
  if (currentTime = "17:28"){
    eclipLat = 44.2716666 * (pi/180);
    eclipLong = 115.878333 * (pi/180);
  }
  if (currentTime = "17:30"){
    eclipLat = 44.1233333 * (pi/180);
    eclipLong = 114.553333 * (pi/180);
  }
  if (currentTime = "17:32"){
    eclipLat = 43.965 * (pi/180;
    eclipLong = 113.268333 * (pi/180);
  }
  if (currentTime = "17:34"){
    eclipLat = 43.795 * (pi/180);
    eclipLong = 112.02 * (pi/180);
  }
  if (currentTime = "17:36"){
    eclipLat = 43.615 * (pi/180);
    eclipLong = 110.806666 * (180/pi);
  }
  if (currentTime = "17:38"){
    eclipLat = 43.4266666 * (pi/180);
    eclipLong = 109.625 * (pi/180);
  }
  if (currentTime = "17:40"){
    eclipLat = 43.2283333 * (pi/180);
    eclipLong = 108.473333 * (pi/180);
  }
  if (currentTime = "17:42"){
    eclipLat = 43.0216666 * (pi/180);
    eclipLong = 107.35 * (pi/180);
  }
  if (currentTime = "17:44"){
    eclipLat = 42.8083333 * (pi/180);
    eclipLong = 106.255 * (pi/180);
  }
  if (currentTime = "17:46"){
    eclipLat = 42.585 * (pi/180);
    eclipLong = 105.185 * (pi/180);
  }
  if (currentTime = "17:48"){
    eclipLat = 42.355 * (pi/180);
    eclipLong = 104.138333 * (pi/180);
  }
  if (currentTime = "17:50"){
    eclipLat = 42.1183333 * (pi/180);
    eclipLong = 103.115 * (pi/180);
  }
  if (currentTime = "17:52"){
    eclipLat = 41.875 * (pi/180);
    eclipLong = 102.113333 * (pi/180);
  }
  if (currentTime = "17:54"){
    eclipLat = 41.625 * (pi/180);
    eclipLong = 101.133333 * (180/pi);
  }
  if (currentTime = "17:56"){
    eclipLat = 41.3683333 * (pi/180);
    eclipLong = 100.171666 * (pi/180);
  }
  if (currentTime = "17:58"){
    eclipLat = 41.1066666 * (pi/180);
    eclipLong = 99.2283333 * (pi/180);
  }
  if (currentTime = "18:00"){
    eclipLat = 40.8383333 * (pi/180);
    eclipLong = 98.3033333 * (pi/180);
  }
  if (currentTime = "18:02"){
    eclipLat = 40.565 * (pi/180);
    eclipLong = 97.3933333 * (pi/180);
  }
  if (currentTime = "18:04"){
    eclipLat = 40.285 * (pi/180);
    eclipLong = 96.5 * (pi/180);
  }
  if (currentTime = "18:06"){
    eclipLat = 40.0 * (pi/180);
    eclipLong = 95.6216666 * (pi/180);
  }
  if (currentTime = "18:08"){
    eclipLat = 39.71 * (pi/180);
    eclipLong = 94.7583333 * (pi/180);
  }
  if (currentTime = "18:10"){
    eclipLat = 39.415 * (pi/180);
    eclipLong = 93.9066666 * (pi/180);
  }
  if (currentTime = "18:12"){
    eclipLat = 39.115 * (pi/180);
    eclipLong = 93.0683333 * (pi/180);
  }
  if (currentTime = "18:14"){
    eclipLat = 38.8116666 * (pi/180);
    eclipLong = 92.2416666 * (pi/180);
  }
  if (currentTime = "18:16"){
    eclipLat = 38.5016666 * (pi/180);
    eclipLong = 91.4266666 * (pi/180);
  }
  if (currentTime = "18:18"){
    eclipLat = 38.1883333 * (pi/180);
    eclipLong = 90.62 * (pi/180);
  }
  if (currentTime = "18:20"){
    eclipLat = 37.87 * (pi/180);
    eclipLong = 89.825 * (pi/180);
  }
  if (currentTime = "18:22"){
    eclipLat = 37.5466666 * (pi/180);
    eclipLong = 89.0383333 * (pi/180);
  }
  if (currentTime = "18:24"){
    eclipLat = 37.22 * (pi/180);
    eclipLong = 88.26 * (pi/180);
  }
  if (currentTime = "18:26"){
    eclipLat = 36.8883333 * (pi/180);
    eclipLong = 87.49 * (pi/180);
  }
  if (currentTime = "18:28"){
    eclipLat = 36.5516666 * (pi/180);
    eclipLong = 86.7266666 * (pi/180);
  }
  if (currentTime = "18:30"){
    eclipLat = 36.2116666 * (pi/180);
    eclipLong = 85.97 * (pi/180);
  }
  if (currentTime = "18:32"){
    eclipLat = 35.8683333 * (pi/180);
    eclipLong = 85.22 * (pi/180);
  }
   if (currentTime = "18:34"){
    eclipLat = 35.52 * (pi/180);
    eclipLong = 84.475 * (pi/180);
  }
   if (currentTime = "18:36"){
    eclipLat = 35.1666666 * (pi/180);
    eclipLong = 83.7333333 * (pi/180);
  }
   if (currentTime = "18:38"){
    eclipLat = 34.81 * (pi/180);
    eclipLong = 82.9966666 * (pi/180);
  }
   if (currentTime = "18:40"){
    eclipLat = 34.4483333 * (pi/180);
    eclipLong = 82.2633333 * (180/pi);
  }
   if (currentTime = "18:42"){
    eclipLat = 34.0833333 * (pi/180);
    eclipLong = 81.5333333 * (pi/180);
  }
   if (currentTime = "18:44"){
    eclipLat = 33.715 * (pi/180);
    eclipLong = 80.805 * (pi/180);
  }
   if (currentTime = "18:46"){
    eclipLat = 33.34 * (pi/180);
    eclipLong = 80.0783333 * (pi/180);
  }
   if (currentTime = "18:48"){
    eclipLat = 32.9633333 * (pi/180);
    eclipLong = 79.3516666 * (pi/180);
  }
}

