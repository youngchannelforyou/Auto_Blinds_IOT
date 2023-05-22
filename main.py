from flask import Flask, render_template
import serial

app = Flask(__name__)
ser = serial.Serial("/dev/cu.usbmodem21101", 9600)  # 포트 및 전송 속도를 실제 환경에 맞게 변경

# 모터 제어에 사용할 명령어
MOTOR_FORWARD = "F"
MOTOR_BACKWARD = "B"
MOTOR_STOP = "S"


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/uprun", methods=["POST"])
def uprun():
    send_data(MOTOR_FORWARD)  # 모터를 시계 반대 방향으로 한 스텝 회전
    return "Motor rotated clockwise"


@app.route("/downrun", methods=["POST"])
def downrun():
    send_data(MOTOR_BACKWARD)  # 모터를 시계 반대 방향으로 한 스텝 회전
    return "Motor rotated clockwise"


@app.route("/stoprun", methods=["POST"])
def stoprun():
    send_data(MOTOR_STOP)  # 모터 정지
    return "Motor stopped"


def send_data(data):
    ser.write(data.encode())  # 아두이노로 데이터 전송


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
