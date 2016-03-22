#include "video.h"
//this whole code is an adaptation of the demo "glview" so it can generate
//an image that, based on a distance we will call dimension, draws red from
//the dimension to the kinect and white from the dimension to the oposit direction
//of the kinect

//these are the global variables that openGl uses to generate the image
pthread_t freenect_thread;
volatile int die = 0;

int g_argc;
char **g_argv;

int window;

pthread_mutex_t gl_backbuf_mutex = PTHREAD_MUTEX_INITIALIZER;

// back: owned by libfreenect (implicit for depth)
// mid: owned by callbacks, "latest frame ready"
// front: owned by GL, "currently being drawn"
uint8_t *depth_mid, *depth_front;
uint8_t *rgb_back, *rgb_mid, *rgb_front;

GLuint gl_depth_tex;
GLuint gl_rgb_tex;
GLfloat camera_angle = 0.0;
int camera_rotate = 0;
int tilt_changed = 0;

freenect_context *f_ctx;
freenect_device *f_dev;
int freenect_angle = 0;
int freenect_led;

freenect_video_format requested_format = FREENECT_VIDEO_RGB;
freenect_video_format current_format = FREENECT_VIDEO_RGB;

pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;
int got_rgb = 0;
int got_depth = 0;
uint16_t t_gamma[2048];

//these global variable were created to be used on the logic of the KinectControl

Kinect* kin;
int dimension;
int distanc;
int** pressed;
bool invertScreen;
bool drawButtons;
int previewMode;

//this will loop, it generates the image of openGl
void DrawGLScene()//the program will loop here
{
    //this part is originaly from openGl
    pthread_mutex_lock(&gl_backbuf_mutex);

    // When using YUV_RGB mode, RGB frames only arrive at 15Hz, so we shouldn't force them to draw in lock-step.
    // However, this is CPU/GPU intensive when we are receiving frames in lockstep.
    if (current_format == FREENECT_VIDEO_YUV_RGB) {
        while (!got_depth && !got_rgb) {
            pthread_cond_wait(&gl_frame_cond, &gl_backbuf_mutex);
        }
    } else {
        while ((!got_depth || !got_rgb) && requested_format != current_format) {
            pthread_cond_wait(&gl_frame_cond, &gl_backbuf_mutex);
        }
    }

    if (requested_format != current_format) {
        pthread_mutex_unlock(&gl_backbuf_mutex);
        return;
    }

    uint8_t *tmp;

    if (got_depth) {
        tmp = depth_front;
        depth_front = depth_mid;
        depth_mid = tmp;
        got_depth = 0;
    }
    if (got_rgb) {
        tmp = rgb_front;
        rgb_front = rgb_mid;
        rgb_mid = tmp;
        got_rgb = 0;
    }
    pthread_mutex_unlock(&gl_backbuf_mutex);
    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, depth_front);

    glLoadIdentity();
    glPushMatrix();
    glTranslatef((640.0/2.0),(480.0/2.0) ,0.0);
    glTranslatef(-(640.0/2.0),-(480.0/2.0) ,0.0);
    //draws the depth preview
    if(previewMode!=1){
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f(0, 1); glVertex3f(0,0,1.0);
        glTexCoord2f(1, 1); glVertex3f(640,0,1.0);
        glTexCoord2f(1, 0); glVertex3f(640,480,1.0);
        glTexCoord2f(0, 0); glVertex3f(0,480,1.0);
        glEnd();
        glPopMatrix();
    }
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    if (current_format == FREENECT_VIDEO_RGB || current_format == FREENECT_VIDEO_YUV_RGB)
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgb_front+640*4);

    glPushMatrix();
    glTranslatef(640+(640.0/2.0),(480.0/2.0) ,0.0);
    glTranslatef(-(640+(640.0/2.0)),-(480.0/2.0) ,0.0);
    //draws the colored preview
    if(previewMode == 0){
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f(0, 1); glVertex3f(640,0,0);
        glTexCoord2f(1, 1); glVertex3f(1280,0,0);
        glTexCoord2f(1, 0); glVertex3f(1280,480,0);
        glTexCoord2f(0, 0); glVertex3f(640,480,0);
        glEnd();
    }
    else if(previewMode == 1){
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f(0, 1); glVertex3f(0,0,0);
        glTexCoord2f(1, 1); glVertex3f(640,0,0);
        glTexCoord2f(1, 0); glVertex3f(640,480,0);
        glTexCoord2f(0, 0); glVertex3f(0,480,0);
        glEnd();
    }
    //if drawButtons is true, this parts draws each button on the screen, not from the original openGl
    if(drawButtons){
        deque<KKey*> keys = kin->getKeys();
        KKey* tempKey;
        int x;
        int y;
        int width;
        int height;
        glColor3f(1.0,1.0,1.0);
        glPointSize(5);
        glLineWidth(5);
        while(!keys.empty()){
            tempKey = keys.front();
            keys.pop_front();
            tempKey->getLocation(x,y,width,height);
            glBegin(GL_LINES);
            glVertex3f(x,480-y,0);
            glVertex3f(x+width,480-y,0);
            glVertex3f(x+width,480-y,0);
            glVertex3f(x+width,480-y-height,0);
            glVertex3f(x+width,480-y-height,0);
            glVertex3f(x,480-y-height,0);
            glVertex3f(x,480-y-height,0);
            glVertex3f(x,480-y,0);
            glEnd();

            glBegin(GL_POINTS);
            glVertex3f(x,480-y,0);
            glVertex3f(x+width,480-y,0);
            glVertex3f(x+width,480-y-height,0);
            glVertex3f(x,480-y-height,0);
            glEnd();
            //draws the button on the right screen, if it exists
            if(previewMode == 0){
                glBegin(GL_LINES);
                glVertex3f(640+x,480-y,0);
                glVertex3f(640+x+width,480-y,0);
                glVertex3f(640+x+width,480-y,0);
                glVertex3f(640+x+width,480-y-height,0);
                glVertex3f(640+x+width,480-y-height,0);
                glVertex3f(640+x,480-y-height,0);
                glVertex3f(640+x,480-y-height,0);
                glVertex3f(640+x,480-y,0);
                glEnd();

                glBegin(GL_POINTS);
                glVertex3f(640+x,480-y,0);
                glVertex3f(640+x+width,480-y,0);
                glVertex3f(640+x+width,480-y-height,0);
                glVertex3f(640+x,480-y-height,0);
                glEnd();
            }
        }
    }
    glPopMatrix();
    glutSwapBuffers();


}


//originaly from the openGl, it makes sure the screen can be resized without problem
void ReSizeGLScene(int Width, int Height)
{
    glViewport(0,0,Width,Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(previewMode == 0)
        glOrtho (0, 1280, 0, 480, -5.0f, 5.0f);
    else
        glOrtho (0, 640, 0, 480, -5.0f, 5.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


//Starts the image
void InitGL(int Width, int Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClearDepth(0.0);
    //glDepthFunc(GL_LESS);
    //glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);

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

//makes sure that all tha has been alloced is deleted
void deleteAll(){
    delete[] depth_mid;
    delete[] depth_front;
    delete[] rgb_back;
    delete[] rgb_mid;
    delete[] rgb_front;


    for(int i = 0;i<640;i++)
        delete[] pressed[i];
    delete[] pressed;
}

//starts the thread
void *gl_threadfunc(int x, int y, int width, int height)
{
    printf("GL thread\n");

    glutInit(&g_argc, g_argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(x, y);

    window = glutCreateWindow("Kinect Control");

    glutDisplayFunc(&DrawGLScene);
    glutIdleFunc(&DrawGLScene);
    glutReshapeFunc(&ReSizeGLScene);
    glutCloseFunc(&deleteAll);

    InitGL(1280, 480);

    glutMainLoop();
    return NULL;
}


//it will loop, this controls how the depth detection of the kinect will be interpreted
void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
//modified from the original glview
{
    int i;
    uint16_t *depth = (uint16_t*)v_depth;

    pthread_mutex_lock(&gl_backbuf_mutex);
    int auxX = 0;
    int auxY = 0;
    if(invertScreen){//will define the color of each pixel, based on the depth and the invertScreen,
        //the ladder will indicate if the screen will be mirrored or not
        for (i=0; i<640*480; i++) {
            if(depth[auxY*640+auxX] <= dimension && depth[auxY*640+auxX] >=dimension - distanc){//red
                depth_mid[3*(auxY*640 + (639-auxX))+0] = 255;
                depth_mid[3*(auxY*640 + (639-auxX))+1] = 0;
                depth_mid[3*(auxY*640 + (639-auxX))+2] = 0;
                pressed[639-auxX][auxY] = in;
            }
            else if(depth[auxY*640+auxX] <= dimension + distanc && depth[auxY*640+auxX] > dimension){//white
                depth_mid[3*(auxY*640 + (639-auxX))+0] = 255;
                depth_mid[3*(auxY*640 + (639-auxX))+1] = 255;
                depth_mid[3*(auxY*640 + (639-auxX))+2] = 255;
                pressed[639-auxX][auxY] = out;
            }
            else{//black
                depth_mid[3*(auxY*640 + (639-auxX))+0] = 0;
                depth_mid[3*(auxY*640 + (639-auxX))+1] = 0;
                depth_mid[3*(auxY*640 + (639-auxX))+2] = 0;
                pressed[639-auxX][auxY] = 0;
            }
            if(++auxX == 640){
                auxY++;
                auxX = 0;
            }
        }
    }
    else{

        for (i=0; i<640*480; i++) {

            if(depth[i] <= dimension && depth[i] >=dimension - distanc){//red
                depth_mid[3*i+0] = 255;
                depth_mid[3*i+1] = 0;
                depth_mid[3*i+2] = 0;
                pressed[auxX][auxY] = in;
            }
            else if(depth[i] <= dimension + distanc && depth[i] > dimension){//white
                depth_mid[3*i+0] = 255;
                depth_mid[3*i+1] = 255;
                depth_mid[3*i+2] = 255;
                pressed[auxX][auxY] = out;
            }
            else{//black
                depth_mid[3*i+0] = 0;
                depth_mid[3*i+1] = 0;
                depth_mid[3*i+2] = 0;
                pressed[auxX][auxY] = 0;
            }
            if(++auxX == 640){
                auxY++;
                auxX = 0;
            }
        }
    }

    kin->checkKeys(pressed);
    got_depth++;
    pthread_cond_signal(&gl_frame_cond);
    pthread_mutex_unlock(&gl_backbuf_mutex);
}
//maybe what draws the preview
void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
    pthread_mutex_lock(&gl_backbuf_mutex);

    // swap buffers
    assert (rgb_back == rgb);
    rgb_back = rgb_mid;
    freenect_set_video_buffer(dev, rgb_back);
    //if invertScreen is true, this will rotate the preview


    rgb_mid = (uint8_t*)rgb;
    if(invertScreen){
        int swap;
        int aux1;
        int aux2;
        for(int i = 0;i<640/2;i++){
            for(int s = 0;s<480;s++){
                for(int k = 1;k<4;k++){
                    aux1 = 3*(i+640*s)+k;
                    aux2 = 3*(640-i+640*s)+k;
                    swap = rgb_mid[aux1];
                    rgb_mid[aux1] = rgb_mid[aux2];
                    rgb_mid[aux2] = swap;
                }
            }
        }
    }
    got_rgb++;
    pthread_cond_signal(&gl_frame_cond);
    pthread_mutex_unlock(&gl_backbuf_mutex);
}

void *freenect_threadfunc(void *arg)
{
    int accelCount = 0;
    freenect_set_led(f_dev,LED_GREEN);
    freenect_set_depth_callback(f_dev, depth_cb);

    freenect_set_video_callback(f_dev, rgb_cb);

    freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, current_format));
    freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
    freenect_set_video_buffer(f_dev, rgb_back);

    freenect_start_depth(f_dev);
    freenect_start_video(f_dev);

    while (!die && freenect_process_events(f_ctx) >= 0) {
        //Throttle the text output
        if (accelCount++ >= 2000)
        {
            accelCount = 0;
            freenect_raw_tilt_state* state;
            freenect_update_tilt_state(f_dev);
            state = freenect_get_tilt_state(f_dev);
            double dx,dy,dz;
            freenect_get_mks_accel(state, &dx, &dy, &dz);
            fflush(stdout);
        }

        if (requested_format != current_format) {
            freenect_stop_video(f_dev);
            freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, requested_format));
            freenect_start_video(f_dev);
            current_format = requested_format;
        }
    }

    printf("\nshutting down streams...\n");

    freenect_stop_depth(f_dev);
    freenect_stop_video(f_dev);

    freenect_close_device(f_dev);
    freenect_shutdown(f_ctx);

    printf("-- done!\n");
    return NULL;
}



void generateImage(int d, int dist, bool invert, bool buttons, int preview, Kinect & ki, int x, int y, int width, int height){
    drawButtons = buttons;
    invertScreen = invert;
    previewMode = preview;
    dimension = d - 100;
    distanc = dist;
    kin = &ki;
    int res;
    int argc = 0;
    char** argv = NULL;
    depth_mid = new uint8_t[640*480*3];
    depth_front = new uint8_t[640*480*3];
    rgb_back = new uint8_t[640*480*3];
    rgb_mid = new uint8_t[640*480*3];
    rgb_front = new uint8_t[640*480*3];
    pressed = new int*[640];

    for(int i = 0;i<640;i++)
        pressed[i] = new int[480];
    printf("Kinect Control\n");

    int i;
    for (i=0; i<2048; i++) {
        float v = i/2048.0;
        v = powf(v, 3)* 6;
        t_gamma[i] = v*6*256;
    }

    g_argc = argc;
    g_argv = argv;

    if (freenect_init(&f_ctx, NULL) < 0) {
        printf("freenect_init() failed\n");
        deleteAll();
        return;
    }

    freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

    int nr_devices = freenect_num_devices (f_ctx);
    printf ("Number of devices found: %d\n", nr_devices);

    int user_device_number = 0;
    if (argc > 1)
        user_device_number = atoi(argv[1]);

    if (nr_devices < 1) {
        freenect_shutdown(f_ctx);
        deleteAll();
        return;
    }

    if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
        printf("Could not open device\n");
        freenect_shutdown(f_ctx);
        deleteAll();
        return;
    }

    res = pthread_create(&freenect_thread, NULL, freenect_threadfunc, NULL);
    if (res) {
        printf("pthread_create failed\n");
        freenect_shutdown(f_ctx);
        deleteAll();
        return;
    }

    // OS X requires GLUT to run on the main thread
    gl_threadfunc(x,y,width,height);

    return;
}
