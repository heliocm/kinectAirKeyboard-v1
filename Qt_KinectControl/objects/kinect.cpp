#include "kinect.h"
Kinect::~Kinect(){
    for(int i = 0;i<m;i++)
        delete[] board[i];
    delete[] board;
    while(!keys.empty()){
        delete keys.front();
        keys.pop_front();
    }
}

Kinect::Kinect(int m, int n){
    display = XOpenDisplay(NULL);
    this->m = m;
    this->n = n;
    board = new KKey**[m];
    for(int i = 0;i<m;i++){
        board[i] = new KKey*[n];
        for(int j = 0;j<n;j++)
            board[i][j] = 0;
    }
}
int Kinect::percentToPixel(int num,int scale){
    return num*scale/100;
}
deque<KKey*>  Kinect::getKeys(){
    return keys;
}

void Kinect::addKey(keyType key, int x, int y, int width, int height, Mode mode){
    x = percentToPixel(x,m);
    width = percentToPixel(width,m);
    y = percentToPixel(y,n);
    height = percentToPixel(height,n);
    KKey* p = new KKey(key, x, y, width, height, mode);
    keys.push_front(p);
    for(int i = x;i<m && i<x + width;i++)
        for(int j = y;j<n && j<y + height;j++)
            board[i][j] = p;
}
void Kinect::checkKeys(int** pressed){
    bool usedBlocks[m][n];
    for(int i = 0;i<m;i++)
        for(int j = 0;j<n;j++){
            usedBlocks[i][j] = false;
        }
    int x;
    int y;
    int width;
    int height;

    for(int i = 0;i<m;i++)
        for(int j = 0;j<n;j++){
            if(!usedBlocks[i][j] && board[i][j] && pressed[i][j]*board[i][j]->getMode() > 0){
                board[i][j]->getLocation(x,y,width,height);
                for(int k = x;k<m && k<x + width;k++)
                    for(int l = y;l<n && l<y + height;l++)
                        usedBlocks[k][l] = true;
                board[i][j]->pressKey(this->display);

            }
        }
    for(deque<KKey*>::iterator it = keys.begin();it!=keys.end();it++){
        (*it)->getLocation(x,y,width,height);
        if(!usedBlocks[x][y])
            (*it)->unBump();
    }
}
