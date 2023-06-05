import csv
import datetime
from flask import Flask, request, render_template
import requests
import serial
from Model import Model

app = Flask(__name__)
ser = serial.Serial("/dev/cu.usbmodem1101", 9600)  # 포트 및 전송 속도를 실제 환경에 맞게 변경

# 모터 제어에 사용할 명령어
MOTOR_FORWARD = "F"
MOTOR_BACKWARD = "B"
MOTOR_STOP = "S"
AUTO_MODE = "A"
AUTO_TYPE = "T"
# 상태 변수
AUTO = True
AI = False
LIGHTLEVEL = 0


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/auto", methods=["POST"])
def auto():
    global AUTO
    global AI

    if AI == True:
        AI = False
    if AUTO == True:
        if AI == True:
            return "change auto"
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


@app.route("/ai", methods=["POST"])
def AIrun():
    global AI
    AI = not AI
    # send_data(AUTO_MODE)

    return "Ai mode"


def send_data(data):
    ser.write(data.encode())  # 아두이노로 데이터 전송


if __name__ == "__main__":
    model = Model()
    app.run(host="0.0.0.0", port=8080)
    temp = None
    light = None
    state = None

    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode().strip()

            if data.startswith("L:"):
                light = data[2:]  # 접두사를 제외한 거리 데이터 추출
                print(light, temp)
            if data.startswith("T:"):
                temp = data[2:]  # 접두사를 제외한 조도 데이터 추출
                print(light, temp)
            if data.startswith("S:"):
                state_value = data[2:]  # 접두사를 제외한 조도 데이터 추출
                state = int(state_value)

        current_date = datetime.date.today()
        current_time = datetime.datetime.now()
        hour = current_time.hour
        minute = current_time.minute

        combined_value = hour * 100 + minute

        if current_date.day == 1:
            model.train_model()

        if AUTO == True:
            if AI == True:
                current_time = datetime.datetime.now().time()
                if current_time.minute % 10 == 0:
                    data = [combined_value, temp, light]
                    prediction = model.predict(data)
                    if prediction == 1:
                        send_data(MOTOR_FORWARD)
                    else:
                        send_data(MOTOR_BACKWARD)
        print(light, temp, state)
        if temp != None and light != None and state != None:
            # CSV 파일 열기 및 읽기
            with open("blind_data.csv", "r") as file:
                reader = csv.reader(file)
                rows = list(reader)

            # 새로운 행 추가
            new_row = [combined_value, temp, light, state]  # 새로운 행 데이터
            rows.append(new_row)

            # CSV 파일 열기 및 쓰기
            with open("blind_data.csv", "w", newline="") as file:
                writer = csv.writer(file)
                writer.writerows(rows)
            temp = None
            light = None
            state = None
