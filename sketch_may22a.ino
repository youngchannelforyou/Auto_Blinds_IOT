#include <Stepper.h>

const int stepsPerRevolution = 64; 
Stepper myStepper(stepsPerRevolution, 11,9,10,8); 
void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(300);
}
void loop() {
  if (Serial.available()) {
    char data = Serial.read();
    if (data == 'F') {
      clockwiseRotation();  // 시계 반대 방향으로 모터 회전
    } else if (data == 'B') {
      counterClockwiseRotation();  // 시계 방향으로 모터 회전
    }
    else if (data == 'S') {
      stopMotor();  // 모터 정지
    }
  }
}

// 시계 반대 방향으로 모터 회전
void clockwiseRotation() {
    // 시계 반대 방향으로 한바퀴 회전
  for(int i=0; i<32; i++) {  // 64 * 32 = 2048 한바퀴
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
void counterClockwiseRotation() {
    // 시계 방향으로 한바퀴 회전
  for(int i=0; i<32; i++) {
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