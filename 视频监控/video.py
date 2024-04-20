import cv2
import time
import base64
import random
import sys
from paho.mqtt import client as mqtt_client


broker = '118.31.35.191'
port = 1883
client_id = f'python-mqtt-{999}'

topic ="greenhouse/webcam/event/property/post"
def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set("webcam","123456789")
    client.on_connect = on_connect
    client.connect(broker, port)
    return client
# 打开摄像头
cap = cv2.VideoCapture(0)  # 0 表示默认的摄像头
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cap.set(cv2.CAP_PROP_FPS, 30)
client = connect_mqtt()
client.loop_start()
try:
    while True:
        # 读取一帧
        start_time = time.perf_counter()
        ret, frame = cap.read()
        _, jpg_encoded = cv2.imencode('.jpg', frame)
        bpg_str = base64.b64encode(jpg_encoded)
        client.publish(topic, bpg_str)
        end_time = time.perf_counter()
except KeyboardInterrupt:
    # 释放资源
    cap.release()
    cv2.destroyAllWindows()
    sys.exit()
