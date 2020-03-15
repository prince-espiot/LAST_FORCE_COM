#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <HX711.h>

#define LEDonBoard 2 

#include "index.h"
#include "style.h"
#include "javascript.h"


const char* ssid = "DESKTOP-7847I2M 5817";
const char* password = ")90P450f";

ESP8266WebServer server(80);

HX711 scale(3, 2); //HX711 scale(dat, sclk); // Dat = pin6; SCK= 7.

void handleRoot() {          
    server.send_P ( 200, "text/html", index_html);   
}
void handleCss() {
      server.send_P ( 200, "text/css", style_css);
}

void handlehx_json(){
  
  server.send ( 200, "application/json",javascript);
  }

float calibration_factor = -80;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board

    //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------


   server.on("/",handleRoot); 
   server.on("/style.css",handleCss);
   server.on("/data.txt",handlehx_json);
   server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  float data;
  data = Hx_Reading();
server.handleClient();
}

float Hx_Reading(){
  scale.set_scale(calibration_factor);
  scale.tare();
  float unitss;
  float ounces;
  unitss = scale.get_units(),10;
  if (unitss < 0)
  {
    unitss = 0.00;
  }
  ounces = unitss * 0.035274;
  return unitss;
  }
