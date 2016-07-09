
#include "Window.h"


int Window::width;
int Window::height;
char* Window::easel;
bool r;

Spectrograph* Window::spectrograph;

Window::Window(int w,int h,Spectrograph *spectro,int *argc,char *argv[]){

	width = w;
	height = h;
	spectrograph = spectro;
	spectrograph->set_window(Utility::blackman_harris);

    std::string s = argv[1];
    if(s == "--live"){
      r = true;
    }

	spectrumInit(argc,argv);
}

void Window::spectrumInit(int *argc,char *argv[]){
	std::cout << "window init" << "\n";

	  easel = new char[width*height*3];


	  glutInitWindowSize(width,height);
	  glutInit(argc, argv);
	  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	  glutCreateWindow("Spectrograph");

	  // initialize some OpenGL state
	  myglinit();
	  // Init spectrum
	  
	  // set callback functions.  drawfunc is the function that gets
	  // called automatically as fast as possible.  keyfunc only is
	  // invoked if the user hits a keyboard key.


	  glutDisplayFunc(drawfunc);

	 glutMainLoop();
}

void Window::myglinit() {


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

}



// main draw function, gets called over and over, as fast as possible
void Window::drawfunc() {



  if(r){
  spectrograph->read_in_data();
  spectrograph->compute(1024, 0.8);
  spectrograph->save_image("spectrogram.png", true);
  }

RGBQUAD color;
  for(int x = 0; x < width; x++){
  	for(int y = 0; y < height; y++){

  		spectrograph->get_color_by_position(x,y,&color);
  		easel[3*(y*width+x) + 0] = color.rgbRed;
  		easel[3*(y*width+x) + 1] = color.rgbGreen;
  		easel[3*(y*width+x) + 2] = color.rgbBlue;
  	}
  }

  // drawpixels draws the rgb data stored in 'easel' to the screen
  glDrawPixels(width,height, GL_RGB, GL_UNSIGNED_BYTE, easel);


  // in double buffer mode so we swap to avoid a flicker
  glutSwapBuffers();

  // instruct event system to call 'drawfunc' again
  glutPostRedisplay();

  //nanosleep((const struct timespec[]){{0, 450000000L}}, NULL);
  usleep(450000);
}