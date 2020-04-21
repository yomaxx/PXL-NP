#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

int main(int argc, char const *argv[])
{
	const unsigned int INIT_PARAM = 12345;
    const unsigned int REPETITIONS = 10;
    const unsigned int PACKET_SIZE = 16;
	const char *TOPIC = "weather_update";

	void *context = zmq_ctx_new();
	if(!context)
	{
		printf("ERROR: ZMQ error during zmq_ctx_new: %s\n", zmq_strerror(errno));
		return EXIT_FAILURE;
	}
	
	void *data_socket = zmq_socket(context, ZMQ_PUB);
	const int rb = zmq_bind(data_socket, "tcp//*:5555");
	if(rb != 0)
	{
		printf("ERROR: ZMQ error during zmq_bind: %s\n", zmq_strerror(errno));
	}

	//topic can be used for filtering
	const size_t topic_size = strlen(TOPIC);
	const size_t envelope_size = topic_size + 1 + PACKET_SIZE * sizeof(int16_t);

	printf("Topic: %s; topic size: %zu; Envelope size: %zu\n", TOPIC, topic_size, envelope_size);

	return 0;
}
