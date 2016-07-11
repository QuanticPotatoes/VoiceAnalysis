#ifndef MICINPUT_H
#define MICINPUT_H
#include <iostream>
#include <deque>
#include <vector>
#include <time.h>
#include <cmath>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "pulse/simple.h"
#include "pulse/error.h"

#define BUFSIZE 1024

class MicInput
{
public:
	MicInput();

	void readMicFlow(std::vector<short> *ptr);

	void FlowRefresh(void);

	int frames(void);
	int channels(void);
	int samplerate(void);

	void init(void *s);


private:
	std::deque<short> micFlow;
	pa_simple *paconn;
	pa_sample_spec ss;
	
	short buf[BUFSIZE];

	int error;

	pthread_t t;
	int ti;


};

#endif