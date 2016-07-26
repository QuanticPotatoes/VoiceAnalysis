#include <iostream>
#include <stdlib.h>
#include <functional>
#include <GL/glut.h>

#include "Spectrograph.h"
#include "Utility.h"


class Window
{
public:
	Window(int width,int height,Spectrograph *spectro,int *argc,char *argv[]);
	void set_window();
	void draw_graph();
	static void drawfunc(void);
    static int width,height;
	static char *easel;
	static Spectrograph *spectrograph;
private:
	static Window* currentInstance;
	static void drawCallback(void);
	void setupDrawCallback(void);
	void keyfunc(unsigned char, int, int);
	void myglinit(void);
	void spectrumInit(int *argc,char *argv[]);



};