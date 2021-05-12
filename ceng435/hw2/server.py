#!/usr/bin/env python3

"""
This file is a Python script that starts the RDTOverUDP file transmission server first,
and TCP file transmission server second.
"""

import sys

from src import *

# The command-line variables

HOST = '0.0.0.0'
UDP_PORT = int(sys.argv[1])
TCP_PORT = int(sys.argv[2])


def calculate_avg_and_total_time(timestamps):
    """Calculates average time for each package and total transmission time for given timestamps"""

    # If there is no timestamp, just return 0,0
    if len(timestamps) == 0: return 0, 0

    # Calculate the total time passed for file transmission
    total_time = (timestamps[-1] - timestamps[0]) * 1000

    # Calculate the difference between time sent and time received of packages
    time_diffs = []
    for i in range(0, len(timestamps), 2):
        time_diffs.append(abs(timestamps[i + 1] - timestamps[i]) * 1000)

    # Calculate the average of the time differences
    avg_time = sum(time_diffs) / len(time_diffs)

    return avg_time, total_time


def receive_rdt_over_udp():
    """Starts a RDPOverUDP file transmission server to receive a file"""

    timestamps = []

    result_file = open('transfer_file_UDP.txt', 'wb') # Open the file

    server = RDTOverUDPServer(HOST, UDP_PORT) # Initialize server

    # Receive package_data for every chunk of file
    for package in server.process():
        # Save the sent and received time for packages
        timestamps.append(package.timestamp_sent)
        timestamps.append(package.timestamp_received)

        # Write chunk into the file
        result_file.write(package.chunk)

    result_file.close() # Close the file

    avg_time, total_time = calculate_avg_and_total_time(timestamps)

    # Print the calculated values
    print ('UDP Packets Average Transmission Time:', '%.6f' % avg_time , 'ms')
    print ('UDP Communication Total Transmission Time:', '%.6f' % total_time, 'ms')


def receive_tcp():
    """Starts a TCP file transmission server to receive a file"""

    timestamps = []

    result_file = open('transfer_file_TCP.txt', 'wb') # Open the file

    server = TCPServer(HOST, TCP_PORT) # Initialize server

    # Receive timestamp and chunk for every chunk of file
    for timestamp_sent, timestamp_received, chunk in server.process():
        # Save the sent and received time for packages
        timestamps.append(timestamp_sent)
        timestamps.append(timestamp_received)

        # Write chunk into the file
        result_file.write(chunk)

    result_file.close() # Close the file

    avg_time, total_time = calculate_avg_and_total_time(timestamps)

    # Print the calculated values
    print ('TCP Packets Average Transmission Time:', '%.6f' % avg_time , 'ms')
    print ('TCP Communication Total Transmission Time:', '%.6f' % total_time, 'ms')


if __name__ == '__main__':
    receive_rdt_over_udp()
    receive_tcp()
