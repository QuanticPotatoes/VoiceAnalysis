#!/bin/bash

if (( $EUID != 0 )); then
    echo "Please run as root"
    exit
fi

read -r -p "You want programming with the program or just use the executable ? [y/N] " response
if [[ $response =~ ^([yY][eE][sS]|[yY])$ ]]
then
    apt-get install -y libfreeimage* freeglut* libpulse-dev libpulse0-dbg libsndfile1*
else
    apt-get install -y libfreeimage3 freeglut3
fi