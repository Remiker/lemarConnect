import paho.mqtt.client as mqtt
import json
# Define the MQTT broker and port
broker = "eu1.cloud.thethings.network"
port = 1883
topic = "v3/lc-test@ttn/devices/lc-algue/up"
#topic = "v3/+/devices/+/up"
username = "lc-test@ttn"  # Replace with your username
password = "NNSXS.W7SM22UBUMSCDHB6CVEK5IDSH6RSFOUNWNRKUZQ.SZRQNBQNMLUZU343R7Q7EWJ4XQPAO66LUSSUFLI5WJWLM5GM7YPQ"  # Replace with your password
# Define the callback function for connection
def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    if rc == 0:
        print("Connection successful!")
        client.subscribe(topic)  # Subscribe to the topic after successful connection
    else:
        print("Failed to connect. Result code:", rc)

# Define the callback function for message receipt
def on_message(client, userdata, msg):
    x = json.loads(msg.payload.decode('utf-8'))
    decoded_payload = x["uplink_message"]["decoded_payload"]["illuminance"]
    print(str(decoded_payload)) # 
    #print(f"Message received on topic {msg.topic}: {msg.payload.decode()}")

# Create an MQTT client instance
client = mqtt.Client()
client.username_pw_set(username, password)
# Set up the connection and message callback functions
client.on_connect = on_connect
client.on_message = on_message



# Connect to the MQTT broker
client.connect(broker, port, 60)

# Loop to maintain the connection and handle events
client.loop_forever()