#include "MicInput.h"
#include "Spectrograph.h"
#include <chrono>

Spectrograph *spectre = NULL;
void *MicTask(void *arg);

MicInput::MicInput(){

	ss.format = PA_SAMPLE_S16NE;
	ss.channels = 1;
	ss.rate = 44100;

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
}

void MicInput::init(void *s){

	spectre = (Spectrograph*) s;

	for (int i = 0; i < BUFSIZE; ++i)
	{
		micFlow.push_front(1000*sin(i));
	}

	ti = pthread_create(&t,NULL,MicTask,(void*)this);

    pthread_detach(t);

}

int MicInput::frames(void){

		return micFlow.size();
	}
int MicInput::channels(void){

		return ss.channels;
	}
int MicInput::samplerate(void){

		return ss.rate;
	}

void MicInput::readMicFlow(std::vector<short> *ptr){
		for(int i = 0; i < micFlow.size(); i++){

		(*ptr)[i] = micFlow[i];
	}
}

void MicInput::FlowRefresh(void){

	pa_simple_read(paconn,buf,sizeof(buf),&error);
	for(short i : buf){

		micFlow.pop_back(); //remove useless sound in back
		micFlow.push_front(i); // Add microphone sound in front
	}

}

void *MicTask(void *arg){
	
	MicInput *mic = (MicInput*) arg;

	while(1){

	mic->FlowRefresh();


	}
}