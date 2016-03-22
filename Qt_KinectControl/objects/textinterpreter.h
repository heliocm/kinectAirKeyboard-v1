#ifndef TEXTINTERPRETER_H
#define TEXTINTERPRETER_H
#include "objects/kinect.h"
#include <fstream>
#include <string>
using namespace std;
class FileException{};

class TextInterpreter
{
public:
    void getFromText(int & dim, int & dist, bool & invert, bool & buttons, int & preview, Kinect & kin, int & xR, int & yR, int & widthR, int & heightR);
};

#endif // TEXTINTERPRETER_H
