from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse
app = FastAPI()
# from influxdb import InfluxDBClient

import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

import paho.mqtt.client as mqtt
import json
import asyncio

#MONGO_URI = "mongodb://Jad:XkI1fA00Zp1ioyQl@193.54.246.79:27017/<LemarConnect>?ssl=true&authSource=admin"  # Replace with your MongoDB URI
#DATABASE_NAME = "lemarconnect"  # Replace with your database name
#COLLECTION_NAME = "payload"  # Replace with your collection name

# token = os.environ.get("INFLUXDB_TOKEN")
token = "dbpeD9hPjiWm4afsX5fEe0lUpAI6QtTeiXmkpTxrQgwfY58TzWscagn0Sr_M6jiD8kl-A3ZF-PA0LcRP2pLlNg=="
org = "IUT_Brest"
# url = "http://localhost:8086"
url = "http://localhost:8086"
write_client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)

bucket="lemarconnect"
# clientdb = influxdb_client()
write_api =  write_client.write_api(write_options=SYNCHRONOUS)
   
# point = Point("test_measurement").field("field1", 42)
# try:
#     write_api.write(bucket=bucket, org=org, record=point)
#     print("Write successful")
# except Exception as e:
#     print(f"Write failed: {e}")
    


#Configurer votre connexion InfluxDB
#influxdb_host = 'localhost'  # Remplacez par l'adresse de votre serveur InfluxDB
#influxdb_port = 8086          # Port par défaut pour InfluxDB
#influxdb_database = 'lemarconnect'    # Remplacez par le nom de votre base de données
#influxdb_user = 'Jad'   # Remplacez par votre nom d'utilisateur
#influxdb_password = 'jadjadjad'  # Remplacez par votre mot de passe

#Création du client InfluxDB
#clientDB = InfluxDBClient(host=influxdb_host, port=influxdb_port, username=influxdb_user, password=influxdb_password, database=influxdb_database)
# Connect to MongoDB
# client = MongoClient(MONGO_URI)
# db = client[DATABASE_NAME]
# collection = db[COLLECTION_NAME]

# Define the MQTT broker and port
broker = "eu1.cloud.thethings.network"
port = 1883
topic = "v3/lc-test@ttn/devices/lc-algue/up"

#topic = "v3/+/devices/+/up"
username = "lc-test@ttn"  # Replace with your username
password = "NNSXS.W7SM22UBUMSCDHB6CVEK5IDSH6RSFOUNWNRKUZQ.SZRQNBQNMLUZU343R7Q7EWJ4XQPAO66LUSSUFLI5WJWLM5GM7YPQ"  # Replace with your password
# Define the callback function for connection
def on_connect(clientmqtt, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    if rc == 0:
        print("Connection successful!")
        clientmqtt.subscribe(topic)  # Subscribe to the topic after successful connection
    else:
        print("Failed to connect. Result code:", rc)


decoded_payload = {"value": None}
#decoded_payload = 0
# Define the callback function for message receipt
message_id = 0
# Callback for receiving MQTT messages
def on_message(client, userdata, msg):
    global message_id
    try:
        # Parse the MQTT message
        x = json.loads(msg.payload.decode("utf-8"))
        decoded_payload["value"] = x["uplink_message"]["decoded_payload"]["humidity"]
        print(f"Decoded Payload: {decoded_payload['value']}")

        # json_body = [
        #         {
        #             "measurement": "your_measurement",  # Remplacez par le nom de votre mesure
        #             "tags": {
        #                 "device": "lc-algue"  # Remplacez par des tags selon vos besoins
        #             },
        #             "fields": {
        #                 "value": decoded_payload["value"]  # Remplacez par les champs que vous souhaitez stocker
        #                 }
        #         }
        #     ]
        # clientDB.write_points(json_body)
        # Add metadata and store it in MongoDB

        # document = {
        #     id : message_id,
        #     "payload": decoded_payload["value"],
        #     # "timestamp": datetime.utcnow(),
        #     "topic": msg.topic,
        # }
        # message_id += 1 
        # collection.insert_one(document)
        print("Data successfully inserted into MongoDB")
    except Exception as e:
        print(f"Error processing message: {e}")

# Create an MQTT client instance
clientmqtt = mqtt.Client()
clientmqtt.username_pw_set(username, password)
# Set up the connection and message callback functions
clientmqtt.on_connect = on_connect
clientmqtt.on_message = on_message



# Connect to the MQTT broker
clientmqtt.connect(broker, port,10)
clientmqtt.loop_start()
# Loop to maintain the connection and handle events

#client.loop_forever()

async def event_stream():
    while True:
        if decoded_payload["value"] is not None:
            # Send the latest payload to the client
            yield f"data: {json.dumps(decoded_payload['value'])}\n\n"
        await asyncio.sleep(1)  # Send updates every second

@app.get("/events")
async def sse_endpoint():
    return StreamingResponse(event_stream(), media_type="text/event-stream")

@app.get("/")
async def root():
    for value in range(50):
            point = (
                Point("measurement1")
                .tag("_measurement", "boltdb_reads_total")
                .field("counter", value)
            )
    write_api.write(bucket=bucket, org=org, record=point)
    print("exited")
    time.sleep(1) # separate points by 1 second
    # if decoded_payload["value"] is None:
    #     return {"message": "No payload received yet"}
    return {"decoded_payload": decoded_payload["value"]}
