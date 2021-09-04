import socket, esp, gc, network
from machine import Pin

esp.osdebug(0)
gc.collect()

ssid='CHOU'
password='24323890'

WIFI=network.WLAN(network.STA_IF)
WIFI.active(True)
WIFI.connect(ssid,password)

while WIFI.isconnected()==False:
    pass

print('Connection successful')
print(WIFI.ifconfig())
    
led = Pin(27, Pin.OUT)
led2 = Pin(26, Pin.OUT)
led3 = Pin(25, Pin.OUT)