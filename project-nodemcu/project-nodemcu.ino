#include <WiFi.h>
#include <WiFiClient.h>
#include "WiFiAP.h"
#include "DHT.h"
#define DHTPIN 19
#define DHTTYPE DHT11

//온습도 센서
DHT dht(DHTPIN, DHTTYPE);

//물주기 변수
int watering = 0;
int soilVal = 0;

const char* ssid     = "iptime303";
const char* password = "9809191124";

WiFiServer server(80);

void setup() {
  //온습도 센서 연결
  pinMode(4, INPUT);
  
  Serial.begin(115200);
  Serial2.begin(115200);
  dht.begin();

  //와이파이 연결
  WiFi.begin(ssid, password);

  //와이파이 연결 기다리기
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
  //client 생성
  WiFiClient client = server.available();
  if(!client.connected()) return;

  Serial.println("new client");
  client.setTimeout(5000);

  String request = client.readStringUntil('\r');
  Serial.println("request: ");
  Serial.println(request);
  
  Serial.print("watering:");
  //만약 물을 주는 링크를 눌렀다면
  if (request.indexOf(F("/Y"))!=-1){
    watering = 1;
    Serial.println("1");
  }
  //만약 물주는 링크를 누르지 않았다면,
  else if (request.indexOf(F("/N")) != -1){
    watering = 0;
    Serial.println("0");
  }
  else{
    Serial.println(F("invalid request"));
  }

  while(client.available()) {
    client.read();
  }
  
  int humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (Serial2.available()){
    soilVal = Serial2.read();
    Serial.println(soilVal);
  }
  
  Serial.print("Humidity : ");
  Serial.println(humidity);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  
  Serial.print("SoilHumidity:");
  Serial.println(soilVal);
  
  client.print("HTTP/1.1 200 OK");
  client.print("Content-Type: text/html\r\n\r\n");
  client.print("<!DOCTYPE HTML>");
  client.print("<html>");
  client.print("<head>");
  client.print("<meta charset=\"UTF-8\">");
  client.print("<title>몬스테라 키우기 :) </title>");
  client.print("</head>");
  client.print("<body>");
  client.print("<h2>몬스테라 키우기 :) </h2>");
  client.print("<br>");
  client.print("<h4>몬스테라는 공기를 정화하는 관엽 식물이다.<br>잎의 모양도 예뻐 인테리어를 하기에도 좋다.</h4>");
  client.print("<br> 몬스테라의 적정 습도: 70%이상");
  client.print("<br> 몬스테라의 적정 온도: 13도 ~ 20도");
  client.print("<br> 몬스테라의 물 주기: 봄, 여름, 가을은 흙은 촉촉하게 유지, 겨울은 토양의 표면이 마르면 주기");
  client.print("<br>");
  client.print("<br><hr>");
  client.print("<br><h2> 실내 온습도 및 토양의 습도</h2>");
  client.print("<br>현재 공기의 습도 : ");
  client.print(humidity);
  client.print(" %");
  client.print("<br> 현재 실내 온도 : ");
  client.print(temperature);
  client.print("도");
  client.print("<br> 현재 토양의 습도 : ");
  if (soilVal == 0){
    client.print("건조합니다.");
  }
  else if (soilVal == 1){
    client.print("적당합니다.");
  }
  else if (soilVal == 2){
    client.print("습합니다.");
  }

  client.print("<br>");
 
  client.print("<br><hr>");
  client.print("<br><h2>물주기를 선택해보자</h2>");
  client.print(F("<br> <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/Y'>물주기</a>를 원한다면 누르세요! "));
  client.print(F("아니면 <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/N'>물 주지 않기</a>를 원한다면 누르세요!"));
  client.print("</body>");
  client.print("</html>");

  Serial2.flush();
  if (watering == 0){
   Serial2.write(0); 
  }
  else if (watering == 1){ 
    Serial2.write(1);   
  }
  
  Serial.println();
  Serial.println("client connect end");
}
