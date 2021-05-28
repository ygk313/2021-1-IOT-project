#include <WiFi.h>
#include <WiFiClient.h>
#include "WiFiAP.h"

const char* ssid     = "iptime303";
const char* password = "9809191124";

WiFiServer server(80);

int getHumidity(){
  int humidity = 0;
  if (Serial2.available()){
    humidity = Serial2.read();
    Serial.println(humidity);
  } 
  return humidity;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if(!client.connected()) return;

  Serial.println("new client");
  client.setTimeout(5000);

  String request = client.readStringUntil('\r');
  Serial.println("request: ");
  Serial.println(request);

  while(client.available()) {
    client.read();
  }

  int humidity = getHumidity();
  float temp = 150.0;

  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.print(" Temperature : ");
  Serial.print(temp);
  Serial.println(" C");

  client.print("HTTP/1.1 200 OK");
  client.print("Content-Type: text/html\r\n\r\n");
  client.print("<!DOCTYPE HTML>");
  client.print("<html>");
  client.print("<head>");
  client.print("<meta charset=\"UTF-8\" http-equiv=\"refresh\" content=\"1\">");
  client.print("<title>DHT sensor test webpage</title>");
  client.print("</head>");
  client.print("<body>");
  client.print("<h2>DHT sensor test webpage</h2>");
  client.print("<br>");
  client.print("Temperature : ");
  client.print(temp);
  client.print(" C");
  client.print("<br>");
  client.print("현재 습도 : ");
  client.print(humidity);
  client.print(" %");
  client.print("</body>");
  client.print("</html>");

  Serial.println("client connect end");
}
