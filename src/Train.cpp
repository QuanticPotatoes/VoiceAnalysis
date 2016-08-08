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
 * @details [long description]
 * 
 * @param file [description]
 */
void Train::createFileTraining(string file){

	file_ = file + ".data";
	//trainfile.open(file + ".data", ios::out | ios::in);

}

void Train::initTrainingFile(int number_input, int number_output){

	number_input_ = number_input;
	number_output_ = number_output;

	trainfile.seekg(0,ios::beg); //go to the file begin
	trainfile << "5 " <<  number_input << " " << number_output;

}

void Train::addDataTrainingToFile(){

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
    trainfile >> num;

    for(i = 0; i < 3; i++){
 
        if(i == 0){

        	num++;
        	strTemp = to_string(num);
        }

        fileStr += strTemp + " " ;//output everything to buffer string

        trainfile >> strTemp; //it will check line from test to strTemp string
    }

    while(getline(trainfile,strTemp)){
    	fileStr += "\n" + strTemp;
    }

    trainfile.close();
    trainfile.open(file_, ios::out | ios::trunc); //File to read from

    cout << fileStr;
    trainfile << fileStr;

    /*trainfile.close();

    trainfile.open(file_, ios::out | ios::app);*/

    //trainfile << "coucou" << "\n";
   /* while(*tab != '\0'){
    	cout << *tab << endl;
    	trainfile << *tab << " ";
    	tab++;
    }

    trainfile << "\n";

    while(*output != '\0'){
    	trainfile << *output << " ";
    	output++;
    }

    trainfile << "\n";
*/
    trainfile.close();

}