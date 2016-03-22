//4 Buttons Kinect - Define 4 botões através de 4 posições definidas pelo programador

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "libfreenect.h"
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define bta 0x0061	//Define o Botão A através de um código KeySym
#define btb 0x0062	//Define o Botão B através de um código KeySym
#define btc 0x0063	//Define o Botão C através de um código KeySym
#define btd 0x0064	//Define o Botão D através de um código KeySym

// As letras de 'a' até 'z' o codigo vai de 0x0061 até 0x0086

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Inicializando variaveis, onde alguns dos valores sao pre-definidos para o bom funcionamento do kinect


//////////////////////////////////////////////////////////////////////////////////////////////////////////
int depth;
char *display_name;

Display *display;
Window main_window;
Window root_window;

pthread_t freenect_thread;
volatile int die = 0;

int g_argc;
char **g_argv;

int window;

float tmprot = 1;

pthread_mutex_t gl_backbuf_mutex = PTHREAD_MUTEX_INITIALIZER;

uint8_t gl_depth_front[640*480*4];
uint8_t gl_depth_back[640*480*4];

uint8_t gl_rgb_front[640*480*4];
uint8_t gl_rgb_back[640*480*4];

GLuint gl_depth_tex;
GLuint gl_rgb_tex;

freenect_context *f_ctx;
freenect_device *f_dev;

int freenect_angle = 17;
int freenect_led;
int snstvty;

int standwait = 0;
float pusx = 0, pusy = 0;

float pointerx = 0, pointery = 0;

float incerx = 12.8 , incery = 9.6;

//Tomando o canto inferior esquerdo como ponto inicial:
//   Botão A  | Zona Neutra |  Botão B
// -----------|-------------|------------  yposab
// Zona Neutra| Zona Neutra | Zona Neutra
// -----------|-------------|------------  yposcd
//   Botão C  | Zona Neutra |  Botão D
//         xposac        xposbd

float xposac = 200.0f;
float xposbd = 440.0f;
float yposcd = 150.0f;
float yposab = 330.0f;


pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;
int got_frames = 0;

//Funções prontas para a manipulação gráfica
void DrawGLScene()
{
    pthread_mutex_lock(&gl_backbuf_mutex);

    while (got_frames < 2)
    {
    	pthread_cond_wait(&gl_frame_cond, &gl_backbuf_mutex);
    }
    memcpy(gl_depth_front, gl_depth_back, sizeof(gl_depth_back));
    memcpy(gl_rgb_front, gl_rgb_back, sizeof(gl_rgb_back));
    got_frames = 0;
    pthread_mutex_unlock(&gl_backbuf_mutex);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, gl_depth_front);

    glTranslated(1280, 0, 0);
    glScalef(-1, 1, 1);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(0,0,0);
    glTexCoord2f(1, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 1); glVertex3f(640,480,0);
    glTexCoord2f(0, 1); glVertex3f(0,480,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, gl_rgb_front);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 0); glVertex3f(1280,0,0);
    glTexCoord2f(1, 1); glVertex3f(1280,480,0);
    glTexCoord2f(0, 1); glVertex3f(640,480,0);
    glEnd();

    glutSwapBuffers();
}


//Função para regular o Kinect(caso haja problemas com o mapeamento dos botões, desabilitar esta função)
void keyPressed(unsigned char key, int x, int y)
{
    switch(key)
    {
    	case 27:
    	    die = 1;
            pthread_join(freenect_thread, NULL);
            glutDestroyWindow(window);
            pthread_exit(NULL);
            break;
        case 'w':
            if (freenect_angle < 29) freenect_angle++;
            freenect_set_tilt_degs(f_dev,freenect_angle);
	    cout<<"Angulo: "<<freenect_angle<<" graus"<<endl;
            break;
        case 's':
            freenect_angle = 0;
            freenect_set_tilt_degs(f_dev,freenect_angle);
	    cout<<"Angulo: "<<freenect_angle<<" graus"<<endl;
            break;
        case 'x':
            if (freenect_angle > -30) freenect_angle--;
            freenect_set_tilt_degs(f_dev,freenect_angle);
            cout<<"Angulo: "<<freenect_angle<<" graus"<<endl;
            break;
        case '1':
            freenect_set_led(f_dev,LED_GREEN);
            break;
        case '2':
            freenect_set_led(f_dev,LED_RED);
            break;
        case '3':
            freenect_set_led(f_dev,LED_YELLOW);
            break;
        case '4':
            freenect_set_led(f_dev,LED_BLINK_GREEN);
            break;
        case '5':
            freenect_set_led(f_dev,LED_BLINK_GREEN);
            break;
        case '6':
            freenect_set_led(f_dev,LED_BLINK_RED_YELLOW);
            break;
        case '0':
            freenect_set_led(f_dev,LED_OFF);
            break;
        case 'o':
            tmprot+=0.1;
	    cout<<tmprot<<endl;
            break;
        case 'p':
            tmprot-=0.1;
	    cout<<tmprot<<endl;
            break;
    }
}

void ReSizeGLScene(int Width, int Height)
{
    glViewport(0,0,Width,Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, 1280, 480, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void InitGL(int Width, int Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &gl_depth_tex);
    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &gl_rgb_tex);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ReSizeGLScene(Width, Height);
}

void *gl_threadfunc(void *arg)
{
    cout<<"GL thread"<<endl;

    glutInit(&g_argc, g_argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(1280, 480);
    glutInitWindowPosition(0, 0);

    window = glutCreateWindow("4ButtonsKinect");

    glutDisplayFunc(&DrawGLScene);
    glutIdleFunc(&DrawGLScene);
    glutReshapeFunc(&ReSizeGLScene);
    glutKeyboardFunc(&keyPressed);

    InitGL(1280, 480);

    glutMainLoop();

    return NULL;
}

uint16_t t_gamma[2048];

void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
    int i;
    int first = 0;
    int px = 0 , py = 0;
    int tx = 0 , ty = 0;
    int alert = 0;
    uint16_t *depth = (uint16_t *)v_depth;

    pthread_mutex_lock(&gl_backbuf_mutex);
    for (i=0; i<640*480; i++)
    {
    	int pval = t_gamma[depth[i]];
        int lb = pval & 0xff;

        tx++;
        if(tx >= 640)
        {
            tx = 0;
            ty++;
        }
        switch (pval>>8)
        {
            case 0:
        	gl_depth_back[3*i+0] = 255;
                gl_depth_back[3*i+1] = 0;
                gl_depth_back[3*i+2] = 0;
                alert++;
                if (!first)
                {
                	first = i;
                	px = tx;
                	py = ty;
                }
                break;
            case 1:
            	gl_depth_back[3*i+0] = 255;
            	gl_depth_back[3*i+1] = 255;
            	gl_depth_back[3*i+2] = 255;
            	break;
            default:
            	gl_depth_back[3*i+0] = 0;
            	gl_depth_back[3*i+1] = 0;
            	gl_depth_back[3*i+2] = 0;
            	break;
        }
    }

    if(alert > snstvty)
    {
    	cout<< "Muito perto"<<endl;
    }
    else
    {
    	if(first)
    	{

	    pointerx = ((px-640.0f) / -1);
            pointery = ((py-480.0f)/ -1);
	    //cout << pointerx << "px"<<endl;
	    //cout << pointery << "py"<<endl;

            if((pusx <= (pointerx + incerx)) && (pusx >= (pointerx-incerx)) && (pusy <= (pointery+incery)) && (pusy >= (pointery-incery)))
            {
            	standwait++;
            	//cout<<standwait<<endl;
            }
            else
            {
            	pusx = pointerx;
                pusy = pointery;
                standwait = 0;
            }

            if(standwait > 8)
            {
				if(pointerx <= (xposac+incerx) && pointery >= (yposab-incery) )      //BOTÃO A
				{
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,bta), TRUE, CurrentTime);
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,bta), FALSE, CurrentTime);
					cout << "BOTAO A"<<endl;
					standwait = 4;
				}
				if(pointerx >= (xposbd-incerx) && pointery >= (yposab-incery))      //BOTÃO B
				{
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,btb), TRUE, CurrentTime);
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,btb), FALSE, CurrentTime);
					cout << "BOTAO B"<<endl;
					standwait = 4;
				}
				if(pointerx <= (xposac+incerx) && pointery <= (yposcd+incery))      //BOTÃO C
				{
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,btc), TRUE, CurrentTime);
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,btc), FALSE, CurrentTime);
					cout << "BOTAO C"<<endl;
					standwait = 0;
				}
				if(pointerx >= (xposbd-incerx) && pointery <= (yposcd+incery))      //BOTÃO D
				{
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,btd), TRUE, CurrentTime);
					XTestFakeKeyEvent(display, XKeysymToKeycode(display,btd), FALSE, CurrentTime);
					cout << "BOTAO D"<<endl;
					standwait = 0;
				}
            }
        XSync(display, 0);
        }
    }

    got_frames++;
    pthread_cond_signal(&gl_frame_cond);
    pthread_mutex_unlock(&gl_backbuf_mutex);
}


//Funções prontas
void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
    pthread_mutex_lock(&gl_backbuf_mutex);
    got_frames++;
    memcpy(gl_rgb_back, rgb, 640*480*3);
    pthread_cond_signal(&gl_frame_cond);
    pthread_mutex_unlock(&gl_backbuf_mutex);
}

void *freenect_threadfunc(void *arg)
{
    freenect_set_tilt_degs(f_dev,freenect_angle);
    freenect_set_led(f_dev,LED_GREEN);
    freenect_set_depth_callback(f_dev, depth_cb);
    freenect_set_video_callback(f_dev, rgb_cb);
    //freenect_set_video_format(f_dev, FREENECT_VIDEO_RGB);
    //freenect_set_depth_format(f_dev, FREENECT_DEPTH_11BIT);

    freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB));
    freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
    freenect_start_depth(f_dev);
    freenect_start_video(f_dev);

    cout << " 'W' -Tilt Up, 'S'- Level, 'X'-Tilt Down, '0'-'6'- LED Mode<<endl" << endl;  //Algumas funcoes que voce pode fazer com o kinect

    while(!die && freenect_process_events(f_ctx) >= 0 )
    {
    	freenect_raw_tilt_state* state;
        freenect_update_tilt_state(f_dev);
        state = freenect_get_tilt_state(f_dev);;
        double dx,dy,dz;
        freenect_get_mks_accel(state, &dx, &dy, &dz);
        //cout<<"raw acceleration: "<<ax <<ay <<az<<"mks acceleration: "<<dx<<dy<<dz<<endl;
        fflush(stdout);
    }

    cout<<"Desligando streams..."<<endl;

    freenect_stop_depth(f_dev);
    freenect_stop_video(f_dev);

    freenect_close_device(f_dev);
    freenect_shutdown(f_ctx);

    cout<<"-- done!"<<endl;
    return NULL;
}


int main(int argc, char **argv)
//Inicialização das funções e rotinas, o programa somente é executado seo kinect estiver conectado e for identificado
{
	int res;

	cout<<"### 4ButtonsKinect ###"<<endl;

	if (argc == 2)
	{
		snstvty = atoi(argv[1]);
	}
	else
	{
		snstvty = 20000;
	}

    mousemask(ALL_MOUSE_EVENTS, NULL);

    display = XOpenDisplay(0);

    root_window = DefaultRootWindow(display);

    int i;
    for (i=0; i<2048; i++)
    {
    	float v = i/2048.0;
    	v = powf(v, 3)* 6;
        t_gamma[i] = v*6*256;
    }

    g_argc = argc;
    g_argv = argv;

    if (freenect_init(&f_ctx, NULL) < 0)
    {
    	cout<<"Inicializacao falhou"<<endl;
    	return 1;
    }

    freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);

    int nr_devices = freenect_num_devices (f_ctx);
    cout<<"Numero de dispositivos encontrados = "<<nr_devices<<endl;

    int user_device_number = 0;
    if (argc > 1) user_device_number = atoi(argv[1]);
    if (nr_devices < 1) return 1;

    if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0)
    {
    	cout<<"Kinect nao encontrado"<<endl;
    	return 1;
    }

    res = pthread_create(&freenect_thread, NULL, freenect_threadfunc, NULL);

    if (res)
    {
    	cout<<"Tentativa de criar thread falhou"<<endl;
        return 1;
    }

    gl_threadfunc(NULL);

    return 0;
}
