> Slovak University of Technology   
> Faculty of Informatics and Information Technologies   
> Ilkovičova 3, 842 16 Bratislava 4   

> **Study Program:** Computer Systems and Networks  
> **Course:** Computer Networks 1  
> **Academic Year:** 2008/09

# Message Transmission Using the UDP Protocol

## Assignment

On top of the UDP (User Datagram Protocol) of the transport layer of the TCP/IP network model, design and implement a protocol for message transmission between applications on computers (sending and receiving messages – *chat* functionality).

The length of a message is not limited. The receiving application must be capable of receiving a message at any time, i.e., the receiving side does not need to be switched into receive mode in advance.

If necessary, the message will be transmitted in blocks, where the maximum block size should be derived from the maximum length of the data field of an Ethernet frame.

The UDP protocol is unreliable; therefore, in the format of the designed protocol, also consider a checksum field and fields for possible communication reliability using a block ARQ method. Use the checksum employed by the UDP protocol (RFC 768).

Detection of an incorrect checksum on the receiving side will only be indicated.

The user interface must allow:

*   writing a message and selecting the destination computer on which the receiving application is running,
*   displaying the received message and the IP address of the computer from which the message was sent,
*   displaying checksums:
    *   the checksum sent on the sender side,
    *   the received and calculated checksum on the receiver side.

### Requirements for the Solution

The designed and implemented solution must meet the following requirements:

*   Message transmission between applications on different computers using a checksum
*   Division of the message into suitable blocks
*   Error indication in case of detection on the receiver side
*   Communication errors will be simulated

### Documentation Must Include

*   specification of the designed protocol format with a description of individual fields and protocol operation,
*   choice of transmitted block size,
*   choice of possible error protection and acknowledgment method,
*   description of the method for simulating error occurrence,
*   block diagram of the solution,
*   description of the user interface,
*   choice of implementation environment.

## Format of the UDP Packet Data Part

| Field    | Size       |
| -------- | ---------- |
| Type     | 1 B        |
| Length   | 2 B        |
| Flags    | 1 B        |
| ID       | 2 B        |
| Number   | 2 B        |
| Total    | 2 B        |
| Checksum | 2 B        |
| Data     | max 1460 B |

### Type (1 B) – Type of Transmitted Data

| Value | Meaning  |
| ----- | -------- |
| 0     | Nickname |
| 1     | Message  |
| 2     | ARQ data |

### Length (2 B)

Number of transmitted characters in the main data part.

### Flags (1 B)

| Bit Pattern | Meaning     | Description                                                                            |
| ----------- | ----------- | -------------------------------------------------------------------------------------- |
| `xxxxxxx1`  | fragment    | Indicates that the message has been split into multiple parts that must be reassembled |
| `xxxxxx1x`  | acknowledge | Receipt confirmation, normally not used, intended for ARQ                              |
| `xxxxx1xx`  | ack #       | Inverted with each sent packet to distinguish acknowledgments                          |

### ID (2 B)

Message identification number, incremented after each completely sent message.

### Number (2 B)

If the message does not fit into a single frame, this field specifies the fragment number so the message can be reassembled in the correct order.

### Total (2 B)

Total number of message segments. Used for memory allocation when reassembling the full message and to verify whether the entire message has been received.

### Checksum (2 B)

Checksum calculated from the custom header and transmitted data using the same method as the UDP checksum.

### Data (max 1460 B)

Transmitted data. This may include:

*   chat participant nickname,
*   message,
*   message fragment (if longer than 1460 B),
*   ARQ data.

## Problem Description, Analysis, and Design

From the assignment, it follows that we will use the UDP (User Datagram Protocol) encapsulated in the IP (Internet Protocol) within the TCP/IP network model.

### TCP/IP Layer Model

| Layer                      | Protocol             |
| -------------------------- | -------------------- |
| 4. Application Layer       | Message transmission |
| 3. Transport Layer         | UDP                  |
| 2. Network Layer           | IP                   |
| 1. Network Interface Layer | Ethernet II          |

UDP is an unreliable, connectionless protocol that does not guarantee delivery, acknowledgments, or packet ordering.

### Reliability

Transmission reliability will be ensured using a simple checksum of the data part. This is implemented by:

1.  Padding the data with a zero byte if necessary to ensure an even number of octets.
2.  Summing pairs of octets.
3.  If the sum exceeds 16 bits, splitting it into two 16-bit parts and adding them together.
4.  Applying a one’s complement.
5.  Writing the result into the packet header.

If a checksum mismatch is detected on the receiving side, the error will be indicated by displaying a message in the receiver’s main window.

The sender will not be informed of the error, as retransmission of erroneous packets is not required by the assignment.

### Error Simulation

Error simulation will be implemented by intentionally generating an incorrect checksum during message transmission (enabled via a checkbox).

### Acknowledgment (ARQ)

Acknowledgment will not be implemented, but the solution is designed to allow future extension and is described in the documentation.

The Stop-and-Wait ARQ method is considered:

*   The sender transmits one packet and waits for an acknowledgment.
*   If no acknowledgment is received within a specified time, the packet is retransmitted.
*   Acknowledgment packets use type `2` and the ACK flag.
*   The ACK# flag is inverted for each packet to prevent duplicate acknowledgments.

### Maximum Message Size

The maximum data payload that fits into a single packet is derived from the Ethernet frame size:

    1500 B (Ethernet payload)
    - 20 B (IP header)
    - 8 B  (UDP header)
    - 12 B (custom protocol header)
    = 1460 B

### Communication Model

Communication will be peer-to-peer, without a server. All participants are equal and can act as both client and server. Each computer maintains its own participant data.

### User Interface and Implementation

The user interface will be graphical and implemented using **MFC** in **Visual C++**. It will allow:

*   sending messages,
*   receiving messages,
*   adding IP addresses to a contact list,
*   removing IP addresses,
*   selecting a message recipient,
*   simulating and detecting transmission errors.

The contact list will be implemented using a simple linked list.
