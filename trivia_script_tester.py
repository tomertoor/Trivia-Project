import socket
import json

IP = "127.0.0.1"
PORT = 42069

SIGN_UP = '1'
LOGIN = '5'

def get_data_from_socket(sock, byte_num):
    return sock.recv(byte_num).decode()

def get_string_data(sock):
    code = str(sock.recv(1).decode())
    data = get_data_from_socket(sock, sock.recv(4).decode())
    return f"CODE:{code}, DATA:{data}"

def send_request(sock, json_data, request_code):
    msg += request_code
    msg += str(len(json_data))
    msg += json_data
    sock.send(msg.encode())

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((IP, PORT))

    while True:
        choice = int(input("1 for get data\n2 for login request\n3 for signup request\n4 to exit"))
        if choice == 4:
            break
        elif choice == 1:
            print(get_string_data(sock))
        elif choice == 2:
            json_data = '{"username": "user1", "password": "1234"}'
            send_request(sock, LOGIN, json_data)
            print(f"Sent the followong data: {json_data}")
        elif choice == 3:
            json_data = '{"username": "user1", "password": "1234", "mail":"user1@gmail.com"'
            send_request(sock, SIGN_UP, json_data)
            print(f"Sent the followong data: {json_data}")
        else:
            print("choose valid option\n")

    sock.close()


if __name__ == "__main__":
    main()