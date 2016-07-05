#ifndef MICINPUT_H
#define MICINPUT_H
#include <iostream>
#include <deque>
#include <vector>
#include <time.h>
#include <errno.h>
#include "pulse/simple.h"
#include "pulse/error.h"
#define BUFSIZE 1024

class MicInput
{
public:
	MicInput();

	void readMicFlow(std::vector<short> *ptr);

	void FlowRefresh(void);

	int size(void){
		return micFlow.size();
	}
	int channels(void){
		return ss.channels;
	}
	int samplerate(void){
		return ss.rate;
	}
private:
	std::deque<int> micFlow;
	pa_simple *paconn;
	pa_sample_spec ss;

	uint8_t buf[BUFSIZE];

	int error;


};

#endif