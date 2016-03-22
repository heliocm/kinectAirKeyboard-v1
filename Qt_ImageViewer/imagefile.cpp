#include "imagefile.h"
ImageFile::ImageFile(string name, ImageFile* before)
{
    this->name = name;
    this->before = before;
}
ImageFile* ImageFile::getBefore(){
    return before;
}

string ImageFile::getName(bool suffix){
    if(suffix)
        return name;
    return name.substr(0,name.find_last_of('.'));
}

string ImageFile::getBin(){
    if(!before)
        return name.substr(0,name.find_last_of('.')) + ".b/";
    return before->getBin() + name.substr(0,name.find_last_of('.')) + ".b/";
}
