import paho.mqtt.client as mqtt
import mysql.connector
import json


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    print("subscribing to topic", "sensor_data")
    client.subscribe("data/time")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    topic=msg.topic
    m_decode=str(msg.payload.decode("utf-8","ignore"))
    print("data Received type",type(m_decode))
    print("data Received",m_decode)
    print("Converting from Json to Object")
    m_in=json.loads(m_decode) #decode json data
    print(type(m_in))
    # print("broker 2 address = ",m_in["broker2"])

broker_address="mosquitto"

print("creating new client instance")
client = mqtt.Client("P1")
print("connecting to mysql db")
cnx = mysql.connector.connect(user='user', password='password',
                              host='db',
                              database='sensorData')
print("connecting to broker")
client.connect(broker_address, 1883, 60)
client.on_connect = on_connect
client.on_message = on_message

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
cnx.close()
