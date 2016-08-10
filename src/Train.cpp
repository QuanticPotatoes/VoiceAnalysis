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
static char *output;

Train::Train(){

}

/**
 * @brief create a training file for the neural network
 *
 * @param file the name of the training file
 */
void Train::createFileTraining(string file){

	file_ = file + ".dat";
}

void Train::initTrainingFile(int number_input, int number_output){

	number_input_ = number_input;
	number_output_ = number_output;

	trainfile.seekg(0,ios::beg); //go to the file begin
	trainfile << "0 " <<  number_input << " " << number_output;

}


void Train::addDataTrainingToFile(unsigned char *input, size_t size_input, int result){


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
        else if(i == 2){

        	number_output_ = stoi(strTemp); // Get the number of the output
        }

        fileStr += strTemp + " " ;

        if(i != 2){
        	trainfile >> strTemp;
        }

    }

    output = (char*) malloc(sizeof(char*) * number_output_);

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

    for(i = 0; i < size_input; i++) // The input data
    {

    	fileStr += to_string((float) *input / 100);
    	fileStr +=  " ";

    	input++;
    	
    }

    fileStr += "\n";

    for(i = 0; i < number_output_; i++){ // The results we expect
    	if(i == result){
    		fileStr += "1";
    	}
    	else {
    		fileStr += "-1";
    	}

    	fileStr +=  " ";
    }

    fileStr += "\n";
    trainfile << fileStr; // put the string in the file

    trainfile.close();

    fileStr.clear(); // the buffile string is clear
}