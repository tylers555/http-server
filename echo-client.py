import socket
import time

HOST = "Tyler-Laptop"  # The server's hostname or IP address
PORT = 65432  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b"This is being sent from the Python script instead!\0")
    data = s.recv(1024)

print("Received: \"" + data[0:-1].decode("utf8") + "\"")