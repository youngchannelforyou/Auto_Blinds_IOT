from flask import Flask, request, render_template
import requests
import serial

app = Flask(__name__)
ser = serial.Serial("/dev/cu.usbmodem11301", 9600)  # 포트 및 전송 속도를 실제 환경에 맞게 변경

# 모터 제어에 사용할 명령어
MOTOR_FORWARD = "F"
MOTOR_BACKWARD = "B"
MOTOR_STOP = "S"
AUTO_MODE = "A"
AUTO_TYPE = "T"
# 상태 변수
AUTO = True
LIGHTLEVEL = 0


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/auto", methods=["POST"])
def auto():
    global AUTO
    AUTO = not AUTO
    send_data(AUTO_MODE)
    print("log.change auto")
    return "change auto"


@app.route("/autotype", methods=["POST"])
def autotype():
    send_data(AUTO_TYPE)
    print("log.change auto type")
    return "change auto type"


@app.route("/uprun", methods=["POST"])
def uprun():
    send_data(MOTOR_FORWARD)  # 모터를 시계 반대 방향으로 한 스텝 회전
    print("log.run motor")
    return "Motor rotated clockwise"


@app.route("/downrun", methods=["POST"])
def downrun():
    send_data(MOTOR_BACKWARD)  # 모터를 시계 반대 방향으로 한 스텝 회전
    print("log.run motor")
    return "Motor rotated clockwise"


@app.route("/stoprun", methods=["POST"])
def stoprun():
    send_data(MOTOR_STOP)  # 모터 정지
    print("log.stop motor")
    return "Motor stopped"


def send_data(data):
    ser.write(data.encode())  # 아두이노로 데이터 전송


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)

while True:
    if ser.in_waiting > 0:
        data = ser.readline().decode().strip()
        distance = None
        lightLevel = None
        if data.startswith("D:"):
            distance = data[2:]  # 접두사를 제외한 거리 데이터 추출
        if data.startswith("L:"):
            lightLevel = data[2:]  # 접두사를 제외한 조도 데이터 추출

        print(distance, lightLevel)
