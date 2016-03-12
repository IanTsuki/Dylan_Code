/*
 * Authored by Dylan VanDerWal (dylanjvanderwal@gmail.com)
 * GNU General Public License .. Feel free to use / Distribute 
 * No Warranties
 * 
 * Refrence from code by Jeremy VanDerWal(jjvanderwal@gmail.com ... www.jjvanderwal.com)
 */
 
#include "DHT.h"
#include "SparkFunTSL2561.h"
#include "SFE_BMP180.h"
#include "Wire.h"
#include "SD.h"
#include "SPI.h"

//Define & Initialize Pins
#define DHTPIN_0 D3              //What pin the DHT is connected to
#define DHTTYPE_0 DHT22          //Define what type of DHT is connected to the DHTPIN_0
DHT dht_0(DHTPIN_0, DHTTYPE_0);   //Associate DHTTYPE_0 to DHTPIN_0
const int microphonePin = A0;    //What pin the microphone will be connected to
SFE_TSL2561 light;               //Create SFE_TSL2561 object and call it light
SFE_BMP180 pressure;             //Create SFE_BMP180 object and call it pressure
#define ALTITUDE 20.0            // Altitude of Position in Meters
const int chipSelect = D8;        // Set chipSelect
String dataString ="";            //Make string to push into CSV file
File sensorData;


//Variable Setup
int sound_sample;                //Variable to hold read varalue from microphone
int DHT_temp;                    //ariable to hold temp
int DHT_hum;                      //Varaible for humidity
int Baro_read;                    //Variable for Baro
int light_read;                   //Variable for light
boolean light_gain;              //Gain setting, 0 = X1, 1 = X16;
unsigned int light_ms;           //Intergration ("Shutter") time in milliseconds
unsigned long CurTime;



//Setup code
void setup() {
  Serial.begin(115200);          //Begin serial output on chanell 115200
  delay(10);                      //Wait 10 milliseconds

  //check if SD is connected properly
  if (!SD.begin(chipSelect)) {
    Serial.print("card read failed, please check and reset board");
    return;
  }
  pinMode(chipSelect, OUTPUT);

  //Start DHT
  dht_0.begin();                 //Begins DHT22 on pin D3

  //Start light sensor
  light.begin();                 //Begins light sensor connected to the SCL & SDA pins
  light_gain = 0;                //If gain = 0(fallse), the device is set to low gain (1X)
                                 //If gain = 1(true), the device is set to high gain (16X)
  unsigned char time = 2;        //If time = 0, intergration will be 13.7ms
                                 //If time = 1, intergration will be 101ms
                                 //If time = 2, intergration will be 402ms
  light.setTiming(light_gain,time,light_ms); //setTiming() will set the third parameter(ms) to the requested intergration time in ms 
  light.setPowerUp();                        //Power ip the sensor

  //Start the pressure sensor
  pressure.begin();               // begins pressure sensor connected to SCL & SDA pins

  dataString = "Time since boot(milliseconds),Temperatur(Degrees Celsius),Humidity(%),Barometer(HectoPascles),Light(Lux),Sound";
  saveData();
}



//Recuring script
void loop() {
  sound_sample = analogRead(microphonePin); //get sound data
  DHT_temp = dht_0.readTemperature();          //get temp
  DHT_hum = dht_0.readHumidity();             //get humidity


  //Get lux
  unsigned int data0, data1;
  if (light.getData(data0,data1)) {
    double lux;
    boolean good;
    good = light.getLux(light_gain,light_ms,data0,data1,lux);
    if (good) {
       light_read = lux;
    }
  }
  //Get Baro
  char status;
  double T,P,p0;
  status = pressure.startTemperature();
  if (status != 0) {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0) {
      status = pressure.startPressure(3);
      if (status !=0) {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status !=0); {
          p0 = pressure.sealevel(P,ALTITUDE);
          Baro_read = p0;
        }  
      }  
    }
  }  
  CurTime = int(millis);
  dataString = String(String(CurTime) + "," + String(DHT_temp) + "," + String(DHT_hum) + "," + String(Baro_read) + "," + String(light_read) + "," + String(sound_sample));
  saveData(); 
  delay(6000);
}



//Script for sving data
void saveData(){
if(SD.exists("data.csv")){ // check the card is still there
// now append new data file
sensorData = SD.open("data.csv", FILE_WRITE);
if (sensorData){
sensorData.println(dataString);
sensorData.close(); // close the file
Serial.println(dataString);
}
}
else{
Serial.print("Error writing to file !");
}
}
