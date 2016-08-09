#include <iostream>
#include <fstream>
#include <cstring>

class Train
{
public:
	Train();
	void createFileTraining(std::string file);
	void initTrainingFile(int number_input, int number_output);
	void addDataTrainingToFile(unsigned char *input, std::size_t size_input);
	
};