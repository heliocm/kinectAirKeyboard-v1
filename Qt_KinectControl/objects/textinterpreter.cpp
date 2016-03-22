#include "textinterpreter.h"

void TextInterpreter::getFromText(int & dim, int & dist, bool & invert, bool & buttons, int & preview, Kinect & kin, int & xR, int & yR, int & widthR, int & heightR){
    ifstream file;
    file.open("config");
    if(!file)
        throw FileException();
    string topic;
    string s;
    int x;
    int y;
    int width;
    int height;
    int m;
    widthR = 1280;
    heightR = 480;

    while(!file.eof()){
        file >> topic;
        if(topic == "Position:"){
            file >> xR;
            file >> yR;
            file >> widthR;
            file >> heightR;
        }
        else if(topic == "Dimension:")
            file >> dim;
        else if(topic =="Distance:")
            file >> dist;
        else if(topic =="Mirror:")
            file >> invert;
        else if(topic == "Show_buttons:")
            file >> buttons;
        else if(topic == "Preview:")
            file >> preview;
        else if(topic == "Kinect:"){
            file >> topic;
            while(topic == "Key:"){

                file >> s;
                file >> topic;
                if(topic != "X:")
                    throw FileException();
                file >> x;
                file >> topic;
                if(topic != "Y:")
                    throw FileException();
                file >> y;
                file >> topic;
                if(topic != "Width:")
                    throw FileException();
                file >> width;
                file >> topic;
                if(topic != "Height:")
                    throw FileException();
                file >> height;
                file >> topic;
                if(topic != "Mode:")
                    throw FileException();
                file >> m;
                if(x+width > 100){
                    width = 100-x;
                }
                if(y+height > 100){
                    height = 100 - y;
                }
                if(x < 0){
                    x = 0;
                    width += x;
                }
                if(y < 0){
                    y = 0;
                    height += y;
                }
                if(m<-2 || m>2)
                    throw FileException();
                kin.addKey(s, x, y, width, height, (Mode)m);
                if(file.eof())
                    topic = "ended";
                else
                    file >> topic;
            }
        }

    }
}
