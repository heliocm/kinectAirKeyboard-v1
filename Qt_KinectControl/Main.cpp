#include <iostream>
#include "objects/video.h"
#include "objects/kinect.h"
#include "objects/textinterpreter.h"
using namespace std;
#include<X11/Xlib.h>
#include<X11/keysym.h>
#include <X11/extensions/XTest.h>

//this project was made by Braian Igreja de Freitas, for the course "Linguagem de programação", taught by
//Sergio Barbosa Villas-Boas


int main(){
    TextInterpreter ti;
    Kinect kin(640,480);
    int dim,dist,preview,x,y,width,height;
    bool invert,buttons;
    try{
    ti.getFromText(dim,dist,invert,buttons,preview,kin,x,y,width,height);
    cout << x << endl<< y << endl<< width << endl<< height << endl;
    generateImage(dim,dist,invert,buttons,preview,kin,x,y,width,height);
    }
    catch(FileException e){
        cout << "valid config file not found."<<endl;
    }

    return 0;
}
