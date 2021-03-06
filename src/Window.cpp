
#include "Window.h"


  int Window::width;
  int Window::height;
  char* Window::easel;
  bool r;
  int multiplier = 1;

  static int x;
  static int y;
  static RGBQUAD color;

Spectrograph* Window::spectrograph;

Window::Window(int w,int h,Spectrograph *spectro,int *argc,char *argv[]){

	width = w*multiplier;
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

    glutInitWindowSize(width,height);
    glutCreateWindow("Voice");

    glutInitWindowPosition (500, 500);
    glutDisplayFunc(extractSound);

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
  
  // drawpixels draws the rgb data stored in 'easel' to the screen
  glDrawPixels(width,height, GL_RGB, GL_UNSIGNED_BYTE, spectrograph->PixelArray);


  // in double buffer mode so we swap to avoid a flicker
  glutSwapBuffers();

  // instruct event system to call 'drawfunc' again
  glutPostRedisplay();
 
  usleep(40000);
  
}

void Window::extractSound() {

  glDrawPixels(width,height, GL_RGB, GL_UNSIGNED_BYTE, spectrograph->VoiceArray);


  // in double buffer mode so we swap to avoid a flicker
  glutSwapBuffers();

  // instruct event system to call 'drawfunc' again
  glutPostRedisplay();
 
  usleep(40000);
}