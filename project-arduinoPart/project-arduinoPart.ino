//물을 주고 있는 지 확인하는 LED 핀
int led_pin = 8;
//토양 수분 센서 값
int soil = A0;

//모터 드라이버 출력 핀모드
int pump_A= 10;
int pump_B = 6;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(pump_A, OUTPUT);
  pinMode(pump_B, OUTPUT);
  pinMode(soil, INPUT);
  
  Serial.begin(115200);
  Serial1.begin(115200);
  //버퍼 비우기
  Serial1.flush();
}
//Serial통신으로 받는 값
int data = 0;

void loop() {
  //토양수분센서 아날로그 입력
  int soil = analogRead(soil);
  // 현재 토양 수분량 Node MCU에 전달할 변수
  int current_soil = 0;
  Serial.println(soil);

  //버퍼 비우기
  Serial1.flush();

  // 0(습합)~1023(건조함) 450 이하면 - 습함
  if (soil <= 450){
    Serial1.write(2);
    current_soil = 2;
  }
  // 450이상 1000이하 - 적당함
  else if (450 < soil && soil<=1000){
    Serial1.write(1);
    current_soil = 1;
  }
  //1000이상 - 건조함
  else if (soil>1000){
    Serial1.write(0);
    current_soil = 0;
  }

  //Node MCU에서 전달받은 값이 있는 경우
  if (Serial1.available()){
    data = Serial1.read();
    Serial1.flush();
    Serial.print("data:");
    Serial.println(data);
  }
  //Node MCU에서 전달 받은 값이 1이면,
  if (data == 1){
    //led 켜기
    digitalWrite(led_pin, HIGH);
    //워터 펌프 정방향으로 돌리기
    digitalWrite(pump_A, HIGH);
    digitalWrite(pump_B,LOW);
    //3초 동안 쉬기
    delay(3000);
    //워터 펌프 멈추기
    digitalWrite(pump_A, LOW);
    digitalWrite(pump_B, LOW);
    //led 끄기
    digitalWrite(led_pin, LOW);
    
    data = 0;
  }
  else if (data == 0){
    digitalWrite(led_pin,LOW);
  }
}
