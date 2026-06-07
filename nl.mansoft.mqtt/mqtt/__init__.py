import display
import buttons
import network
import ubinascii
from umqtt.simple import MQTTClient
import mch22
import time
import ucollections
import esp32
import nvs

def mqtt_display(lines):
	display.drawFill(0x000000)
	y = 100
	for line in lines:
		display.drawText(5, y, line[0], line[1], "dejavusans20")
		y += 30
	display.flush()

def on_action_btn(button, pressed):
    try:
        event_queue.append((button, pressed))
        print(f'{button} {pressed}')
    except IndexError:
        pass  # queue full, drop event

def on_action_btn_a(pressed):
	on_action_btn("A", pressed)

def on_action_btn_b(pressed):
	on_action_btn("B", pressed)

def on_action_btn_home(pressed):
	on_action_btn("Home", pressed)
	if not(pressed):
		mch22.exit_python()

def on_action_btn_menu(pressed):
	on_action_btn("Menu", pressed)

def on_action_btn_select(pressed):
	on_action_btn("Select", pressed)

def on_action_btn_start(pressed):
	on_action_btn("Start", pressed)

def on_action_btn_left(pressed):
	on_action_btn("Left", pressed)

def on_action_btn_right(pressed):
	on_action_btn("Right", pressed)
	
def on_action_btn_up(pressed):
	on_action_btn("Up", pressed)

def on_action_btn_down(pressed):
	on_action_btn("Down", pressed)

def on_action_btn_press(pressed):
	on_action_btn("press", pressed)

def subscribe_callback(topic, msg):
	mqtt_display([
		("Last received:", 0x00ffff),
		("on Topic:", 0x00ff00),
		(topic.decode(), 0xff0000),
		("Payload:", 0x00ff00),
		(msg.decode(), 0xff0000)
	])

def nvs_blob_to_string(nvs, key, dflt=None):
    try:
        size = mynvs.get_blob(key, bytearray(0))
    except OSError:
        return dflt

    if not size:
        return dflt

    buf = bytearray(size)
    nvs.get_blob(key, buf)
    return buf.decode("utf-8")

event_queue = ucollections.deque((), 32)

mynvs = esp32.NVS("nl.mansoft.mqtt")
broker = nvs_blob_to_string(mynvs, "broker", "broker.hivemq.com")
publish_topic = nvs_blob_to_string(mynvs, "publish", "henripub")
subscribe_topic = nvs_blob_to_string(mynvs, "subscribe", "henrisub")
broker_user = nvs_blob_to_string(mynvs, "user")
broker_password = nvs_blob_to_string(mynvs, "password")

sta_if = network.WLAN(network.WLAN.IF_STA)
sta_if.active(True)
while not sta_if.active():
    time.sleep(0.2)

index = 0
last_ssid = sta_if.config("ssid")
while (ssid := nvs.nvs_getstr("wifi", f"s{index:02d}.ssid")) and ssid != last_ssid:
    index = index + 1
password = nvs.nvs_getstr("wifi", f"s{index:02d}.password")

mqtt_display([
	(f"Connecting to {ssid}", 0x00ff00),
])

sta_if.connect(ssid, password)

while not sta_if.isconnected():
    time.sleep(0.2)
    
client_id = "tanmatsu-" + ubinascii.hexlify(sta_if.config("mac")[-3:]).decode()

mqttClient = MQTTClient(client_id, broker, keepalive=60, user=broker_user, password=broker_password, ssl=(broker_password is not None))

mqttClient.connect()

mqttClient.set_callback(subscribe_callback)
mqttClient.subscribe(subscribe_topic)

mqtt_display([
	("Broker:", 0x00ff00),
	(broker, 0xff0000),
	("Publishing on Topic:", 0x00ff00),
	(publish_topic, 0xff0000),
	("Subscribing to Topic:", 0x00ff00),
	(subscribe_topic, 0xff0000)
])

last_ping = time.time()

buttons.attach(buttons.BTN_A, on_action_btn_a)
buttons.attach(buttons.BTN_B, on_action_btn_b)
buttons.attach(buttons.BTN_HOME, on_action_btn_home)
buttons.attach(buttons.BTN_MENU, on_action_btn_menu)
buttons.attach(buttons.BTN_SELECT, on_action_btn_select)
buttons.attach(buttons.BTN_START, on_action_btn_start)
buttons.attach(buttons.BTN_LEFT, on_action_btn_left)
buttons.attach(buttons.BTN_RIGHT, on_action_btn_right)
buttons.attach(buttons.BTN_UP, on_action_btn_up)
buttons.attach(buttons.BTN_DOWN, on_action_btn_down)
buttons.attach(buttons.BTN_PRESS, on_action_btn_press)

while True:
    # Handle MQTT incoming messages
    mqttClient.check_msg()
    
    # ping regularly
    if time.time() - last_ping > 30:
        mqttClient.ping()
        last_ping = time.time()

    # Handle queued button events
    if event_queue:
        button, pressed = event_queue.popleft()
        payload = client_id + ":\n" + button + " " + ("pressed" if pressed else "released")
        mqttClient.publish(publish_topic, payload)
        mqtt_display([
            ("Last published:", 0x00ffff),
            ("Topic:", 0x00ff00),
            (publish_topic, 0xff0000),
            ("Payload:", 0x00ff00),
            (payload, 0xff0000)
        ])
