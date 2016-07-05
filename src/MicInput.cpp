#include "MicInput.h"

MicInput::MicInput(void){

	ss.format = PA_SAMPLE_S16NE;
	ss.channels = 2;
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

void MicInput::readMicFlow(std::vector<short> *ptr){
		for(int i = 0; i < (*ptr).size(); i++){

		//std::cout << "test :" << (*ptr)[i] << "\n";
	}
}

void FlowRefresh(void){

}