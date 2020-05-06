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

	sleep (1);

	//check if connect failed
	if (rd != 0 && rc != 0)
    {
        printf("ERROR: ZeroMQ error occurred during zmq_ctx_new(): %s\n", zmq_strerror(errno));

        return EXIT_FAILURE;
    }

	//bericht
	const char *berichtJoin = (argc > 1)? argv [1]: "BlackJack>join?>";
	const char *berichtGame = (argc > 1)? argv [1]: "BlackJack>game?>";
	//filter instellen
	const char *filterJoin = (argc > 1)? argv [1]: "BlackJack>join!>";
	const char *filterGame = (argc > 1)? argv [1]: "BlackJack>game!>";

    //making char for text
	char buf [256];
	char name[50];
	char textJoin[60];
	char filterPlayer[50];
	char action;
	//asking player for name
	printf("Enter your name: ");
	scanf("%s", name);

	//parsing text to send
	strcpy(textJoin, berichtJoin);
	strcat(textJoin, name);
	strcat(textJoin, ">");

	//setup player filter
	strcpy(filterPlayer, "BlackJack>game!>");
	strcat(filterPlayer, name);
	strcat(filterPlayer, ">");
	printf("Filterplayer: %s\n", filterPlayer);

	//setup receive message
	zmq_msg_t message;
	rc = zmq_msg_init (&message);


	do{
		//sending join with name to server
		rd = zmq_send(publisher, textJoin, strlen(textJoin), 0); printf("message send\n");
		assert (rd == strlen(textJoin));
	
		//waiting until server has enough players
		printf("Wating for players\n");
		rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filterJoin, strlen (filterJoin));
		rc = zmq_recv (subscriber, buf, 256, 0);
    	printf("Message received: %s\n\n",buf);
    	memset(buf,0,256);

		//receive first message with cards
		rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filterGame, strlen (filterGame));
		rc = zmq_recv (subscriber, buf, 256, 0);
    	printf("Message received: %s\n\n",buf);
    	memset(buf,0,256);

    
    	//do 	//receiving player instructions
    	//{
   			rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filterPlayer, strlen (filterPlayer));
			rc = zmq_recv (subscriber, buf, 256, 0);
   			printf("action received: %s\n\n",buf);
   			memset(buf,0,256);

   			//entering action and sending
   			printf("enter your action: ");
			action = getchar();
		//}while(action != 's');

		//waiting for winner anouncement
		printf("Waiting for winner anouncement...");
		rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filterGame, strlen (filterGame));
		rc = zmq_recv (subscriber, buf, 256, 0);
    	printf("Message received: %s\n\n",buf);
    	memset(buf,0,256);

    	//play again
    	printf("Do you want to play again?:");
    	action = getchar();
	}while(action == 'y');
	//free (string);
	zmq_msg_close (&message);
	zmq_close(publisher);
	zmq_close( subscriber );
	zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
	zmq_ctx_term( context ); //cleaning lady goes to work
	printf("KTNXBYE!\n");
	return 0;
}