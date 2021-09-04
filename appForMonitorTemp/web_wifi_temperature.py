import machine
import time

import esp
esp.osdebug(None)
#------------------------

import network

ssid = 'CHOU'
password = '24323890'
#------------------------

station = network.WLAN(network.STA_IF)
time.sleep(1)

station.active(True)
station.connect(ssid, password)
station.ifconfig()

while not station.active():
    pass
print('network config:', station.ifconfig())

#------------------------
import socket

s= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
#s.bind(('', 80))
s.listen(5)

#------------------------
import dht
d = dht.DHT11(machine.Pin(14))

def web_page():
    d.measure()
    t = d.temperature()
    h = d.humidity()
        
    html_page = """ <!DOCTYPE HTML>
                <html>
                <head>
                    <meta name="viewport" content="width=device-width, initial-scale=1">
                </head>
                <body>
                    <center><h2>ESP32 Web Server in MicroPython </h2></center> 
                    <center><p>Temperature is <strong>"""+ str(t) + """ C.</strong>.</p></center>
                    <center><p>Humidity is <strong>"""+ str(h) + """ %.</strong>.</p></center>
                </body>
                </html>"""
    return html_page

while True:
    conn, addr = s.accept()    
    request=conn.recv(1024)
    
    response = web_page()
    conn.send('HTTP/1.1 200 OK\n')
    conn.send('Content-Type: text/html\n')
    conn.send('Connection: close\n\n')
    conn.sendall(response)
    
    conn.close()