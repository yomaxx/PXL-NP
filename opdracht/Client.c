#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

char *parse(int keer, char *ParseString)
{
	char *String,*ParsedString;
	String = strdup(ParseString);
	for (int i = 0; i < keer; i++)
	{
		ParsedString = strsep(&String, ">");
	}
	return ParsedString;
}

int main( int argc, char * argv[] )
{
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket(context, ZMQ_PUSH);
	void *subscriber = zmq_socket (context, ZMQ_SUB);

	//connect
	int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
	int rs = zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

	sleep (1);

	//check if connect failed
	if (rp != 0 && rs != 0)
    {
        printf("ERROR: ZeroMQ error occurred during zmq_ctx_new(): %s\n", zmq_strerror(errno));

        return EXIT_FAILURE;
    }

	//bericht
	const char *berichtJoin = (argc > 1)? argv [1]: "BlackJack>join?>";
	const char *berichtGame = (argc > 1)? argv [1]: "BlackJack>game?>";
	const char *berichtWinner = (argc > 1)? argv [1]: "BlackJack>winner?>";
	//filter instellen
	const char *filterJoin = (argc > 1)? argv [1]: "BlackJack>join!>";
	const char *filterGame = (argc > 1)? argv [1]: "BlackJack>game!>";
	const char *filterWinner = (argc > 1)? argv [1]: "BlackJack>winner!>";

    //making chars
	char buf [256];
	char *ParsedString;
	char name[50];
	char textJoin[60];
	char textplay[60];
	char filterPlayer[50];
	char action[30];
	char play;
	//asking player for name
	printf("Enter your name: ");
	scanf(" %s", name);

	//parsing text to send
	strcpy(textJoin, berichtJoin);
	strcat(textJoin, name);
	strcat(textJoin, ">");

	//setup player filter
	strcpy(filterPlayer, "BlackJack>game!>");
	strcat(filterPlayer, name);
	strcat(filterPlayer, ">");

	//setup receive message
	zmq_msg_t message;
	rs = zmq_msg_init (&message);

	do{		//Game loop
		//sending join with name to server
		rp = zmq_send(publisher, textJoin, strlen(textJoin), 0);
		assert (rp == strlen(textJoin));
	
		//waiting until server has enough players
		printf("Wating for players\n");
		rs = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filterJoin, strlen (filterJoin));
		rs = zmq_recv (subscriber, buf, 256, 0);
		ParsedString = parse(3, buf);
    	printf("Message received: %s\n\n",ParsedString);
    	memset(buf,0,256);

		//receive first message with cards
		rs = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filterGame, strlen (filterGame));
		rs = zmq_recv (subscriber, buf, 256, 0);
		ParsedString = parse(3, buf);
    	printf("Message received: %s\n\n",ParsedString);
    	memset(buf,0,256);
    	rs = zmq_setsockopt (subscriber, ZMQ_UNSUBSCRIBE, filterGame, strlen (filterGame));

		rs = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filterPlayer, strlen (filterPlayer));
		rs = zmq_recv (subscriber, buf, 256, 0);
		ParsedString = parse(4, buf);
   		printf("action received: %s\n",ParsedString);

    	do 	//player turn loop
    	{
			//entering action and sending
			do{
   			printf("enter your action: ");
			scanf(" %s", action);
			}while(action[0] != 'h' && action[0] != 's');

			//parsing text to send
			strcpy(textplay, berichtGame);
			strcat(textplay, name); strcat(textplay, ">");
			strcat(textplay, action); strcat(textplay, ">");
			rp = zmq_send(publisher, textplay, strlen(textplay), 0);
			assert (rp == strlen(textplay));

   			//receiving instructions
   			//memset(token,0,256);
   			memset(buf,0,256);	
			rs = zmq_recv (subscriber, buf, 256, 0);

			ParsedString = parse(4, buf);
   			printf("action received: %s\n",ParsedString);
   			
		}while((action[0] == 'h') && (ParsedString[0] != '!'));

   		memset(buf,0,256);		
		//waiting for winner anouncement
		printf("Waiting for winner anouncement...\n");
		rs = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, filterWinner, strlen (filterWinner));
		rs = zmq_recv (subscriber, buf, 256, 0);
		ParsedString = parse(3, buf);
    	printf("Message received: %s\n\n",ParsedString);
    	memset(buf,0,256);

    	//play again
    	printf("Do you want to play again?(y/n):");
    	play = getchar();
    	play = getchar();
	}while(play == 'y');
	//free (string);
	zmq_msg_close (&message);
	zmq_close(publisher);
	zmq_close( subscriber );
	zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
	zmq_ctx_term( context ); //cleaning lady goes to work
	printf("KTNXBYE!\n");
	return 0;
}