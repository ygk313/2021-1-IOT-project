#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  dht.begin();

}

void loop() {
  int humidity = dht.readHumidity();
  Serial.print("현재 습도:");
  Serial.print(humidity);
  Serial.print("%");
  Serial.println();
  Serial1.write(humidity);
  delay(1000);
}
