#!/usr/bin/env python3

import socket
import sys
from time import sleep

if (len(sys.argv) != 3):
    print("USAGE: python3 spider_client.py 127.0.0.1 1337")
    exit(1)
# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = (sys.argv[1], int(sys.argv[2]))
print('connecting to {} port {}'.format(*server_address))

while 1:
    try:
        sock.connect(server_address)
        print("connected !")
        break;
    except:
        print("Connect timed out, retrying in 5 seconds...")
        sleep(5)
        continue

try:
    # Send data
    message = b'This is the message.  It will be repeated.'
    print('sending {!r}'.format(message))
    sock.sendall(message)

    # Look for the response
    amount_received = 0
    amount_expected = len(message)

    while amount_received < amount_expected:
        data = sock.recv(16)
        amount_received += len(data)
        print('received {!r}'.format(data))

finally:
    print('closing socket')
    sock.close()
