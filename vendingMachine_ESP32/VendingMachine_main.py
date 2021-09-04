import esp, network, time, gc, socket
from machine import Pin,PWM

server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server.bind(('', 80))  #('HOST','PORT')
server.listen(4)

while True:
    gc.collect()
    print("web server is listening")
    conn, addr = server.accept()  #conn is a new socket object  to send and receive data
    print("URL:", addr)
    web_request = conn.recv(1024)  #recieved data is data type:bytes
    web_request = str(web_request)  #convert bytes to string form
    print('https REQ-Content ={}'.format(web_request))
    req_optA = 'GET /?btn_a'
    req_optB = 'GET /?btn_b'
    req_optC = 'GET /?btn_c'
    req_optD = 'GET /?btn_d'
    
    
    if (web_request.find(req_optA) != -1):
        led_powerOn.on() #To show the program is working
        for move in range(minAngle, maxAngle):
            servo_A.duty(move) and led.value(1)
            time.sleep(3)
            servo_A.duty(stopPosition)
            break
        time.sleep(0.2) #Debounce for button
    elif (web_request.find(req_optB) != -1):
        led_powerOn.on() #To show the program is working
        for move in range(minAngle, maxAngle):
            servo_B.duty(move) and led.value(1)
            time.sleep(3)
            servo_B.duty(stopPosition)
            break
        time.sleep(0.2) #Debounce for button
    elif (web_request.find(req_optC) != -1):
        led_powerOn.on() #To show the program is working
        for move in range(minAngle, maxAngle):
            servo_C.duty(move) and led.value(1)
            time.sleep(3)
            servo_C.duty(stopPosition)
            break
        time.sleep(0.2) #Debounce for button
    elif (web_request.find(req_optD) != -1):
        led_powerOn.on() #To show the program is working
        for move in range(minAngle, maxAngle):
            servo_D.duty(move) and led.value(1)
            time.sleep(3)
            servo_D.duty(stopPosition)
            break
        time.sleep(0.2) #Debounce for button
    
    
    server_response = http_web()
    conn.sendall('HTTP/1.1 200 OK\n')
    conn.sendall('Conttttent-Type:text/html')
    conn.sendall('Connection:close\n')
    conn.sendall(server_response)
    conn.close()    