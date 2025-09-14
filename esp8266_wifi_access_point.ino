#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "Головенець";
const char *password = "Головенець";

ESP8266WebServer server(80);

void handleRoot()
{
server.send(200, "text/html", "<h1>ІТІР-20-1 Головенець Максим<h1>");
}
void setup() {
// put your setup code here, to run once:
delay(1000);
Serial.begin(115200);
Serial.println();
Serial.print("Configuring access point...");


WiFi.softAP(ssid, password);

IPAddress myIP = WiFi.softAPIP();
Serial.println(ssid);
Serial.print("AP IP address: ");
Serial.println(myIP);
Server.on("/", handleRoot);
Server.begin();
Serial.println("HTTP server sterted");
}


void loop() {
// put your main code here, to run repeatedly:
server.handleClient();
}
