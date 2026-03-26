> COMPUTER NETWORKS 1 – exercises   
> Academic year 2008/09, summer semester

# Assignment 2 - Message Transmission

## **Task Description**

Over the UDP (User Datagram Protocol) transport-layer protocol of the TCP/IP network model, design and implement a protocol for message transmission between applications running on computers (sending and receiving messages – “chat” functionality).

The length of a message is not limited.  
The receiving application must be able to receive a message at any time, i.e., the receiving side does not need to be switched into a special receive mode in advance.

If necessary, the message will be transmitted in blocks. The maximum block size must be derived from the maximum length of the data field of an Ethernet frame.

The UDP protocol is unreliable; therefore, in the format of the designed protocol, consider including:

*   a checksum field, and
*   fields for possible communication reliability using a block-based ARQ (Automatic Repeat reQuest) method.

Use the same checksum as used by the UDP protocol (RFC 768).  
Detection of an incorrect checksum on the receiving side will only be indicated.

## **User Interface Requirements**

The user interface must allow the user to:

*   write a message and select the destination computer on which the receiving application is running,
*   display the received message and the IP address of the computer from which the message was sent,
*   display checksums:
    *   the checksum sent by the transmitter,
    *   the checksum received and the checksum recalculated by the receiver.

## **Functional Requirements**

The designed and implemented solution must meet the following requirements:

*   Message transmission between applications on different computers using a checksum
*   Splitting the message into suitable blocks
*   Indicating an error if it is detected on the receiving side (the communication error will be simulated)

## **Documentation Requirements**

The documentation **must include**:

*   specification of the format of the designed protocol, including a description of individual fields and a description of protocol operation,
*   justification of the chosen block size,
*   selection of any error protection mechanism and the acknowledgment method used,
*   description of the method used to simulate communication errors,
*   block diagram (architectural design) of the solution,
*   description of the user interface,
*   choice of implementation environment.

The documentation must also include the **assignment text itself**.