#ifndef KKEY_H
#define KKEY_H

// sudo apt-get install libxtst-dev

#include<X11/Xlib.h>
#include<X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <string>
using namespace std;
typedef string keyType;
enum Mode {bumpOut=-2, out, empty, in, bumpIn};
class KKey
{
    keyType key;
    int x;
    int y;
    int height;
    int width;
    Mode mode;
    bool bump;
    //use to assist in case the button bumps

    int keyToVK(keyType c);
    //convert the char to an int that will be used to simulate the keypress

public:
    KKey(keyType key, int x, int y, int width, int height, Mode mode);
    //key represents the char of what key the kinect will simulate,
    //x,y is the coordinate in percentage, height and width is the dimension in percentage,
    //mode represents how the button will work, it can be:
    //
    //bumpOut: If an white image is detected on the button, the key will be pressed once, the only way to press
    //the key again will be after there are no other white image on the button
    //
    //out: Will press the key indefinitely if there is a white image on the button
    //
    //empty: Usually not used
    //
    //bumpIn: If an red image is detected on the button, the key will be pressed once, the only way to press
    //the key again will be after there are no other red image on the button
    //
    //out: Will press the key indefinitely if there is a red image on the button

    keyType getKey();
    //return the char

    Mode getMode();
    //return the mode

    void getLocation(int & x, int & y, int & width, int & height);
    //return every detail about the location of the button

    void pressKey(Display* display);
    //simulates the keypress of the char in the button

    void unBump();
    //allow buttons that bump to be pressed again

};

#endif // KKEY_H
