#include "MicInput.h"
#include "Spectrograph.h"
#include <chrono>

static LinkedList<short>::Node *tmp;

MicInput::MicInput(){

	ss.format = PA_SAMPLE_S16NE;
	ss.channels = 1;
	//ss.rate = 44100;$
	//ss.rate = 22050;
	ss.rate = 16961;

	if(!(paconn = pa_simple_new(NULL,
						"micrecord",
						PA_STREAM_RECORD,
						NULL,
						"microphone",
						&ss,
						NULL,
						NULL,
						&error
						))){

		      fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", 
		      pa_strerror(error));
	}

		for (int i = 0; i < BUFSIZE; ++i)
	{
		flowMic.push(1)->data[0] = 0;
	}
}

int MicInput::frames(void){

		return BUFSIZE;
	}
int MicInput::channels(void){

		return ss.channels;
	}
int MicInput::samplerate(void){

		return ss.rate;
	}

void MicInput::readMicFlow(std::vector<short> *ptr){


		tmp = flowMic.front();


		for(int i = 0; i < BUFSIZE; i++){

		(*ptr)[i] = tmp->data[0];

		tmp = tmp->prev;

	}
		
	
}

void MicInput::FlowRefresh(void){

	
	pa_simple_read(paconn,buf,sizeof(buf),&error);

	
	for(short i : buf){


		flowMic.recycle(); //recycle useless sound in back
		flowMic.front()->data[0] = i*3; // Add microphone sound in front

		//micFlow.pop_back(); 
		//micFlow.push_front(i*2); 
	}
	
	

}