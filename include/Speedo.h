#ifndef SPEEDO_H
#define SPEEDO_H

#include <car.h>
#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

class Speedo
{
public:
    Speedo(IVideoDriver *driver,ISceneNode* node,int screenWidth,int screenHeight,Car* car);//
    virtual ~Speedo();
    void render();
protected:
private:
    ITexture* gauge1;
    ITexture* gauge2;
    ITexture* mapPreview;

    IVideoDriver *driver;
    ISceneNode* carNode;
    int screenWidth;
    int screenHeight;
    Car* car;
};

#endif // SPEEDO_H
