import cv2
import time
import base64
import random
import sys
from paho.mqtt import client as mqtt_client
import mysql.connector
import queue
import numpy as np


broker = '118.31.35.191'
port = 1883
client_id = f'python-mqtt-image_recv'
topic ="greenhouse/webcam/event/property/post"

base64_str_queue = queue.Queue()

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
        client.subscribe(topic)
    def on_message(client, userdata, message):
        base64_str_queue.put(message.payload)

    client = mqtt_client.Client(client_id)
    client.username_pw_set("image_recv","123456789")
    client.on_message = on_message
    client.on_connect = on_connect
    client.connect(broker, port)
    return client
client = connect_mqtt()
client.loop_start()
conn = mysql.connector.connect(
                host="localhost",
                user="root",
                passwd="a2547682164..",
                database="database1",
                autocommit=True  # 设置自动提交事务
            )
cursor = conn.cursor()
while True:
    try:
        start_time = time.perf_counter()
        base64_str = base64_str_queue.get()
        image_data = base64.b64decode(base64_str)
        decoded_image = cv2.imdecode(np.frombuffer(image_data, np.uint8), cv2.IMREAD_COLOR)

        cv2.imshow('frame',decoded_image)
        # 等待 100 毫秒
        cv2.waitKey(1)
        cursor.execute("INSERT INTO video_frame (base64str, timestamp) VALUES (%s, NOW())", (base64_str,))
        end_time = time.perf_counter()
        # print(end_time - start_time)
        # if(end_time - start_time) < 0.08:
        #     time.sleep(0.1 - (end_time - start_time))
    except KeyboardInterrupt:
        cursor.close()
        conn.close()
        sys.exit()