#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Wire.h"

#include "jq.h" // content of jquery.min.js

#define TMP102_I2C_ADDRESS 72

IPAddress    apIP(192, 168, 42, 1);
ESP8266WebServer server(80);
float temperature = 0.0;

void httpRoot() {
  String runTime = String(millis() / 1000);
  String msg = "<!DOCTYPE html>\n<html>\n<head>\n<script src=\"/jquery.min.js\"></script><script>function get_data() {$.getJSON(\"/\data\", function(result){$(\"#temp\").text(result.temp);$(\"#cnt\").text(result.cnt);});}</script>\n";
  msg += "<title>ESP8266 TMP102 AP</title>\n</head>\n<body onload=\"var intervalID = setInterval(function(){get_data();}, 500);\">\n<p>\n";
  msg += "<h1><span id=\"temp\">--</span>&deg;C</h1>\n";
  msg += "<span id=\"cnt\">--</span>\n";
  msg += "</body></html>";
  server.send(200, "text/html", msg);
}

void jsonData() {
  String runTime = String(millis() / 1000);
  String msg = "{\"temp\":";
  msg += temperature;
  msg += ", \"cnt\":";
  msg += runTime;
  msg += "}";
  server.send(200, "application/json", msg);
}

void httpJQ() {
  server.sendContent_P(jq, jq_length);
}

void httpUnknown() {
  String msg = "Unknown URL\n\n";
  msg += "URI: ";
  msg += server.uri();
  msg += "\nMethod: ";
  msg += (server.method() == HTTP_GET) ? "GET" : "POST";
  msg += "\nArgs: ";
  msg += server.args();
  msg += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    msg += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", msg);
}

int getTemp102()
{
  byte firstbyte, secondbyte; //these are the bytes we read from the TMP102 temperature registers
  int val; /* an int is capable of storing two bytes, this is where we "chuck" the two bytes together. */ 
 
  Wire.beginTransmission(TMP102_I2C_ADDRESS); //Say hi to the sensor. 
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(TMP102_I2C_ADDRESS, 2);
  Wire.endTransmission();
 
  firstbyte      = (Wire.read()); 
  secondbyte     = (Wire.read()); 

  val = firstbyte;
  if ((firstbyte & 0x80) > 0) 
  {
    val |= 0x0F00;
  } 
  val <<= 4; 
  val |= (secondbyte >> 4);
  return val;    
}

void setup()
{
  Wire.begin(); // start the I2C library

  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP ... ");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  boolean result = WiFi.softAP("ESPsoftAP");
  Serial.println(result ? "Ready" : "Failed!");

  if (MDNS.begin("espwebserver")) {
    Serial.println("MDNS responder ready.");
  }
  server.on("/", httpRoot);
  server.on("/data", jsonData);
  server.on("/jquery.min.js", httpJQ);
  server.onNotFound(httpUnknown);
  server.begin();
  Serial.println("HTTP is now ON.");
}

void loop()
{
  static uint32_t tempTime = 0;
  if ((millis() - tempTime) > 5000) {
    tempTime += 5000;
    int val = getTemp102();
    temperature = val * 0.0625;
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.println("°C0");
  }
  server.handleClient();
  delay(10);
}
