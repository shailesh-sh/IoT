/*
#include "DHT.h"                  //for accuracy in temp and humidity
#define dhtPin A0;

dht DHT;                          //created an object
*/

#include <SoftwareSerial.h>
#include <stdlib.h>

#define SSID "ABC"
#define PASS "XYZ"

int tempPin = A1;
int moistPin = A2;
int lightPin = A3;
int lightPin_2 = A4;
int led = 13;
int temp_activ = 2;
int light_activ1 = 3;
int light_activ2 = 4;
float tempC;
float light_1;
float light_2;

String apikey="8KPNAC0Q81O3XN2M";

SoftwareSerial ser(0,1);
//setup is where we specify the input, output & baud rate, do we have to setup wifi or bluetooth ?

void setup() {
  Serial.begin(9600);         //default baud rate
  ser.begin(9600);
  Serial.println("AT+RST");
  ser.println("AT+RST"); 
  delay(5000);
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  Serial.println("AT");
/*  if(Serial.find("OK")){
    Serial.println("ok");
  }else{
    Serial.println("not ok");
  }
 */ pinMode(tempPin,INPUT);
  pinMode(temp_activ,INPUT);
  pinMode(light_activ1,INPUT);
  pinMode(light_activ2,INPUT);
  pinMode(moistPin,INPUT);
  pinMode(lightPin,INPUT);
  pinMode(lightPin_2,INPUT);
  pinMode(led,OUTPUT);        // for led that will lit up when board is active 
}

//loop is main code where we write the desired code

void loop() {
/*  
  
  DHT.read11(dhtPin);
  Serial.println(DHT.temperature);
  Serial.println(DHT.humidity);


*/
  
  digitalWrite(led,HIGH);       //for turning on the led that is attached to the board
  int temp_activ_read = digitalRead(temp_activ);
  int light_activ1_read = digitalRead(light_activ1);
  int light_activ2_read = digitalRead(light_activ2);
  
  if(temp_activ_read){
    tempC=fortemp(tempPin);
  }

  if(light_activ1_read){
    light_1=forlight(lightPin);  
  }

  if(light_activ2_read){
    light_2=forlight(lightPin_2);  
  }
//  Serial.println(light_1);
//  Serial.println(light_2);
 // wifi(tempC, light_1, light_2);
  delay(16000);
}



//function to convert output of tempPin to voltage if adafruit library isn't used 

float getVolt(int pin){
   return (analogRead(pin) * 0.004882814);
}

float fortemp(int pin){
  float volt = getVolt(tempPin);
  float celsius = (volt-0.5)*100.0;
  float tempF = celsius*(9.0/5.0)+32.0;
  Serial.println("Temperature(in celsius) :");
  Serial.println(celsius);
  Serial.println("Temperature(in fahrenheit) :");
  Serial.println(tempF);
  return celsius;
}

float forlight(int pin){
  float light = analogRead(pin);
  Serial.println("Light Intensity : " );
  Serial.println(light);
  return light;
}

void wifi(float temp, float light, float light_2){
  char buf[16];
  String strTemp = dtostrf(temp, 4, 1, buf);
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

 String getStr = "GET /update?api_key=";
  getStr += apikey;
  getStr +="&field1=";
  getStr += String(strTemp);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

  }
