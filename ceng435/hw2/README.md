# CENG435 HW2

TCP and RDTOverUDP (Selective Repeat) based reliable file transmission protocol implementations for METU's CENG 435 Data Communications and Networking class.

## Author

Ozan Akın (2309599)

## Make commands

* `server`: Runs the server program using server.py
* `client`: Runs the client program using client.py
* `sim`: Runs the simulator
* `client_sim`: Runs the client in simulator mode using client.py

## Getting Started

The system has two different modes, normal and simulator mode. In normal mode server runs in inek1 and client runs in any other inek machine. In simulator mode server runs in inek1, and both client and simulator runs in any other inek machine. Both simulator and client must be running in the same machine in order to system work.

### Example run

* Run `make server` in inek1
* Run `make sim` and `make client_sim` in an inek machine other than inek1.

### Port Usage

The ports assigned to me are in between 16000 and 16007.

In normal mode the port usage as follows:

* `16000` is used for UDP client as sender port
* `16001` is used for TCP client as sender port
* `16002` is used for UDP server as listen port
* `16003` is used for TCP server as listen port

In simulator mode the port usage as follows:

* `16000` is used for UDP client as sender port
* `16001` is used for TCP client as sender port
* `16002` is used for UDP server as listen port
* `16003` is used for TCP server as listen port
* `16004` is used for simulator's UDP server as listen port
* `16005` is used for simulator's TCP server as listen port
* `16006` is used for simulator's UDP client as sender port
* `16007` is used for simulator's TCP client as sender port

## Study

In this assignment, I implemented two different approach for file transmission. One approach that uses TCP and another approach that used UDP. Since UDP is an unreliable protocol I also implemented a RDT (Reliable Data Transfer) protocol in order to transfer files right.

First I tried to implement the TCP based file transmission, since it looked easier to me. Then I decided that I can implement RDTOverUDP after I finish TCP. Since TCP is a reliable protocol, transmitting file contents using one TCP socket is enough. After transferring file data from client, client can close the socket. After client closes the socket, server automatically shuts down.

Then, I started to look for RDT implementations. After my research, I decided to implement Selective Repeat based RDT, however implementing directly selective repeat looked hard to me. So I decided to first hold an experiment with RDT3.0.

After my experiments with RDT3.0, which I implemented using some finite state machine logic, I realized there are some cases that RDT3.0 cannot handle. If an ACK package is lost, during a retransmission a package can take place of another package. In order to resolve this issue, a sequential number must be added to the implementation. This was the biggest problem I faced with.

Then I implemented the Selection Repeat Protocol for RDT. I mostly obey the regulations of the protocol. However I made all packages sent from server have equal size in order to make things easier. I used the name RDTOverUDP for my implementation.

I also did some testing on the timeout value for resending packages. Although 1 second is recommended for waiting before resending a package, after my testing 1 second is too much for our scope of the homework. In most cases the TCP or UDP transmission time not exceeds 5ms. However considering the possibility that using this code in real life, I leave the timeout as it is, 1 second.

The homework took about one week for me. After the homework, I learnt that reliability in the scope of network is one of the most important things to deal with. I think it is just amazing that current file transmission works mostly without no problems.

## Protocols

For the TCP part, I implemented a application layer protocol with one way only. The packages are only transmitted from client to server. Each packages contains three thing, the timestamp of the package have sent, the content length of the actual data that package carries, and the actual data. Also there is an alignment to make sure every package created is actually 1000 bytes (the MSS value). To accomplish this, the left space of the package are filled with space (' ') character. Further information can be found `tcp.py` file inside the `src` directory.

```8 byte timestamp (double) | 4 byte chunk_length (unsigned integer) | chunk (bytes) | alignment```

___

For the UDP part, the data both transmitted from server and client. There are two different packages for the transmission, one for sending data, other one is ACK for data.

The send data consists the fields below:

* sequential_number, 4-byte unsigned integer, is used for which chunk is sent from client.
* timestamp, 8-byte double, is the timestamp which the package created
* checksum, a 16 characters string, is the md5 checksum of the chunk
* chunk_length, 4-byte unsigned integer, is the size of the chunk (not the whole package)
* chunk, variable-sized string, the actual data the package contains
* alignment, variable-sized string, the alignment of the package that ensures every package
  is packed from or unpacked by these functions are exactly 1000 bytes, as stated as PACKAGE_SIZE
  in constants.py. Alignment size is zero in most of the cases.

Also there is an alignment to make sure every package created is actually 1000 bytes (the MSS value). To accomplish this, the left space of the package (alignment) are filled with space (' ') character.

```seq (4 byte) | timestamp (8 byte) | checksum (16 byte) | chunk_length (4 byte) | chunk | alignment```

The ACK package contains the current timestamp and sequential_number. However its size is not exactly MSS value like send packages.


For the checksum I used md5 checksum of the chunks. Also note that the chunk_length is the length of only the chunk part, not the whole package. Further information can be found `rdt_over_udp.py` file inside the `src` directory.

## Analyzing Results

If we look closely at printed values we can see that the average transmission time in RDTOverUDP is less than TCP. This is because in RDTOverUDP's timer does not include ACK packages, however in TCP it does. This is one the reasons that although the average transmission time is lower in UDP, total transmission time is lower in TCP, with also the packet loses in simulator.

I discovered that keeping the window size larger like 100 speeds up the UDP transmission. I started my project with windows size as 5, however I increased to 100 to make the transmission faster.
