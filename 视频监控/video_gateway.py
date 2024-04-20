import cv2
import time
import base64
import random
import sys
from paho.mqtt import client as mqtt_client
import threading
import functools

broker = '118.31.35.191'
port = 1883
client_id = f'python-mqtt-webcam'

topic ="greenhouse/webcam/event/property/post"
def connect_mqtt():
    '''
    client:此回调的客户端实例
    userdata:在 Client() 或 user_data_set() 中设置的私有用户数据
    flags:代理发送的响应标志
    rc:连接结果
    '''
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
    '''
    client:此回调的客户端实例
    userdata:在 Client() 或 user_data_set() 中设置的私有用户数据
    flags:代理发送的响应标志
    rc:断开结果
    '''
    def on_disconnect(client, userdata, rc):
        a = 1
    '''
    client:此回调的客户端实例
    userdata:在 Client() 或 user_data_set() 中设置的私有用户数据
    message:MQTTMessage信息实例，这是一个包含成员 topic、payload、qos、retain 的类。
    sub:特定主题
    callback:定义的callback函数
    '''
    def on_message(client, userdata, message):
        a = 1
    client = mqtt_client.Client(client_id)
    client.username_pw_set("webcam","123456789")
    client.on_connect = on_connect
    client.on_disconnect= on_disconnect
    client.on_message = on_message
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
        ret, frame = cap.read()
        _, jpg_encoded = cv2.imencode('.jpg', frame)
        base64_str = base64.b64encode(jpg_encoded)
        # 使用 lambda 函数创建带有参数的函数
        timer = threading.Timer(0.1, lambda: client.publish(topic, base64_str))
        timer.start()
except KeyboardInterrupt:
    # 释放资源
    cap.release()
    cv2.destroyAllWindows()
    sys.exit()
