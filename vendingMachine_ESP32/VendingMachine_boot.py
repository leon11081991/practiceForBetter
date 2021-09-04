import esp, network, time, gc, socket
from machine import Pin,PWM
esp.osdebug(0)

SSID="CHOU"
password="24323890"
led = Pin(2, Pin.OUT)

gc.collect()
wifi=network.WLAN(network.STA_IF)
wifi.active(True)
try:
    while True:
        wifi.connect(SSID,password)
        print("Start to connect Wifi.")
        for sec in range(10):
            print("Waiting for connection in {} seconds.".format(sec))
            time.sleep(1)  #blue LED turn on if wifi connected
            if wifi.isconnected():
                led.value(True)
                break
        if wifi.isconnected():
            print("Wifi connected")
            print("Network Config=",wifi.ifconfig())
            break
        else:
            continue
except error:
    print(error)

#---------------------------------

led_powerOn=Pin(14, Pin.OUT)

servo_A=PWM(Pin(25), freq=50, duty=0)
servo_B=PWM(Pin(26 ), freq=50, duty=0)
servo_C=PWM(Pin(32), freq=50, duty=0)
servo_D=PWM(Pin(33), freq=50, duty=0)
minAngle=int(0.5/20*1023)
stopPosition=int(1.5/20*1023)
maxAngle=int(2.5/20*1023)
degree=[minAngle, maxAngle] 

#---------------------------------

def http_web():
    html = """
        <html>
            <head>
                <title>Welcome To Buy</title>
                <meta name="viewport" content="width=device-width, initial-scale=1">
                <link rel="icon" href="data:,">
                <style>
                    html{font-family: Helvetica;
                        display:inline-block;
                        margin: 0px auto;
                        text-align: center;}
                    h1{color: #0F3376; padding: 2vh;}
                    p{font-size: 1.5rem;}
                    .button{display: inline-block;
                            background-color: #e7bd3b;
                            border: none;
                            border-radius: 4px;
                            color: white;
                            padding: 16px 40px;
                            text-decoration: none;
                            font-size: 30px;
                            margin: 2px;
                            cursor: pointer;}
                </style>
            </head>
            <body>
                <h1>Please choose the product that you want.</h1>
                
                <p>
                    <input type="button" value="buttonA" onclick="location.href='/?btn_a'">
                </p>
                <p>
                    <input type="button" value="buttonB" onclick="location.href='/?btn_b'">
                </p>
                <p>
                    <input type="button" value="buttonC" onclick="location.href='/?btn_c'">
                </p>
                <p>
                    <input type="button" value="buttonD" onclick="location.href='/?btn_d'">
                </p>
            </body>
        </html>"""
    return html

#---------------------------------

