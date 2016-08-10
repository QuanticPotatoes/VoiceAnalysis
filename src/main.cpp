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

#include "Spectrograph.h"
#include "Utility.h"
#include "Window.h"
#include "Train.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{ 
    int w = 60, h = 150;
    //int w = 60, h = 250;
    RGBQUAD color;
    std::string fname = "--live";
    int number_output_train = -1;

    if (argc < 2){
        std::cout << "By default the program start with (--live)." << std::endl;
        argv[1] = (char*) "--live";
    }
    else{
        fname = argv[1]; 

        if(argc == 3){
            number_output_train = atoi(argv[2]);
        }
    }

    Spectrograph spectrograph(fname,w,h,number_output_train);

    if (!spectrograph.file_is_valid() && !(argv[1] == "--live")){
        return -1;
    }

    /*Train trainingFile;
    trainingFile.createFileTraining("voice");
    //trainingFile.initNeuralFile(1500,36);
    char lol[5] = {'1','1','1','1','1'};
    char v[5] = {2,3,5,6,6};
    trainingFile.addDataTrainingToFile(&lol[0]);*/

    Window window(w,h,&spectrograph,&argc,argv);
    return 0;
}
