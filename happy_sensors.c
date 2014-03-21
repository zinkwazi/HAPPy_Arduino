// High Altitude Payload Project Arduino sensor array.
// http://happycapsule.com
// 
// Version 1.0 by Greg Lawler - successful flight Aug 14, 2011


#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Chronodot.h>

#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Chronodot chronodot = Chronodot();
// OneWire addresses, need to grab them with a canned script...
DeviceAddress outsideThermometer = { 0x10, 0xB7, 0x5f, 0xb5, 0x01, 0x08, 0x00, 0x4e };
DeviceAddress insideThermometer = { 0x28, 0x6E, 0x33, 0xE3, 0x02, 0x00, 0x00, 0x8A };
const int chipSelect = 4;


void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  sensors.begin();
  sensors.setResolution(outsideThermometer, 10);
  sensors.setResolution(insideThermometer, 10);
//  bmp085Calibration();
 
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}

void loop()
{
  delay(1000);
  sensors.requestTemperatures();
  float tempO = sensors.getTempC(outsideThermometer);
  float tempI = sensors.getTempC(insideThermometer);
  if (tempO == -127.00) {
    Serial.print("Error getting temperature");
  } else {
 //   Serial.print(" C: ");
 //   Serial.println(tempC);
  }
  
  // build the timestamp
  chronodot.readTimeDate();
  String timeStamp = "";
  
  int yearZ = chronodot.timeDate.year;
  timeStamp += yearZ;
  timeStamp += "-";

  int monthZ = chronodot.timeDate.month;
  if (monthZ < 10) timeStamp += "0";
  timeStamp += monthZ;    
  timeStamp += "-";
  
  int dayZ = chronodot.timeDate.day;
  if (dayZ < 10) timeStamp += "0";
  timeStamp += dayZ;
  timeStamp += ",";
 
  int hourZ = chronodot.timeDate.hours;
  if (hourZ < 10) timeStamp += "0";
  timeStamp += hourZ; 
  timeStamp += ":";

  int minZ = chronodot.timeDate.minutes;
  if (minZ < 10) timeStamp += "0";
  timeStamp += minZ; 
  timeStamp += ":";

  int secZ = chronodot.timeDate.seconds;
  if (secZ < 10) timeStamp += "0";
  timeStamp += secZ; 
  timeStamp += ":";
  
  // open the log file and append the data row.
  File dataFile = SD.open("HAPPy_data.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.print(timeStamp);
    dataFile.print(",");
    dataFile.print(tempO);
    dataFile.print(",");
    dataFile.print(tempI);
    dataFile.println(",");

    // close the log file... 
    dataFile.close();

    // print to the serial port too:
    Serial.println(timeStamp);
    Serial.print("Outside Temperature: ");
    Serial.println(tempO);
    Serial.print("Inside Temperature: ");
    Serial.println(tempI);
    Serial.println();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Shizzle! Error opening log file...");
  }
}
