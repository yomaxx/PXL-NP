import zmq
import struct
import time

port1 = "5555"
port2 = "5556"
context = zmq.Context()

#server reply socet
Socket1 = context.socket(zmq.REQ)
Socket2 = context.socket(zmq.REQ)

#connect socket
Socket1.bind("tcp://*:%s" % port1)
Socket2.bind("tcp://*:%s" % port2)

time.sleep(1)

####################################################################
while(1):
    time.sleep(1)
    Socket1.send(b"hello")
    Socket2.send(b"world")
    message1 = Socket1.recv()
    message2 = Socket2.recv()
    print(message1)
    print(message2)
    
    
    