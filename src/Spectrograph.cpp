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
#include "fann.h"
#include "fann_cpp.h"
#include "Train.h"
#include <iostream>
#include <cassert>
#include <chrono>
#include <cmath>

    static bool detect;
    static int x_detect,x_image;
    int b;
    int num_chunks;
    static Train TrainingFile;
    // Variable in save image
    static RGBQUAD color;
    static float ratio;
    static int data_size_used;
    static double log_coef;
    static int data_size_image;
    static int freq;
    static int x,y,w,z;
    static int number_voice; // Number of voice frames detected
    static const short w_max = 1;
    static bool VoiceContinuous;
    static LinkedList<RGBQUAD>::Node *tmp;
    static LinkedList<RGBQUAD> VoiceList;
    static float chunk_width_ratio;
    // Variable in compute
    static int STEP;
    static int new_size;
    static unsigned char number_of_core;


    typedef struct taskStruc {
    Spectrograph *spectrum = NULL;
    MicInput *mic = NULL;
    } taskStruc;

Spectrograph::Spectrograph(std::string fname, int width, int height) :
    fname_(fname), file_handle_(fname), width_(width), height_(height),
    window_(Utility::blackman_harris) {

    spectroRefresh();

    TrainingFile.createFileTraining("voice");

        // Color for our plot
    // Black
    gradient_.add_color({  0,   0,   0,   0});
    // Purple
    gradient_.add_color({ 55,   0, 110,   0});
    // Blue
    gradient_.add_color({  0,   0, 180,   0});
    // Cyan
    gradient_.add_color({  0, 255, 255,   0});
    // Green
    gradient_.add_color({  0, 255,   0,   0});
    // Green Yellow
    // Yellow
    gradient_.add_color({255, 255,   0,   0});
    // Orange
    gradient_.add_color({230, 160,   0,   0});
    // Red
    gradient_.add_color({255,   0,   0,   0});


    if (!file_handle_){
        std::cerr << "Error Loading file " << fname << std::endl;
    } else {
      if(fname_ != "--live"){
      read_in_data();
        }
    }    




}


void *ThreadTask(void *arg){

    //Get the number of the core
    cpu_set_t cpuset;
    number_of_core = sysconf(_SC_NPROCESSORS_ONLN);

    std::cout << "Number of cores used : " << sysconf(_SC_NPROCESSORS_ONLN) << std::endl;
    CPU_ZERO(&cpuset);
    CPU_SET(number_of_core,&cpuset);

    sched_setaffinity(0, sizeof(cpuset), &cpuset);

    taskStruc *tts = (taskStruc*) arg;
    
    tts->spectrum->read_in_data();

    while(1){

    
    tts->mic->FlowRefresh();

    tts->spectrum->sendToMicFlow();

    tts->spectrum->compute(1024, 0.99);

    tts->spectrum->save_image("spectrogram.png", true);

    usleep(10000);
    }
}

void Spectrograph::spectroRefresh(void){

    if(fname_ == "--live"){
    pthread_t t;

    taskStruc *ts; // Struc which contains the pointer spectrograph and micinput
    ts->spectrum = this;

    ts->mic = &micinput;

    pthread_create(&t,NULL,ThreadTask,(void*)ts);

    pthread_detach(t);

    }
    
}

void Spectrograph::set_window(std::function<double(int, int)> window){
    window_ = window;
}

bool Spectrograph::file_is_valid(){
    return file_handle_;
}

void Spectrograph::sendToMicFlow(){

    micinput.readMicFlow(&data_);

}

void Spectrograph::read_in_data(){

        PixelArray = (unsigned char*) malloc(sizeof(unsigned char*) * height_ * width_ * 3);
        VoiceArray = (unsigned char*) malloc(sizeof(unsigned char*) * height_ * width_ * 3);

        // Remplis la liste chaine de 0
        for (int m = 0; m < width_; ++m)
        {
            list.push(height_);
            memset(list.front()->data,0,sizeof(list.front()->data));
        }

    if(fname_ == "--live"){

        int audio_length_sec = micinput.frames() / micinput.samplerate();

        const int data_size = micinput.frames();

        data_ = std::vector<short>(data_size, 0);

        micinput.readMicFlow(&data_);

        max_frequency_ = micinput.samplerate() * 0.5;

    }
    else {

        std::cout << "Reading in file: " << fname_ << std::endl;

        int audio_length_sec = file_handle_.frames() / file_handle_.samplerate();
        std::cout << "Length (s): " << audio_length_sec << std::endl;

        const int data_size = file_handle_.channels() * file_handle_.frames();

        data_ = std::vector<short>(data_size, 0);

        file_handle_.read(&data_[0], data_.size());

        max_frequency_ = file_handle_.samplerate() * 0.5;

        compute(1024,0.99);
        save_image("coucou.png",true);

    }



}

std::complex<double> Spectrograph::omega(float p, float q){

    const float trig_arg = 2 * M_PI * q / p;
    // VVV Comment out this line to use the cache VVVV
    //return { cos(trig_arg), sin(trig_arg) };

    auto memo = omega_cache_.find(trig_arg);
    if (memo != omega_cache_.end()){
        return memo->second;
    } else {
        complex_d result = { cos(trig_arg), sin(trig_arg) };
        omega_cache_[trig_arg] = result;
        return result;
    }
}

void Spectrograph::save_image(
        std::string fname, 
        bool log_mode){

    
   
    if(!imageinit){

    
        data_size_image = spectrogram_.front().size();
        // Only the data below 1/2 of the sampling rate (nyquist frequency)
        // is useful
        float multiplier = 0.5;
        for (int i = 1; i < file_handle_.channels(); i++){
            multiplier *= 0.5;
        }
        data_size_used = data_size_image * multiplier;

        log_coef = 
            (1.0/log( (double) (height_ + 1))) * (double)(data_size_used);

        imageinit = true;
    }

    for (x = 0; x < spectrogram_.size(); x++){
        freq = 0;
        detect = false;

        if(list.first->v){
           list.first->v = false;
           number_voice--; 
        }

        list.recycle();

        //for(w = 0; w < w_max+1; w++){
        
        

            for (y = 1; y <= height_;  y++){

                color = get_color(spectrogram_[x][freq], 15);
                

                list.front()->data[y-1] = color;
                
                if(((int) color.rgbGreen) > 92 && ((int) color.rgbBlue) > 200 && !detect ){
                    
                    list.front()->data[y-1] = {255,255,255,0};

                    detect = true;
                    number_voice++;

                    list.front()->v = true;
                }

                if (log_mode){
                    freq = data_size_used - 1 - (int) (log_coef * log(height_ + 1 - y));
                } else {
                    ratio = (float) (y)/height_;
                    freq = (int) (ratio * data_size_used);
                }
            }
       //}

    }

    tmp = list.first;


    for (x = 0; x < width_; ++x)
    {

        for (y = 0; y < height_; ++y)
        {
            
            
            PixelArray[3*(x + y*width_) + 0] = tmp->data[y].rgbRed;
            PixelArray[3*(x + y*width_) + 1] = tmp->data[y].rgbGreen;
            PixelArray[3*(x + y*width_) + 2] = tmp->data[y].rgbBlue;          
            

        }

         tmp = tmp->prev;
    }


    if(number_voice > 0)
    {

        tmp = list.front();

        memset(VoiceArray,0,sizeof(char*) * height_ * width_ * 3);
        z = 0;

         for (x = 0; x < width_; ++x)
            {

        if(tmp->v)
        {

        for(w = 0; w < (float) width_ / (float) number_voice; w++){

            for (y = 0; y < height_; ++y)
                {
                    VoiceArray[3*(z + y*width_) + 0] = tmp->data[y].rgbRed;
                    VoiceArray[3*(z + y*width_) + 1] = tmp->data[y].rgbGreen;
                    VoiceArray[3*(z + y*width_) + 2] = tmp->data[y].rgbBlue;
                }

            z++;

          }

        }

            tmp = tmp->prev;
        }

        unsigned char lol[] = {'1','1','4'};
        std::cout << *VoiceArray ;
        TrainingFile.addDataTrainingToFile(VoiceArray, height_ * width_ * 3);
    }

     
}

void Spectrograph::get_color_by_position(int x, int y, RGBQUAD *color){

    FreeImage_GetPixelColor(bitmap,x,y,color);

}



RGBQUAD Spectrograph::get_color(std::complex<double>& c, float threshold){
    double value = 0.5 * std::log10(Utility::mag(c) + 1);
    gradient_.set_max(threshold);

    auto rgba_color = gradient_.get_color(value);

    return rgba_color.toFreeImageQuad();
}

void Spectrograph::compute(const int CHUNK_SIZE, const float OVERLAP){
    assert(0.0 <= OVERLAP && OVERLAP < 1.0);
    STEP = (int) (CHUNK_SIZE * (1.0 - OVERLAP));

    // Pad the data
    new_size = 0;
    while (new_size + CHUNK_SIZE < data_.size()){
        new_size += STEP;
    }
    if (new_size != data_.size()){
        new_size += CHUNK_SIZE;
        std::vector<short> padding(new_size - data_.size(), 0);
        data_.insert(data_.end(), padding.begin(), padding.end());
    }

    chunkify(CHUNK_SIZE, STEP);
    //read_in_spectrum();
}

int Spectrograph::get_number_of_chunks(const int CHUNK_SIZE, const int STEP){
    int i = 0;
    int chunks = 0;
    for( ; i + CHUNK_SIZE <= data_.size(); i += STEP){
        ++chunks;
    }
    if (i == data_.size()){
        chunks -= 1;
    }
    return chunks;
}

void Spectrograph::chunkify(const int CHUNK_SIZE, const int STEP){
    
    spectrogram_.clear();

    num_chunks = get_number_of_chunks(CHUNK_SIZE, STEP);

    if(fname_ != "--live"){

        spectrogram_.reserve(width_);
        chunk_width_ratio = (float)(num_chunks)/width_;
    }
    else
    {
        spectrogram_.reserve(2);
        chunk_width_ratio = (float)(num_chunks)/2;
    }
    

    int j = 0;
    float float_j = 0.0;

    while (j < num_chunks){
        float_j += chunk_width_ratio;
        j = (int)(float_j);

        spectrogram_.push_back(
                std::vector<complex_d>(
                    data_.begin() + j * STEP,
                    data_.begin() + j * STEP + CHUNK_SIZE
                    )
        );
        transform(spectrogram_.back());
    }
}

// TODO: Cache calculations of omega
void Spectrograph::transform(std::vector<std::complex<double>>& signal, int min_size){
    if (min_size < 0){
        min_size = signal.size();
    }
    int power = pad_to_power2(signal, min_size);

    if (!power){
        return;
    }

    std::vector<std::complex<double>> transformed(signal.size(), 0);
    // Apply window function and sort by bit-reversed index
    for (int i = 0; i < signal.size(); i++){
        transformed[Utility::reverse_bits(i, power)] = 
            signal[i] * window_(i, signal.size());
    }

    int n = 2;
    while (n <= transformed.size()){
        // Iterate over length-n segments
        for (int i = 0; i <= transformed.size() - n; i += n){
            // Combine each half of the segment
            for (int m = i; m < i + n/2; m++){
                complex_d term1 = transformed[m];
                complex_d term2 = omega(n, -m) * transformed[m + n/2];

                transformed[m]       = term1 + term2;
                transformed[m + n/2] = term1 - term2;
            }
        }
        n *= 2;
    }
    signal.assign(transformed.begin(), transformed.end());
} 


void Spectrograph::transform_recursive(
        std::vector<std::complex<double>>& signal, 
        int min_size){
    pad_to_power2(signal, signal.size());
    for (int i = 0; i < signal.size(); i++){
        signal[i] *= window_(i, signal.size());
    }
    _transform(signal);
}

void Spectrograph::_transform(
        std::vector<std::complex<double>>& signal, 
        int min_size){

    int n = signal.size();
    if (n == 1){
        return;
    }

    std::vector<std::complex<double>> f_even;
    std::vector<std::complex<double>> f_odd;
    f_even.reserve(signal.size());
    f_odd.reserve(signal.size());

    for (int i = 0; i < signal.size(); i++){
        if (i % 2){
            f_odd.push_back(signal[i]);
        } else {
            f_even.push_back(signal[i]);
        }
    }

    _transform(f_odd);
    _transform(f_even);

    for (int m = 0; m < n/2; m++){
        complex_d w = omega(n, -m);
        signal[m] = f_even[m] + w * f_odd[m];
        signal[m + n/2] = f_even[m] - w * f_odd[m];
    }
}

void Spectrograph::pad(std::vector<std::complex<double>>& signal, int new_size){
    if (new_size > signal.size()){
        std::vector<std::complex<double>> padding(new_size - signal.size(), 0);
        signal.insert(signal.end(), padding.begin(), padding.end());
    }
}

int Spectrograph::pad_to_power2(std::vector<std::complex<double>>& signal, int min_size){
    int power = 1;
    int new_size = 2;

    while (new_size < min_size){
        new_size *= 2;
        power++;
    }
    pad(signal, new_size);
    return power;
}


