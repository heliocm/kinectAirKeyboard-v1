#ifndef IMAGEFILE_H
#define IMAGEFILE_H
#include <string>
using namespace std;
class ImageFile
{
    string name;
    ImageFile* before;
public:
    ImageFile(string name, ImageFile* before);
    string getName(bool suffix);
    string getBin();
    ImageFile* getBefore();
};

#endif // IMAGEFILE_H
