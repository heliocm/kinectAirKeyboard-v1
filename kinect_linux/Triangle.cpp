#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#define ESCAPE 27               // ascii code for the escape key

using namespace std;
///////////////////////////////////////////////

float x_top  = 0.0f, x_right = x_top + 0.3f, x_left = x_top - 0.3f;
int color = 0, be = 0;
const char b1 = 'a', b2 = 'b' , b3 = 'd', b4 = 'c';

///////////////////////////////////////////////

int  window;         // The number of our GLUT window
int  Width;          // Width of window
int  Height;         // Height of window

int InitGL(int w, int h)
{
        glShadeModel(GL_SMOOTH);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //(0.0 < value < 1.0) Background, all 0.0 = black , all 1.0 = white

        glClearDepth(1.0f);                     //Depth Buffer inicialization
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        return true;
}

void RedraWorld()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
     glLoadIdentity();

     /* eh AQUI que você vai chamar todas as funcoes que
        desenham algo */

	if (be == 0)
	     {
	     switch(color)
		{

			case 0: //RED
			     glBegin(GL_TRIANGLES);                         // Drawing Using Triangles
	    			glColor3f(1.0f,0.0f,0.0f);		            // RGB
				glVertex3f( x_top, 0.3f, 0.0f);                 // Top            (0.0 < value < 1.0)
	    			glVertex3f( x_left,-0.3f, 0.0f);            // Bottom Left    (0.0 < value < 1.0)
	    			glVertex3f( x_right,-0.3f, 0.0f);           // Bottom Right   (0.0 < value < 1.0)
	     			glEnd();
			break;
			case 1: //GREEN
			     glBegin(GL_TRIANGLES);
	    			glColor3f(0.0f,1.0f,0.0f);
				glVertex3f( x_top, 0.3f, 0.0f);
	    			glVertex3f( x_left,-0.3f, 0.0f);
	    			glVertex3f( x_right,-0.3f, 0.0f);
	     			glEnd();
			break;
			case 2: //BLUE
			     glBegin(GL_TRIANGLES);
	    			glColor3f(0.0f,0.0f,1.0f);
				glVertex3f( x_top, 0.3f, 0.0f);
	    			glVertex3f( x_left,-0.3f, 0.0f);
	    			glVertex3f( x_right,-0.3f, 0.0f);
	     			glEnd();
			break;
		}
	     }

     glutSwapBuffers();

}


void ShoWorld(void)
{
        RedraWorld();
}

void ResizeWindow(int width, int height)
  {
    Width  = width;
    Height = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    }

void key(unsigned char tecla, int x, int y)
{
  switch ( tecla ) {
	case 27:    //Press ESC for terminate the program
		glutDestroyWindow(window);
		exit(0);
	break;
        case b1:
            if(be == 0)
	    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (x_left <= -1.0f)
		{
			x_top = -0.7f;
			x_right = -0.4;
			x_left = -1.0f;
		}
		else
		{
			x_top -= 0.1;
			x_right -= 0.1;
			x_left -= 0.1;
		}
	    }
	    else
	    {
		cout << "There is nothing to move"<< endl;
	    }
	    //cout << "x_left =" << x_left << endl;
            break;
        case b2:
            if(be == 0)
	    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (x_right >= 1.0f)
		{
			x_top = 0.7f;
			x_right = 1.0f;
			x_left = 0.4f;
		}
		else
		{
			x_top += 0.1;
			x_right += 0.1;
			x_left += 0.1;
		}
	    }
	    else
	    {
		cout << "There is nothing to move"<< endl;
	    }
	    //cout << "x_right =" << x_right << endl;
            break;
        case b3:
            if ( be == 0)
		{
			be = 1;
		}
	    else
		{
			be = 0;
		}
	    //cout << "be =" << be << endl;
            break;
        case b4:
            if(be == 0)
		{
			if (color >= 2)
			{
				color = 0;
			}
			else
			{
				color += 1;
			}
		}
		else
		{
			cout << "There is nothing to change color"<< endl;
		}
	    //cout << "color=" << color << endl;
            break;
  }
}

void key_arrow ( int tecla, int x, int y )
{
  switch ( tecla ) {
    case GLUT_KEY_UP:                   // Up arrow pressed
      glutFullScreen ( );               // go to full screen
      break;
    case GLUT_KEY_DOWN:                 // down arrow
      glutReshapeWindow ( 640, 480 );   // go to window 640 for 480
      break;
  }
}

int main(int  argc, char *argv[])
    {
        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

        glutInitWindowSize(640, 480);

        glutInitWindowPosition(0, 0);

        window = glutCreateWindow("Opengl Triangle");

        glutReshapeFunc(ResizeWindow);

        glutDisplayFunc(RedraWorld);

        glutKeyboardFunc(key);

        glutSpecialFunc(key_arrow);

        glutIdleFunc(ShoWorld);

        InitGL( 640, 480);

        glutMainLoop();
        return (0);
    }
