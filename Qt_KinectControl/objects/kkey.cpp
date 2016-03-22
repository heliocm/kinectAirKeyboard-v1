#include "kkey.h"
KKey::KKey(keyType key, int x, int y, int width, int height, Mode mode)
{
    this->key = key;
    this->x = x;
    this->y = y;
    this->height = height;
    this->width = width;
    this->mode = mode;
    bump = false;
}
int KKey::keyToVK(keyType c){
    return XStringToKeysym(c.c_str());
}

keyType KKey::getKey(){
    return key;
}
Mode KKey::getMode(){
    return mode;
}

void KKey::getLocation(int & x, int & y, int & width, int & height){
    x = this->x;
    y = this->y;
    width = this->width;
    height = this->height;
}
void KKey::unBump(){
    bump = false;
}

void KKey::pressKey(Display* display){
    if(bump)
        return;
    unsigned int keycode = XKeysymToKeycode(display, keyToVK(key));
    XTestFakeKeyEvent(display,keycode,true,0);
    XTestFakeKeyEvent(display,keycode,false,0);
    XFlush(display);
    if(mode == 2 || mode == -2)
        bump = true;
}
