import re

import paho.mqtt.client as mqtt
import mysql.connector
import json


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    print("subscribing to topic", "sensor_data")

    table = (
        "CREATE TABLE IF NOT EXISTS`record` ("
        "  `recordID` int(11) NOT NULL AUTO_INCREMENT,"
        "  `sensorData` int(11) NOT NULL,"
        "  `sensorID` varchar(20) NOT NULL,"
        "  `timestamp` TIMESTAMP NOT NULL,"
        "  PRIMARY KEY (`recordID`)"
        ") ENGINE=InnoDB")
    cursor.execute(table)

    client.subscribe('data/time')


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    insert = ("INSERT INTO `record`"
              "(sensorData, sensorID, timestamp) "
              "VALUES (%s, %s, %s)")

    topic = msg.topic
    m_decode = str(msg.payload.decode("utf-8", "ignore"))
    # print("data Received type",type(m_decode))
    # print("data Received",m_decode)
    # print("Converting from Json to Object")
    m_in = json.loads(m_decode)  # decode json data
    m_in["timestamp"] = format_timestamp(m_in["timestamp"])
    data = (m_in["sensorData"], m_in["sensorId"], m_in["timestamp"])
    # print(data)
    # print(data[0])
    if int(data[0]) < 10:
        broadcastFault(m_decode)

    cursor.execute(insert, data)
    cnx.commit()


def format_timestamp(timestamp: str):
    return re.sub("[a-zA-Z]", " ", timestamp)


def broadcastFault(bottle):
    print(bottle)
    print(type(bottle))
    client.publish(topic="fault_topic", payload=bottle, retain=True)
    print("yes")

# print("broker 2 address = ",m_in["broker2"])


broker_address = "mosquitto"

print("creating new client instance")
client = mqtt.Client("P1")
print("connecting to mysql db")
cnx = mysql.connector.connect(user='user', password='password',
                              host='db',
                              database='sensorData')
cursor = cnx.cursor()
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
