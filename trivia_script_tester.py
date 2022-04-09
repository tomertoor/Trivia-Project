import socket
import json

IP = "127.0.0.1"
PORT = 42069

def get_data_from_socket(sock):
    return sock.recv(1024).decode()

def get_code_from_socket(sock):
    return sock.recv(1).decode()

def send_data_to_server(sock, data):
    msg = "100"
    msg += ( str(len(json.dumps(data))) + json.dumps(data) )
    msg = data
    sock.send(msg.encode())


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((IP, PORT))

    print(get_data_from_socket(sock))
    send_data_to_server(sock, "Hello")

    sock.close()


if __name__ == "__main__":
    main()