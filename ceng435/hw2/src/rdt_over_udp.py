"""
This file includes the components required to transfer a file using UDP.

Since UDP is an unreliable protocol, a reliable data transfer protocol
was implemented on top of it.

Selective Repeat Protocol was chosen for reliable data transfer.
"""

import socket
import struct
import time
from collections import deque

from . import utils, constants

"""
100 is the window size for both server and client
"""
WINDOW_SIZE = 100


"""The variables below defines states for every package_data sent from client and received by server.

A PackageData is a class that contains the needed info for sending and receiving packages

States STATE_WAITING and STATE_RECEIVED is used for client-side of the protocol. and states
STATE_WAITING, STATE_SENT, and STATE_ACKED is used for server-side of the protocol.

* STATE_WAITING is the default state
* STATE_SENT means that the package_data was sent from client
* STATE_RECEIVED means that the package_data was received by server
* STATE_ACKED means that the already sent package_data was ACK-ed by the server that was received.
"""
STATE_WAITING = 0
STATE_SENT = 1
STATE_RECEIVED = 2
STATE_ACKED = 3

"""These five functions are utility functions that only used by this file."""

def create_socket(host, port):
    """
    Creates an UDP socket with given host and port
    """

    conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    conn.bind((host, port))

    return conn

"""
The four functions below are used for packing and unpacking binary packages.

The package format of the send packages, which is used in the first two packages, as follows:

* sequential_number, 4-byte unsigned integer, is used for which chunk is sent from client.
* timestamp, 8-byte double, is the timestamp which the package created
* checksum, a 16 characters string, is the md5 checksum of the data
* chunk_length, 4-byte unsigned integer, is the size of the chunk (not the whole package)
* chunk, variable-sized string, the actual data the package contains
* alignment, variable-sized string, the alignment of the package that ensures every package
  is packed from or unpacked by these functions are exactly 1000 bytes, as stated as PACKAGE_SIZE
  in constants.py. Alignment size is zero in most of the cases.

Moroever a package whose chunk_length is 0 is always sent after the process. This packages states
that the transmission must stop after all package_data's in the current window are received.


Also these functions consits something called struct format, which is a string used for
struct library to determine the format of the binary data. The characters used in format
string can be found below:

* ! character in format string stands for network byte order, which is Big Endian.
* I stands for 4-byte unsigned integers.
* d stands for 8-byte doubles.
* s stands for a string, a number prefix states its length.
"""

def pack_send_package(seq, chunk):
    """Packs the given chunk in binary format for sending.

    sequential_number, timestamp, chunk_length, md5 checksum, and alignment are added to result data.
    """

    timestamp = utils.get_timestamp()

    chunk_length = len(chunk)
    checksum = utils.get_checksum(bytes(str(seq), 'utf8') + bytes(str(timestamp), 'utf8') + \
            bytes(str(chunk_length), 'utf8') + chunk)


    # RDT_OVER_UDP_FILE_READ_SIZE is maximum chunk size can be fit in a package.
    # The maximum chunk size is bounded by PACKAGE_SIZE in constants.py
    align_size = constants.RDT_OVER_UDP_FILE_READ_SIZE - chunk_length

    # The given string given is called struct format.
    # * ! character in format string stands for network byte order, which is Big Endian.
    # * I stands for 4-byte unsigned integers.
    # * d stands for 8-byte doubles.
    # * 16s stands for a string with length of 16.
    # * The rest includes two variable-size strings, for chunk and alignment.
    return struct.pack(f'!Id16sI{chunk_length}s{align_size}s', seq, timestamp, \
                            checksum, chunk_length, chunk, b' ' * align_size)

def unpack_send_package(data):
    """Unpacks received package.

    To determine the chunk size first the header of the package is unpacked, which includes
    sequential_number, timestamp, checksum, and chunk_length.

    Then the alignment_size is calculated based on the chunk_length, after that the chunk
    itself is unpacked.

    The whole code is wrapped with try-catch in case of unpack error, which can be occur
    in corrupted package.

    This functions returns a tuple consists of sequential_number, timestamp and chunk.
    If the chunk cannot be acked using the checksum, the chunk in the return data will be none
    If an unpack error occurs, then code returns only None
    """

    try:
        # RDT_OVER_UDP_SEND_HEADER_SIZE is the size of the package header, which contains
        # sequential_number, timestamp, check_checksum, and chunk_length.

        seq, timestamp, checksum, chunk_length = \
                        struct.unpack('!Id16sI', data[:constants.RDT_OVER_UDP_SEND_HEADER_SIZE])

        align_size = constants.RDT_OVER_UDP_FILE_READ_SIZE - chunk_length

        chunk, _ = struct.unpack(f'!{chunk_length}s{align_size}s', data[constants.RDT_OVER_UDP_SEND_HEADER_SIZE:])

        if not utils.check_checksum(checksum, bytes(str(seq), 'utf8') + bytes(str(timestamp), 'utf8') + \
                bytes(str(chunk_length), 'utf8') + chunk):
            return seq, timestamp, None

        return seq, timestamp, chunk
    except KeyboardInterrupt as e: # These line is required to stop the code in case of a KeyboardInterrupt.
        raise e
    except:
        return None


def pack_ack_package(seq):
    """Packs the ACK package sent from server

    The package contains the current timestamp and sequential_number
    """

    timestamp = utils.get_timestamp()

    return struct.pack('!Id', seq, timestamp)

def unpack_ack_package(data):
    """Unpacks the ACK package sent from server

    The package contains the current timestamp and sequential_number.

    This functions returns None if an unpack error occurs, or a tuple containing
    sequential_number and timestamp in success.
    """

    try:
        return struct.unpack('!Id', data)
    except KeyboardInterrupt as e: # These line is required to stop the code in case of a KeyboardInterrupt.
        raise e
    except:
        return None

class PackageData:
    """The PackageData class contains the data required to build a package
    by the utility functions defined above. This class is used in send and
    receive windows.

    * seq is the sequential_number
    * chunk is the actual data
    * state is the current PackageData state
    * timestamp_sent is the time that the package_data is sent
    * timestamp_received is the time that package_data is received.
    """

    seq = None
    chunk = None
    state = None
    timestamp_sent = None
    timestamp_received = None

    def __init__(self, seq, chunk=None):
        """The constructor for PackageData

        sequential_number is reset every 10000 chunks sent

        chunk can be null in server-side, that means the chunk info
        that belong to sequential number `seq` has not received yet.

        STATE_WAITING is the default state
        """

        self.seq = seq % 10000
        self.chunk = chunk

        self.state = STATE_WAITING

    def mark_as_sent(self):
        """Marks this package as sent, only used in client-side"""
        self.timestamp_sent = utils.get_timestamp()

        self.state = STATE_SENT

    def mark_as_acked(self):
        """Marks this package as ACK-ed, only used in client-side"""

        self.state = STATE_ACKED

    def mark_as_received(self, timestamp_sent, chunk):
        """Marks this package as received, only used in server-side"""

        self.chunk = chunk
        self.timestamp_sent = timestamp_sent # This data comes from the client-side

        self.timestamp_received = utils.get_timestamp()

        self.state = STATE_RECEIVED

    def __str__(self):
        return f'{self.seq} - {self.state}'

    def __repr__(self):
        return self.__str__()

class RDTOverUDPServer:
    """This class defines the server-side of the RDTOverUDP protocol"""

    def __init__(self, listen_host, listen_port):
        """The constructor for class RDTOverUDPServer

        listen_host and listen_port are host and port that is listened by server
        """

        self.listen_host = listen_host
        self.listen_port = listen_port

        # The receive window for keeping the track of packages.
        # Python's builtin deque class is used for performance improvements.
        self.window = deque()


        # The variable stating that the last package has received by server.
        #
        # This variable does not state that all packages have been received,
        # it only states the last package is received.
        self.has_finished = False

        for i in range(WINDOW_SIZE): # Creates the first window
            package_data = PackageData(i)

            self.window.append(package_data)

    def send_ack(self, address, seq):
        """Packs an ACK package_data with the sequential number and sends to given address"""

        packed = pack_ack_package(seq)

        self.socket.sendto(packed, address)

    def mark_package_data_as_received_by_seq(self, seq, timestamp, chunk):
        """Marks the package_data stated by the given sequential_number `seq`
           as received. Also sets the timestamp_sent and chunk attributes of
           the package_data
        """

        for package_data in self.window:
            if package_data.seq == seq and package_data.state == STATE_WAITING:
                package_data.mark_as_received(timestamp, chunk)

                break

    def process(self):
        """Process function

        The process function is the place that the actual magic happens
        """

        # Create a socket with listen_host and listen_port
        self.socket = create_socket(self.listen_host, self.listen_port)

        # Set timeout in case of client process failure
        self.socket.settimeout(20.0)

        # Continue execution until the length of the current window is zero.
        # Which means all package_data's in the current window are received and
        # there will be no packages (i.e. has_finished is true).
        while len(self.window) > 0:
            # Receive a packed-data with PACKAGE_SIZE bytes from current socket.
            try:
                packed, address = self.socket.recvfrom(constants.PACKAGE_SIZE)
            except socket.timeout:
                break

            # Check that packed data is not None and exactly PACKAGE_SIZE bytes.
            # With the alignment, it is guaranteed that every package has exactly
            # PACKAGE_SIZE bytes.
            if packed is not None and len(packed) == constants.PACKAGE_SIZE:
                result = unpack_send_package(packed) # Unpack the package

                # Check that the result is not None, which means
                # the packet can be unpacked  without any errors.
                if result is not None:
                    seq, timestamp, chunk = result

                    # Check that the chunk is not None and have a zero-length.
                    # This means that the package is a checksum-verified last
                    # package of the transmission.
                    if chunk is not None and len(chunk) == 0:
                        for i in range(5):
                            # If the last ACK package is lost, the client keeps
                            # sending the last package. In order to make sure that
                            # last is sent, send 5 of it.
                            self.send_ack(address, seq) # Send an ACK message

                        # Remove any remaining not-received packages since this is the
                        # last package.
                        while len(self.window) > 0 and self.window[-1].seq != seq:
                            self.window.pop()

                        self.window.pop() # Remove the last package itself.

                        self.has_finished = True

                    # Chunk is checksum-verified but not the last package
                    elif chunk:
                        self.send_ack(address, seq) # Send an ack message

                        # Mark the package_data as received
                        self.mark_package_data_as_received_by_seq(seq, timestamp, chunk)

                        # This code block removes the first received packages and
                        # yields them. Yielding means that these packages are now
                        # writable to any file since there no missing chunk between
                        # them.
                        while len(self.window) > 0:
                            if self.window[0].state != STATE_RECEIVED:
                                break

                            yield self.window[0]

                            new_sequential_number = self.window[-1].seq + 1

                            self.window.popleft() # Remove the received package

                            # Append a new package_data with a sequential_number one more than
                            # the sequential_number of last package data in the receive window.
                            # This functions appends a package_data for every package_data is
                            # removed (i.e. received) by server. The IF statement checks
                            # that the last package of the transmission received or not, i.e.
                            # new package_data's must be appended since there are more chunks
                            # to receive.
                            if not self.has_finished:
                                new_package_data = PackageData(new_sequential_number)

                                self.window.append(new_package_data)

        # Since the window size is zero, all packages must be received,
        # socket can be closed.
        self.socket.close()

class RDTOverUDPClient:
    """This class defines the client-side of the RDTOverUDP protocol"""

    def __init__(self, sender_port, target_host, target_port, data):
        """The constructor of the class RDTOverUDPClient

        * sender_port is the port that is used by UDP socket to connect from.
        * target_host and target_port are the host and port of the server
        * data is a Python generator for receiving chunks from the actual data
          must be transmitted.
        """

        self.sender_port = sender_port
        self.target = (target_host, target_port)

        self.data = data

        # Retransmission count is the counter that keeping the track of
        # how many packages are retransmitted.
        self.retransmission_count = 0

        self.window = deque() # The sender window
        self.fill_window() # The function that fills the sender window

    def next_seq(self):
        """The function for generating next sequential number

        If the window is empty, the first sequential number is 0"""

        if len(self.window) > 0: return self.window[-1].seq + 1

        return 0

    def fill_window(self):
        """Fills the sender window"""

        while len(self.window) < WINDOW_SIZE:
            try:
                chunk = next(self.data) # Fetch the next chunk from the generator

                package_data = PackageData(self.next_seq(), chunk)

                self.window.append(package_data)
            except StopIteration: # The generator raises an error in the end
                break

    def send_package_data(self, package_data):
        """Sends a packed package data using the given_package_data"""

        packed = pack_send_package(package_data.seq, package_data.chunk)

        self.socket.sendto(packed, self.target)

        # If the sent package data has already been sent, then
        # increase the retransmission count by one.
        if package_data.state == STATE_SENT:
            self.retransmission_count += 1

        # Mark the package_data as sent and update the `timestamp_sent` timestamp.
        package_data.mark_as_sent()

    def mark_package_data_acked_by_seq(self, seq):
        """Mark a package_data with given sequential_number"""

        for package_data in self.window:
            if package_data.seq == seq:
                package_data.mark_as_acked()

                break

    def process(self):
        """Process function

        The process function is the place that the actual magic happens
        """

        self.socket = create_socket('', self.sender_port) # Create a UDP socket
        # Set the timeout of the socket with RETRANSMISSION_TIMEOUT timeout
        self.socket.settimeout(constants.RETRANSMISSION_TIMEOUT)

        # If the window size not zero, i.e. there are still package_data's must be
        # sent, send every package that in waiting state (not sent before) and wait
        # for incoming packages.
        while len(self.window) > 0:
            for package_data in self.window:
                if package_data.state == STATE_WAITING:
                    self.send_package_data(package_data)

                    break

            self.wait()

        # Since the window size is zero, there are no package_data's must be send,
        # and all package_data's sent are ACK-ed by the server, the socket can be closed.
        self.socket.close()

        # Return the retransmission count
        return self.retransmission_count

    def must_wait(self):
        """This functions checks that there is a sent package or not

        If there are packages that sent (and not ACK-ed) that means code must wait
        or resend these packages in order to get ACK messages.
        """

        for package_data in self.window:
            if package_data.state == STATE_SENT:
                return True

        return False

    def wait(self):
        while self.must_wait(): # If the code must wait.
            try:
                # RDT_OVER_UDP_ACK_PACKAGE_SIZE is the size of the ACK package which is
                # defined in constants.py.
                packed, _ = self.socket.recvfrom(constants.RDT_OVER_UDP_ACK_PACKAGE_SIZE)

                # If the packed data is not none
                if packed is not None:
                    result = unpack_ack_package(packed)

                    # If the result unpacked data is not none, that means no unpacking
                    # error occurred.
                    if result is not None:
                        # Fetch the sequential number from the result tuple
                        # and send a ACK message
                        seq, _ = result

                        self.mark_package_data_acked_by_seq(seq)

            # The code waited enough for receiving an ACK message, either the ACK messages take
            # too long to come, or some SEND or ACK messages have been lost or corrupted.
            except socket.timeout:
                pass

            # Calculate threshold_timestamp, the first time that the package_data's must resend.
            threshold_timestamp = utils.get_timestamp() - constants.RETRANSMISSION_TIMEOUT

            # Check every package_data in the sender window, if their timestamp_sent are below
            # the threshold, resend them.
            for package_data in self.window:
                if package_data.state == STATE_SENT and package_data.timestamp_sent < threshold_timestamp:
                    self.send_package_data(package_data)

            # Remove the first packages from the window has already been ACK-ed,
            # and refill the sender window.
            while len(self.window) > 0:
                if self.window[0].state == STATE_ACKED:
                    self.window.popleft()
                else:
                    break

            self.fill_window()
