#include <Stepper.h>

const int stepsPerRevolution = 64; 
Stepper myStepper(stepsPerRevolution, 11,9,10,8); 
const int lightSensorPin = A0;  
const int TEMP_LM35 = A1;
static unsigned long previousMillis = 0;
const unsigned long interval = 60000;  // 10분에 해당하는 시간 (밀리초 단위)
const int TRIG = 3;    // 초음파 송신 핀
const int ECHO = 2;   // 초음파 수신 핀
bool Auto = true;
bool AutoMode = true;
bool state = false;   // 커튼 상태 false가 올라가있는 상태
int currentPosition = 0;
int LIGHT = 0;

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(600);
  LIGHT = analogRead(lightSensorPin);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  
  if (Serial.available()) {
    char data = Serial.read();
    if (data == 'F') {
      UPclockwiseRotation();  // 시계 반대 방향으로 모터 회전
    } else if (data == 'B') {
      DowncounterClockwiseRotation();  // 시계 방향으로 모터 회전
    } else if (data == 'S') {
      stopMotor();  // 모터 정지
    } else if (data == 'A') {
      Auto = !Auto;
      LIGHT = analogRead(lightSensorPin);
    } else if (data == 'T') {
      AutoMode = !AutoMode;
    }
  }

  int lightLevel = analogRead(lightSensorPin);
  Serial.print("LIGHT: ");
  Serial.print(lightLevel);
  if (Auto == true){
  
    Serial.print("LIGHT: ");
    Serial.print(lightLevel);
    Serial.print("state: ");
    Serial.println(state);

    if(state == false && lightLevel <= 30){
      if(AutoMode == true){
        DowncounterClockwiseRotation();
        state = true;
      }
    } else if (state == true && lightLevel >= 50 && AutoMode == true){
      UPclockwiseRotation(); 
      state = false;
    }
    if(state == false && lightLevel >= 50){
      if(AutoMode == false){
        DowncounterClockwiseRotation();
        state = true;
      }
    } else if (state == true && lightLevel <= 30){
      if(AutoMode == false){
        UPclockwiseRotation(); 
        state = false;
      }
    }
  }
  
  
  float temp_lm35 = (5.0*analogRead(TEMP_LM35)*100)/1024;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("L:");  // 값의 접두사를 추가하여 보냄
    Serial.println(lightLevel);  // 값을 시리얼 통신으로 보냄
    Serial.print("T:");  // 값의 접두사를 추가하여 보냄
    Serial.println(temp_lm35);  // 값을 시리얼 통신으로 보냄
    Serial.print("T:");  // 값의 접두사를 추가하여 보냄
    Serial.println(temp_lm35);  // 값을 시리얼 통신으로 보냄
    Serial.print("S:");  // 값의 접두사를 추가하여 보냄
    Serial.println(state);  // 값을 시리얼 통신으로 보냄
  }
  
}

void UPclockwiseRotation() {
  long duration, distance;
  int count = 0;
  int lightLevel = analogRead(lightSensorPin);
    // 시계 반대 방향으로 한바퀴 회전
  while(true) {  // 64 * 32 = 2048 한바퀴
    Serial.print("lightLevel: ");
    Serial.print(lightLevel);
    Serial.println(" dd");
    
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    
    duration = pulseIn (ECHO, HIGH); 
    distance = duration * 17 / 1000; 
    // Serial.print("Distance: ");
    // Serial.print(distance);
    // Serial.println(" cm");

    if (distance > 20){
      count = count + 1;
      if(count >= 3){
        stopMotor();  // 모터 정지
        currentPosition = 0;
        state = false;
        return;      // 루프 종료
      }
    }else{
      count = 0;
    }



    currentPosition++;
    myStepper.step(-stepsPerRevolution);

    if (Serial.available()) {
      char data = Serial.read();
      if (data == 'S') {
        stopMotor();  // 모터 정지
        return;      // 루프 종료
      }
    }

  }
}

// 시계 방향으로 모터 회전
void DowncounterClockwiseRotation() {
  int lightLevel = analogRead(lightSensorPin);
  

  long duration, distance;
    // 시계 방향으로 한바퀴 회전
  if(currentPosition < -155){
    stopMotor();  // 모터 정지
    state = true;
    return;      // 루프 종료
  }
  while(true) {
    Serial.print("LIGHT: ");
    Serial.print(lightLevel);
    if(currentPosition < -155){
      stopMotor();  // 모터 정지
      return;      // 루프 종료
    }
    state = true;
    currentPosition--;
    myStepper.step(stepsPerRevolution);

    if (Serial.available()) {
      char data = Serial.read();
      if (data == 'S') {
        stopMotor();  // 모터 정지
        return;      // 루프 종료
      }
    }
  }
}

// 모터 정지
void stopMotor() {
  myStepper.step(0);
}