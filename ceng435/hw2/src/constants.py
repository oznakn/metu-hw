# The package_size, representing MSS value.
PACKAGE_SIZE = 1000 # MSS

# The sequential number, an unsigned integer, size inside the package
SEQ_NUMBER_SIZE = 4

# The timestamp, a double, size inside the package
TIMESTAMP_SIZE = 8

# The data_length, an unsigned integer, size inside the package
DATA_LENGTH_SIZE = 4

# The checksum, a 16 characters/bytes string, size inside the package
CHECKSUM_SIZE = 16


# The calculated header size of the send packages is used by RDTOverUDP based file transmission protocol
RDT_OVER_UDP_SEND_HEADER_SIZE = SEQ_NUMBER_SIZE + TIMESTAMP_SIZE + CHECKSUM_SIZE + DATA_LENGTH_SIZE

# The calculated value of the maximum chunk size to fit PACKAGE_SIZE
RDT_OVER_UDP_FILE_READ_SIZE = PACKAGE_SIZE - RDT_OVER_UDP_SEND_HEADER_SIZE

# The size of the ACK packages is used by RDTOverUDP based file transmission protocol
RDT_OVER_UDP_ACK_PACKAGE_SIZE = TIMESTAMP_SIZE + SEQ_NUMBER_SIZE


# The calculated header size of the packages is used by TCP based file transmission protocol
TCP_HEADER_SIZE = TIMESTAMP_SIZE + DATA_LENGTH_SIZE

# The calculated value of the maximum chunk size to fit PACKAGE_SIZE
TCP_FILE_READ_SIZE = PACKAGE_SIZE - TCP_HEADER_SIZE


# Wait timeout until a package will be retransmitted
RETRANSMISSION_TIMEOUT = 1
