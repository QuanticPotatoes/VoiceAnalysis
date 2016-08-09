/*
 * Voice Analysis 
 * Copyright (C) Jérémy DIEUZE, 2016
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "Train.h"

using namespace std;

static fstream trainfile;
static fstream trainfile_output;
static string file_;
static int number_input_;
static int number_output_;

Train::Train(){

}

/**
 * @brief create a training file for the neural network
 *
 * @param file the name of the training file
 */
void Train::createFileTraining(string file){

	file_ = file + ".data";
}

void Train::initTrainingFile(int number_input, int number_output){

	number_input_ = number_input;
	number_output_ = number_output;

	trainfile.seekg(0,ios::beg); //go to the file begin
	trainfile << "0 " <<  number_input << " " << number_output;

}

void Train::addDataTrainingToFile(unsigned char *input, size_t size_input){

	char *tab = (char*) malloc(10);
	char *output = (char*) malloc(10);

	memset(tab,'a',10);
	memset(output,'b',10);

	if(trainfile.is_open()){
		trainfile.close();
	}
	trainfile.open(file_, ios::in); //File to read from

    int num, i;
    string strTemp, fileStr;
    trainfile >> num; // The number of examples to train

    for(i = 0; i < 3; i++){ // The first line

        if(i == 0){

        	num++;
        	strTemp = to_string(num);
        }

        fileStr += strTemp + " " ;

        if(i != 2){
        	trainfile >> strTemp;
        }

    }

    trainfile.ignore();

    while(getline(trainfile,strTemp)){ // All line since the new example

    	fileStr += strTemp + "\n";
    }

    trainfile.close();
    trainfile.open(file_, ios::out | ios::trunc);

    trainfile << fileStr << "\n";

    trainfile.close();
    trainfile.open(file_, ios::out | ios::app);

    fileStr.clear();

    for(int i = 0; i < size_input; i++) // The input data
    {

    	fileStr += to_string((int) *input);
    	fileStr +=  " ";

    	input++;
    	
    }

    fileStr += "\n";

    while(*output != '\0'){ // The results we expect
    	fileStr += "b" ;
    	fileStr +=  " ";
    	output++;
    }

    fileStr += "\n";
    trainfile << fileStr; // put the string in the file

    trainfile.close();

    fileStr.clear(); // the buffile string is clear
}