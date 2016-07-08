C++ Voice Analysis Program
===============

A Voice Analysis Program written in C++.

You will need the FreeImage, Pulse, glut libraries and a C++11 compatible compiler to compile the program.

Run with (assuming g++)
```
$ mkdir build & cd build
$ g++ -o spectro ../src/*.cpp -I ../include -std=c++11 -lfreeimageplus -lsndfile -lGL -lGLU -lglut -lpulse -lpulse-simple -lpthread
$ ./spectro <in default the param --live is active]
$ ./spectro <filename>
```

or run the Makefile in the src folder.

Copying
-------

Thanks to Christian Briones for his spectrogram generation program.

This source is released under the GPLv3 license.
Read the COPYING file for legal information.

