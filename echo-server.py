# echo-server.py

import socket

HOST = "tis0"  # Standard loopback interface address (localhost)
PORT = 65432  # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    print("Started!")
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            data = conn.recv(1024)
            if bytes('\0', encoding="UTF-8") in data:
                print("Received: \"" + data[0:-1].decode("utf8") + "\"")
                conn.sendall(data)
                break
        
    