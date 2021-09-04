def web_page():
    if led1.value()==1:
        gpio_state="on"
    else:
        gpio_state="off"
    if led2.value()==1:
        gpio_state="on"
    else:
        gpio_state="off"
    if led3.value()==1:
        gpio_state="on"
    else:
        gpio_state="off"
    
    html="""
        <html>
        <head>
            <title> LED controler </title>
    
        </head>
        <body>
            """ + gpio_state + """
        </body>
        </html>"""
    return html 

server=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server.bind(('',80))
server.listen(5)

while True:
    conn,addr = server.accept()
    request = conn.recv(1024)
    request = str(request)
    print('--------------------------------')
    led_on = request.find('/?led=on')
    led_off= request.find('/?led=off')
    led2_on = request.find('/?led2=on')
    led2_off= request.find('/?led2=off')
    led3_on = request.find('/?led3=on')
    led3_off= request.find('/?led3=off')
    if led_on != -1:
        print('LED1 ON')
        led.value(1)
    if led_off != -1:
        print('LED1 OFF')
        led.value(0)
    if led2_on != -1:
        print('LED2 ON')
        led2.value(1)
    if led2_off != -1:
        print('LED2 OFF')
        led2.value(0)
    if led3_on != -1:
        print('LED3 ON')
        led3.value(1)
    if led3_off != -1:
        print('LED3 OFF')
        led3.value(0)


response = web_page()
conn.send('HTTP/1.1 200 OK\n')
conn.send('Content-Type:text/html\n')
conn.send('Connection: close\n\n')
conn.sendall(response)
conn.close()