#ifndef MICINPUT_H
#define MICINPUT_H
#include <iostream>
#include <deque>
#include <mutex>
#include <vector>
#include <time.h>
#include <cmath>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "pulse/simple.h"
#include "pulse/error.h"
#include "LinkedList.h"

#define BUFSIZE 2048

class MicInput
{
public:
	MicInput();

	void readMicFlow(std::vector<short> *ptr);

	void FlowRefresh(void);

	int frames(void);
	int channels(void);
	int samplerate(void);


private:

	std::mutex m;
	
	std::deque<short> micFlow;
	LinkedList<short> flowMic;
	pa_simple *paconn;
	pa_sample_spec ss;
	short buf[BUFSIZE];
	int error;

};

#endif