import socket
import json

IP = "127.0.0.1"
PORT = 42069

def get_data_from_socket(sock):
    # data_len = socket.recv(4).decode()
    # byte_json = socket.recv(data_len).decode()
    # my_json = byte_json.decode('utf8').replace("'", '"')
    # return my_json
    return sock.recv(1024).decode()

def get_code_from_socket(sock):
    return sock.recv(1).decode()

def send_data_to_server(sock, data):
    msg = "100"
    msg += ( str(len(json.dumps(data))) + json.dumps(data) )
    msg = data
    sock.send(msg.encode())


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((IP, PORT))

print(get_data_from_socket(sock))
send_data_to_server(sock, "Hello")

sock.close()