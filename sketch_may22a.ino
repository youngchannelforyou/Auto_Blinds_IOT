#include <Stepper.h>

const int stepsPerRevolution = 64; 
Stepper myStepper(stepsPerRevolution, 11,9,10,8); 
const int lightSensorPin = A0;  

const int TRIG = 3;    // 초음파 송신 핀
const int ECHO = 2;   // 초음파 수신 핀
bool Auto = true;
bool AutoMode = true;
int currentPosition = 0;
int LIGHT = 0;

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(300);
  UPclockwiseRotation();
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

  if (Auto == true){
    int lightLevel = analogRead(lightSensorPin);

    if(LIGHT > 500 || lightLevel < 400){
      LIGHT = analogRead(lightSensorPin);
      if(AutoMode == true){
        DowncounterClockwiseRotation();
      } else {
        UPclockwiseRotation(); 
      }
    } else if (LIGHT < 400 || lightLevel > 500){
      LIGHT = analogRead(lightSensorPin);
      if(AutoMode == true){
        UPclockwiseRotation(); 
      } else {
        DowncounterClockwiseRotation();
      }
    }
    
    delay(500); // 일정 시간 대기
  }
}

// 시계 반대 방향으로 모터 회전
void UPclockwiseRotation() {
  long duration, distance;
    // 시계 반대 방향으로 한바퀴 회전
  while(true) {  // 64 * 32 = 2048 한바퀴
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration = pulseIn (ECHO, HIGH); 
    distance = duration * 17 / 1000; 
    if (distance > 4){
      stopMotor();  // 모터 정지
      currentPosition = 0;
      return;      // 루프 종료
    }

    currentPosition++;
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

// 시계 방향으로 모터 회전
void DowncounterClockwiseRotation() {
  long duration, distance;
    // 시계 방향으로 한바퀴 회전
  while(true) {
    if(currentPosition < -100){
      stopMotor();  // 모터 정지
      return;      // 루프 종료
    }

    currentPosition--;
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

// 모터 정지
void stopMotor() {
  myStepper.step(0);
}