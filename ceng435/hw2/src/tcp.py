"""
This file includes the components required to transfer a file using TCP.
"""

import socket
import struct

from . import utils, constants

def create_socket(host, port):
    """Creates a TCP socket for given host and port"""

    conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    conn.bind((host, port))

    return conn

"""
The protocol uses struct for creating binary packed data.

The package format of the packages as follows:

* timestamp, 8-byte double, is the timestamp which the package created
* chunk_length, 4-byte unsigned integer, is the size of the chunk (not the whole package)
* chunk, variable-sized string, the actual data the package contains
* alignment, variable-sized string, the alignment of the package that ensures every package
  is packed from or unpacked by these functions are exactly 1000 bytes, as stated as PACKAGE_SIZE
  in constants.py. Alignment size is zero in most of the cases.


Also these functions consits something called struct format, which is a string used for
struct library to determine the format of the binary data. The characters used in format
string can be found below:

* ! character in format string stands for network byte order, which is Big Endian.
* I stands for 4-byte unsigned integers.
* d stands for 8-byte doubles.
* s stands for a string, a number prefix states its length.
"""

def pack_package(chunk):
    """Packs the chunk with adding additional data"""

    timestamp = utils.get_timestamp()

    chunk_length = len(chunk)

    # TCP_FILE_READ_SIZE is the maximum chunk size in order to fit the PACKAGE_SIZE
    # given in the constants.py
    aligner_size = constants.TCP_FILE_READ_SIZE - chunk_length

    return struct.pack(f'!dI{chunk_length}s{aligner_size}s', \
                        timestamp, chunk_length, chunk, b' ' * aligner_size)

def unpack_package(data):
    """Unpacks the data

    To determine the chunk size first the header of the package is unpacked which includes
    timestamp and chunk_length.
    """

    try:
        # TCP_HEADER_SIZE is the header size of the package, which includes timestamp and
        # chunk_length
        timestamp, chunk_length = struct.unpack('!dI', data[:constants.TCP_HEADER_SIZE])

        aligner_size = constants.TCP_FILE_READ_SIZE - chunk_length

        chunk, _ = struct.unpack(f'!{chunk_length}s{aligner_size}s', data[constants.TCP_HEADER_SIZE:])

        return timestamp, chunk
    except KeyboardInterrupt as e: # These line is required to stop the code in case of a KeyboardInterrupt.
        raise e
    except:
        return None


class TCPServer:
    """This class defines the server-side of the TCP based file transmission"""

    def __init__(self, host, port):
        """Constructor of the class TCPServer

        host and port are the values that server is going to use.
        """

        self.host = host
        self.port = port

    def process(self):
        """Runs the server and recives the file chunks"""

        self.socket = create_socket(self.host, self.port) # Create a TCP socket
        self.socket.listen() # Listen the host and port

        conn, addr = self.socket.accept() # Accept a TCP connection

        with conn: # Keep the TCP connection open in this block.
            while True:
                buffer = b'' # Create an empty buffer, buffer size can be at most constants.PACKAGE_SIZE

                # Since TCP is actually a streaming protocol, the received packages can be only
                # some part of a package. Thus, keeping track of a buffer is required
                while len(buffer) < constants.PACKAGE_SIZE:
                    # Receive data whose length is equal to remaining buffer space
                    buffer_chunk = conn.recv(constants.PACKAGE_SIZE - len(buffer))

                    if not buffer_chunk: # Received an empty chunk, end of the transmission
                        break

                    buffer += buffer_chunk

                if not buffer: # Received an empty buffer, end of the transmission
                    break

                timestamp_received = utils.get_timestamp()

                timestamp_sent, chunk = unpack_package(buffer) # Unpack the package using given buffer

                # Yield the time_sent, time_received and file chunk.
                yield timestamp_sent, timestamp_received, chunk


        # End of the file tranmssion, socket can be closed.
        self.socket.close()


class TCPClient:
    """This class defines the client-side of the TCP based file transmission"""

    def __init__(self, sender_port, target_host, target_port, data):
        """Constructor of the class TCPServer

        sender_port is the port that TCP socket is going to use.
        target_host and target_port are the values that server is going to use.
        data is a generator that creates chunks of file data.
        """

        self.sender_port = sender_port

        self.target = (target_host, target_port)

        self.data = data

    def process(self):
        """Sends chunks of file to TCP based file transmission server"""

        # Create a TCP socket and connect to the server
        self.socket = create_socket('', self.sender_port)
        self.socket.connect(self.target)

        # For every chunk of data, pack and send the chunk
        for chunk in self.data:
            packed = pack_package(chunk)

            self.socket.sendall(packed)

        # All chunks have been sent, socket can be closed.
        self.socket.close()
