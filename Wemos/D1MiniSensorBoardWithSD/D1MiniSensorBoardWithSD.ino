/*
 * Authored by Dylan VanDerWal (dylanjvanderwal@gmail.com)
 * GNU General Public License .. Feel free to use / Distribute 
 * No Warranties
 * 
 * Refrence from code by Jeremy VanDerWal(jjvanderwal@gmail.com ... www.jjvanderwal.com)
 */
 
#include "DHT.h"
#include <SparkFunTSL2561.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <SD.h>

//Define & Initialize Pins
#define DHTPIN_0 D3              //What pin the DHT is connected to
#define DHTTYPE_0 DHT22          //Define what type of DHT is connected to the DHTPIN_0
DHT dht_0(DHTPIN_0, DHTTYPE_0;   //Associate DHTTYPE_0 to DHTPIN_0
Const int microphonePin = A0;    //What pin the microphone will be connected to
SFE_TSL2561 light;               //Create SFE_TSL2561 object and call it light
SFE_BMP180 pressure;             //Create SFE_BMP180 object and call it pressure
#define ALTITUDE 20.0            // Altitude of Position in Meters

//Make function for reading sensors
void readSensor();

//Variable Setup

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
