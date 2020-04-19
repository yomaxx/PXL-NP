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
	const char *filter = (argc > 1)? argv [1]: "example>task!>Max";
	const char *filter2 = (argc > 1)? argv [1]: "example>answer!>Max";

    //luisteren naar reactie en verzenden
	for(int i=0; i < 2; i++)
	{
		char buf [256];
		buf[0] ='\0';

		zmq_msg_t message;
	    rc = zmq_msg_init (&message);
		switch(i)
		{
			case 0:
				rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filter, strlen (filter));
				//send and receive
				rd = zmq_send(publisher, "example>task?>Max Valkenburg>", 29, 0); printf("message send\n");
				assert (rd == 29);
				break;
			case 1:
				
				rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filter2, strlen (filter2));
				//send and receive
				rd = zmq_send(publisher, "example>answer?>Max Valkenburg>CORONA-FREE-CHANNEL>4cb9aedf2d6c0a521be6af4d6d097eb78f868c83>*HATSJU*>", 101, 0); 
				assert (rd ==101);
				printf("answer send\n");
				break;
		}
		printf("Waiting for reply\n");
		rc = zmq_recv (subscriber, buf, 256, 0); assert(rc != -1);
    	int size = zmq_msg_size (&message);
    	char *string = malloc (size + 1);
    	memcpy (string, zmq_msg_data (&message), size);
    	zmq_msg_close (&message);
    	string [size] = 0;
    	printf("Message received: %s\n\n",buf);
		sleep(1);
	}
	//free (string);
	zmq_close(publisher);
	zmq_close( subscriber );
	zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
	zmq_ctx_term( context ); //cleaning lady goes to work
	printf("KTNXBYE!\n");
	return 0;
}