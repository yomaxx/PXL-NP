#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int main( int argc, char * argv[] )
{
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket(context, ZMQ_PUSH);
	void *subscriber = zmq_socket (context, ZMQ_SUB);

	//connect
	int rd = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
	int rc = zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );
	sleep (3);

	//filter instellen
	const char *filter = (argc > 1)? argv [1]: "test123>join!>";

   	//luisteren naar reactie en verzenden

	char buf [256];
	buf[0] ='\0';

	zmq_msg_t message;
	rc = zmq_msg_init (&message);

	rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filter, strlen (filter));
	//send and receive
	rd = zmq_send(publisher, "test123>join?>Max Valkenburg>", 29, 0); printf("message send\n");
	assert (rd == 29);


	printf("Waiting for reply\n");
	rc = zmq_recv (subscriber, buf, 256, 0); assert(rc != -1);
    	int size = zmq_msg_size (&message);
    	char *string = malloc (size + 1);
   	 memcpy (string, zmq_msg_data (&message), size);
    	zmq_msg_close (&message);
    	string [size] = 0;
    	printf("Message received: %s\n\n",buf);
	sleep(1);
	
	//free (string);
	zmq_close(publisher);
	zmq_close( subscriber );
	zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
	zmq_ctx_term( context ); //cleaning lady goes to work
	printf("KTNXBYE!\n");
	return 0;
}
