#!/usr/bin/env python3

"""
This file is a Python script that starts the RDTOverUDP file transmission to server first,
and TCP file transmission to server second.
"""

import sys
import time

from src import *

# The command-line variables

TARGET_HOST = sys.argv[1]
UDP_TARGET_PORT = int(sys.argv[2])
TCP_TARGET_PORT = int(sys.argv[3])
UDP_SENDER_PORT = int(sys.argv[4])
TCP_SENDER_PORT = int(sys.argv[5])


def chunk_rdt_over_udp_file(filename):
    """The generator function to split chunks of the file given with its filename"""

    file = open(filename, 'rb')

    while True:
        # RDT_OVER_UDP_FILE_READ_SIZE is the maximum chunk size to read
        # in order to fit the PACKAGE_SIZE defined in constant.py
        chunk = file.read(RDT_OVER_UDP_FILE_READ_SIZE)

        # chunk is None, end of the file
        if not chunk:
            break

        # Yield the chunk
        yield chunk

    # Yield and empty chunk to state that the end of file is received.
    # This line is required for the last package of RDTOverUDP protocol.
    yield b''

    file.close() # Close the file


def chunk_tcp_file(filename):
    file = open(filename, 'rb')

    while True:
        # TCP_FILE_READ_SIZE is the maximum chunk size to read
        # in order to fit the PACKAGE_SIZE defined in constant.py
        chunk = file.read(TCP_FILE_READ_SIZE)

        # chunk is None, end of the file
        if not chunk:
            break

        # Yield the chunk
        yield chunk

    file.close() # Close the file


def send_rdt_over_udp():
    """Sends the file data using RDTOverUDP protocol"""

    data = chunk_rdt_over_udp_file('transfer_file_UDP.txt')

    client = RDTOverUDPClient(UDP_SENDER_PORT, TARGET_HOST, UDP_TARGET_PORT, data)

    c = client.process() # Process and send the file and receive retransmission packet count

    print ('UDP Transmission Re-transferred Packets:', c)


def send_tcp():
    """Sends the file data using TCP protocol"""

    data = chunk_tcp_file('transfer_file_TCP.txt')

    client = TCPClient(TCP_SENDER_PORT, TARGET_HOST, TCP_TARGET_PORT, data)

    client.process()


if __name__ == '__main__':
    time.sleep(1) # Wait one second for RDTOverUDPServer start
    send_rdt_over_udp()

    time.sleep(1) # Wait one second for TCPServer start
    send_tcp()
