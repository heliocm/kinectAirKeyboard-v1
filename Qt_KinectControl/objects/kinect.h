#ifndef KINECT_H
#define KINECT_H
#include <deque>
#include <objects/kkey.h>
using namespace std;
class Kinect
{
    deque<KKey*> keys;//represent all the buttons added by the user
    KKey*** board;//it is a matrix m X n that locate every button position
    int m;
    int n;
    Display* display;//it is used to simulate keypresses
public:

    Kinect(int m, int n);
    //m is the width of the kinect and n is the height, in pixels

    virtual ~Kinect();

    int percentToPixel(int num,int scale);
    //convert a percentage unity(0%-100%) to the scale the kinect
    //is working on

    void addKey(keyType key, int x, int y, int height, int width, Mode mode);
    //add a key to the kinect

    void checkKeys(int** pressed);
    //compares pressed with board to see if a key was pressed

    deque<KKey*>  getKeys();
    //return the list of keys
};

#endif // KINECT_H
