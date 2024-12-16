from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse

app = FastAPI()

import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

import paho.mqtt.client as mqtt
import json
import asyncio

# Define token, org, url and bucket for influxDB after creating a user on influxDB server
token = "dbpeD9hPjiWm4afsX5fEe0lUpAI6QtTeiXmkpTxrQgwfY58TzWscagn0Sr_M6jiD8kl-A3ZF-PA0LcRP2pLlNg=="
org = "IUT_Brest"
url = "http://influxdb_docker:8086"
bucket="lemarconnect"
write_client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)

write_api =  write_client.write_api(write_options=SYNCHRONOUS)
    

# Define the MQTT broker and port
broker = "eu1.cloud.thethings.network"
port = 1883
topic = "v3/lc-test@ttn/devices/lc-algue/up"

#topic = "v3/+/devices/+/up" # gets all the devices on the account
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


decoded_payload = {"value": None} # initialize the variable globaly to be used later
# Define the callback function for message receipt
# Callback for receiving MQTT messages
def on_message(client, userdata, msg):
    global message_id
    try:
        # Parse the MQTT message
        x = json.loads(msg.payload.decode("utf-8"))
        decoded_payload["value"] = x["uplink_message"]["decoded_payload"]["humidity"] # get the value of humidity only
        print(f"Decoded Payload: {decoded_payload['value']}")
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
# Loop to maintain the connection and handle events, it starts on different thread so it doesn't block the code
clientmqtt.loop_start()


# async def event_stream():
#     while True:
#         if decoded_payload["value"] is not None:
#             # Send the latest payload to the client
#             yield f"data: {json.dumps(decoded_payload['value'])}\n\n"
#         await asyncio.sleep(1)  # Send updates every second

# @app.get("/events")
# async def sse_endpoint():
#     return StreamingResponse(event_stream(), media_type="text/event-stream")

@app.get("/")
async def root():
    value = 0
    # write_client.delete_api.delete()
    while(value < 50):
            point = (
                Point("measurement1")
                .tag("source", "test")
                .field("counter", value)
            )
            value +=1
            time.sleep(5) # separate points by 5 second
    # query to receive the values that have been registered on influxdb within the last 30 minutes, on point "measurement1"
    write_api.write(bucket=bucket, org=org, record=point)
    query_api = write_client.query_api()
    query = """from(bucket: "lemarconnect")
    |> range(start: -30m)
    |> filter(fn: (r) => r._measurement == "measurement1")"""
    tables = query_api.query(query, org=org) 

    for table in tables:
        for record in table.records:
            print(record)
            
    print("exited")
    
    if decoded_payload["value"] is None:
        return {"message": "No payload received yet"}
    return {"decoded_payload": decoded_payload["value"]} # display the value on python_docker server


#################### to be used later ################

# start_time = "1970-01-01T00:00:00Z"  # Beginning of time
# stop_time = datetime.utcnow().isoformat() + "Z"  # Current time in UTC

# # Specify the predicate (optional, filter by measurement, tag, or field)
# predicate = '_measurement="your_measurement_name"'  # Replace with your measurement

# # Perform the deletion
# try:
#     delete_api.delete(start_time, stop_time, predicate, bucket=bucket, org=org)
#     print("Data deleted successfully.")
# except Exception as e:
#     print(f"Failed to delete data: {e}")
