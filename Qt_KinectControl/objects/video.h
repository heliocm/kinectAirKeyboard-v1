#ifndef VIDEO_H
#define VIDEO_H

//this whole code is an adaptation of the demo "glview", written in c++ so it can generate
//an image that, based on a distance we will call dimension, draws red from
//the dimension to the kinect and white from the dimension to the oposit direction
//of the kinect

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <objects/libfreenect.h>
#include <pthread.h>
#include <GL/freeglut.h>
#include <math.h>
#include <objects/kinect.h>

void generateImage(int d, int dist, bool invert, bool buttons, int preview, Kinect & ki, int x, int y, int width, int height);
//d is the distance between the kinect and the dimension
//dist is the distance divided by 2 that will be draw by the kinect
//invert defines if the image will be mirrored or not

#endif // VIDEO_H
