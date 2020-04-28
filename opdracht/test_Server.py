import zmq
import struct
import time

zmqfilter = "test123>join?>".encode('ascii')
topic = "test123>game!>hello>".encode('ascii')

context = zmq.Context()
publisher = context.socket(zmq.PUSH)
subscriber = context.socket(zmq.SUB)

publisher.connect("tcp://benternet.pxl-ea-ict.be:24041")
subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042")

subscriber.setsockopt(zmq.SUBSCRIBE, zmqfilter)

print("waiting for message")
message = subscriber.recv()
print("received message: %s" %message)

time.sleep(2)
print("sending reply")
publisher.send(b"test123>join!>hello")