
all: *.cpp ../include/*
	g++ -o ../build/spectro *.cpp -I ../include -std=c++11 -lfreeimageplus -lsndfile -lGL -lGLU -lglut -lpulse -lpulse-simple -lpthread -lfann
debug: *.cpp ../include/*
	g++ -g -o ../build/spectro *.cpp -I ../include -std=c++11 -lfreeimageplus -lsndfile -lGL -lGLU -lglut -lpulse -lpulse-simple -lpthread -lfann