import socket
import time

IP = "127.0.0.1"
PORT = 42069

SIGN_UP = '1'
LOGIN = '5'

"""
this function gets the data from the socket by the bytes num
"""
def get_data_from_socket(sock, byte_num):
    return str(sock.recv(byte_num).decode())

"""
this function returns the full data from the server
"""
def get_string_data(sock):
    code = str(sock.recv(1).decode())
    data = get_data_from_socket(sock, int(sock.recv(4).decode()))
    return f"CODE:{code}, DATA:{data}\n"

"""
this funtion sends request to teh server by the json data given
"""
def send_request(sock, request_code, json_data):
    msg = request_code
    msg += str(len(json_data)).rjust(4, '0')
    msg += json_data
    sock.send(msg.encode())


def required_tests(sock):
    json_data = '{"username": "noUserNameLikeThat", "password": "1234"}'
    send_request(sock, LOGIN, json_data)
    time.sleep(1)
    print(get_string_data(sock))
    # user must be signed up to login
    json_data = '{"username": "user2", "password": "1234@#fds", "email":"user1@gmail.com", "phone": "052-3333876", "birthDate": "04.08.2005", "apt": "45", "city": "Jerusalem", "street": "gdudHahamishi"}'
    send_request(sock, SIGN_UP, json_data)
    time.sleep(1)
    print(get_string_data(sock))
    # first sign up for user1
    json_data = '{"username": "user1", "password": "1234", "email":"user1@gmail.com", "phone": "0523333876", "birthDate": "04.08.2005", "apt": "45", "city": "Jerusalem", "street": "gdudHahamishi"}'
    send_request(sock, SIGN_UP, json_data)
    time.sleep(1)
    print(get_string_data(sock))
    # cant be signed up twice with the same user name
    json_data = '{"username": "user2", "password": "1234@#fds"}'
    send_request(sock, LOGIN, json_data)
    time.sleep(1)
    print(get_string_data(sock))
    # user1 now logged in
    json_data = '{"username": "user1", "password": "1234"}'
    send_request(sock, LOGIN, json_data)
    time.sleep(1)
    print(get_string_data(sock))
    # cant be logged in twice
    # dont know what valid username is

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((IP, PORT))
    
    #required_tests(sock)

    """
    while True:
        choice = int(input("1 for get data\n2 for login request\n3 for signup request\n4 to exit\n"))
        if choice == 4:
            break
        elif choice == 1:
            print(get_string_data(sock))
        elif choice == 2:
            json_data = '{"username": "user1", "password": "1234"}'
            send_request(sock, LOGIN, json_data)
            print(f"Sent the following data: {json_data}\n")
        elif choice == 3:
            json_data = '{"username": "user1", "password": "1234", "email":"user1@gmail.com"}'
            send_request(sock, SIGN_UP, json_data)
            print(f"Sent the followong data: {json_data}\n")
        else:
            print("choose valid option\n")
    """

    json_data = '{"username": "user3", "password": "1234Hj%$#", "email":"user1@gmail.com", "phone": "02-3333876", "birthDate": "04.08.2005", "apt": "45", "city": "Jerusalem", "street": "gdudHahamishi"}'
    send_request(sock, SIGN_UP, json_data)
    time.sleep(1)
    print(get_string_data(sock))

    sock.close()


if __name__ == "__main__":
    main()