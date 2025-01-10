from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import StreamingResponse

app = FastAPI()

import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

import paho.mqtt.client as mqtt
import json
import http
import requests
# Define token, org, url and bucket for influxDB after creating a user on influxDB server
token = "dbpeD9hPjiWm4afsX5fEe0lUpAI6QtTeiXmkpTxrQgwfY58TzWscagn0Sr_M6jiD8kl-A3ZF-PA0LcRP2pLlNg=="
org = "IUT_Brest"
url = "http://influxdb_docker:8086"
bucket="lemarconnect"
write_client = influxdb_client.InfluxDBClient(url=url, token=token, org=org) # define write_client for influxDB

write_api =  write_client.write_api(write_options=SYNCHRONOUS) # initialize write api for influxDB 
    

# Define the MQTT broker and port
broker = "eu1.cloud.thethings.network"
port = 1883
# topic = "v3/lc-test@ttn/devices/lc-algue/up"
topic = "v3/lc-esp32@ttn/devices/lc-esp32-1/up"

#topic = "v3/+/devices/+/up" # gets all the devices on the account
username = "lc-esp32@ttn"
password = "NNSXS.FS5OHJU7VMO6LBMMK3HMEEUAHRXA4WZTUNFFHDA.2P4JP5KCTUSOXPCVA3AK3BECGL5YVIYY2EE4RQLVJZ53CMERZ2PQ"
# username = "lc-test@ttn"  # Replace with your username
# password = "NNSXS.W7SM22UBUMSCDHB6CVEK5IDSH6RSFOUNWNRKUZQ.SZRQNBQNMLUZU343R7Q7EWJ4XQPAO66LUSSUFLI5WJWLM5GM7YPQ"  # Replace with your password
# Define the callback function for connection
def on_connect(clientmqtt, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    if rc == 0:
        print("Connection successful!")
        clientmqtt.subscribe(topic)  # Subscribe to the topic after successful connection
    else:
        print("Failed to connect. Result code:", rc)


humidity = 0 # initialize the variable globaly to be used later
temperature = 0
lumo = 0
pressure = 0
# Define the callback function for message receipt
# Callback for receiving MQTT messages
def on_message(client, userdata, msg): 
    global message_id
    try:
        # Parse the MQTT message
        x = json.loads(msg.payload.decode("utf-8"))
        humidity = x["uplink_message"]["decoded_payload"]["humidity"] # get the value of humidity only
        # print(f"Decoded Payload: ", humidity)
        temperature = x["uplink_message"]["decoded_payload"]["temperature"] # get the value of humidity only
        lumo = x["uplink_message"]["decoded_payload"]["illuminance"] # get the value of humidity only
        # pressure= x["uplink_message"]["decoded_payload"]["pressure"] # get the value of humidity only
        sendDataToInfluxDB(humidity,"humidity") # sending data to influxDB, for now only humidity
        sendDataToInfluxDB(temperature,"temperature")
        sendDataToInfluxDB(lumo,"illuminance")
        # sendDataToInfluxDB(pressure,"pressure")
        
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

def sendDataToInfluxDB(value,field):
    point = (
                Point("measurement1")
                .tag("source", field)
                .field("counter_float", float(value))
            )
    write_api.write(bucket=bucket, org=org, record=point)

################# not needed! ##########

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
    x=0
    while(x<5):
        sendDataToInfluxDB(51+x,"temp")
        time.sleep(1)
        x+=1
    

        # querys = """from(bucket: "lemarconnect")
        # |> filter(fn: (r) => r._measurement == "average_temperature")
        # |> mean()
        # |> map(fn: (r) => ({r with jsonStr: string(v: json.encode(v: {"location": r.location, "mean": r._value}))}))
        # |> map(
        #     fn: (r) => ({
        #         r with
        #         status_code: http.post(
        #             url: "http://somehost.com/",
        #             headers: {x: "a", y: "b"},
        #             data: bytes(v: r.jsonStr)
        #         )
        #     })"""

    # query = """from(bucket: "lemarconnect")
    # |> range(start: -30m)
    # |> filter(fn: (r) => r._measurement == "measurement1")"""    
    # write_client.delete_api.delete()

    # query to receive the values that have been registered on influxdb within the last 30 minutes, on point "measurement1"
    query_api = write_client.query_api()
    query = """from(bucket: "lemarconnect")
    |> range(start: -30m)
    |> filter(fn: (r) => r._measurement == "measurement1")"""
    tables = query_api.query(query, org=org) 

    for table in tables:
        for record in table.records:
            print(record)
            
    # print("exited")
    
    if humidity == 0:
        return {"message": "No payload received yet"}
    return {"decoded_payload": humidity} # display the value on python_docker server


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
