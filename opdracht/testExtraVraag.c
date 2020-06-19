#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int main( int argc, char * argv[] )
{	char buf [256];
	const char *string = "world";
	void *context = zmq_ctx_new();
	void *subscriber = zmq_socket (context, ZMQ_REP);
	int rs = zmq_connect( subscriber, "tcp://192.168.0.211:5555");

	zmq_msg_t message;
	rs = zmq_msg_init (&message);



	while(1)
	{
		rs = zmq_recv (subscriber, buf, 256, 0);
		rs = zmq_send(subscriber, string, strlen(string), 0);
		
		printf("%s", buf);
	}

}
