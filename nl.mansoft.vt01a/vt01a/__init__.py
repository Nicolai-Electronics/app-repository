import usocket
import display
import network
import buttons
import mch22
import time
import nvs

WIDTH = display.width()
HEIGHT = display.height()

SIZE = min(WIDTH, HEIGHT)
X0 = (WIDTH  - SIZE) // 2
Y0 = (HEIGHT - SIZE) // 2

GREEN = 0x00FF00
BLACK = 0x000000

PORT = 4444

# ------------------------------------------------------------
# VT01 coordinate conversion (12‑bit signed → screen pixels)
# ------------------------------------------------------------
def vt_to_screen(x, y):
    # Convert VT01 coords (-2048..+2047) into 0..SIZE-1
    sx = int((0.5 + x / 4096.0) * SIZE)
    sy = int((0.5 + y / 4096.0) * SIZE)

    # Center inside the display
    return X0 + sx, Y0 + sy

# ------------------------------------------------------------
# VT01 protocol helpers
# ------------------------------------------------------------
def read16(sock):
    data = sock.recv(2)
    if len(data) < 2:
        raise OSError("connection closed")

    lo = data[0]
    hi = data[1]

    bit12 = ((hi << 8) | lo) & 0xFFF
    return bit12 - 0x1000 if (bit12 & 0x800) else bit12

# ------------------------------------------------------------
# Drawing primitives mapped to MCH22 API
# ------------------------------------------------------------
def erase():
    display.drawFill(BLACK)
    display.flush()

def draw_point(x, y):
    sx, sy = vt_to_screen(x, y)
    display.drawPixel(sx, sy, GREEN)

def draw_line(x1, y1, x2, y2):
    sx1, sy1 = vt_to_screen(x1, y1)
    sx2, sy2 = vt_to_screen(x2, y2)
    display.drawLine(sx1, sy1, sx2, sy2, GREEN)

# ------------------------------------------------------------
# Command handler
# ------------------------------------------------------------
def command_handler(sock):
    while True:
        cmd = sock.recv(1)
        if not cmd:
            break

        cmd = cmd[0]

        # Printable ASCII passthrough
        if 0x20 <= cmd <= 0x7F or cmd in (0x0D, 0x0A):
            print(chr(cmd), end="")
            continue

        # Binary commands
        if cmd == 0x01:  # ERASE
            erase()

        elif cmd == 0x02:  # PLOT POINT
            x = read16(sock)
            y = read16(sock)
            draw_point(x, y)
            display.flush()

        elif cmd == 0x03:  # DRAW LINE
            x1 = read16(sock)
            y1 = read16(sock)
            x2 = read16(sock)
            y2 = read16(sock)
            draw_line(x1, y1, x2, y2)
            display.flush()

        else:
            print("Unknown command:", cmd)

# ------------------------------------------------------------
# TCP server
# ------------------------------------------------------------
def start_server(port):
    s = usocket.socket()
    s.setsockopt(usocket.SOL_SOCKET, usocket.SO_REUSEADDR, 1)
    s.bind(("0.0.0.0", port))
    s.listen(1)

    print("VT01 server listening on", port)

    while True:
        conn, addr = s.accept()
        print("Client connected:", addr)
        vt01_display([("VT01A emulator", 0xFF0000),(sta_if.ifconfig()[0] + ":4444", 0xFFFFFF),("Client connected", 0x00FF00),(addr[0], 0x00FF00)])
        try:
            command_handler(conn)
        except Exception as e:
            print("Connection closed:", e)
        finally:
            conn.close()
            print("Client disconnected:", addr)
            vt01_display([("VT01A emulator", 0xFF0000),(sta_if.ifconfig()[0] + ":4444", 0xFFFFFF),("Client disconnected", 0x00FF00),(addr[0], 0x00FF00)])

def vt01_display(lines):
	display.drawFill(0x000000)
	y = 100
	for line in lines:
		display.drawText(5, y, line[0], line[1], "dejavusans20")
		y += 30
	display.flush()

def on_action_btn_home(pressed):
    mch22.exit_python()

sta_if = network.WLAN(network.WLAN.IF_STA)
sta_if.active(True)
while not sta_if.active():
    time.sleep(0.2)

index = 0
last_ssid = sta_if.config("ssid")
while (ssid := nvs.nvs_getstr("wifi", f"s{index:02d}.ssid")) and ssid != last_ssid:
    index = index + 1
password = nvs.nvs_getstr("wifi", f"s{index:02d}.password")

vt01_display([
	(f"Connecting to {ssid}", 0x00ff00),
])

sta_if.connect(ssid, password)

while not sta_if.isconnected():
    time.sleep(0.2)
    
vt01_display([("VT01A emulator", 0xFF0000),(sta_if.ifconfig()[0] + ":4444", 0xFFFFFF)])
buttons.attach(buttons.BTN_HOME, on_action_btn_home)
start_server(PORT)
