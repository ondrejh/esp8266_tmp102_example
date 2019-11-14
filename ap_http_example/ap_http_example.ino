#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

IPAddress    apIP(192, 168, 42, 1);
ESP8266WebServer server(80);

void httpRoot() {
  String runTime = String(millis() / 1000);
  String msg = "Hello world!<br>";
  msg += "RunTime: ";
  msg += runTime;
  msg += "s";
  server.send(200, "text/html", msg);
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

void setup()
{
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
  server.onNotFound(httpUnknown);
  server.begin();
  Serial.println("HTTP is now ON.");
}

void loop()
{
  server.handleClient();
  delay(10);
}
