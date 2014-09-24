#include "speedo.h"
#include "global.h"

Speedo::Speedo(IVideoDriver *driver,ISceneNode* node,int screenWidth,int screenHeight,Car* ptrCar1)//Car* ptrCar1
{
    gauge1  = driver->getTexture("GameData/hud/palca.png");
    gauge2  = driver->getTexture("GameData/hud/palca_e.png");
    mapPreview = driver->getTexture("GameData/mapPreview.png");

    this->driver = driver;
    carNode = node;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->car = ptrCar1;
}

void Speedo::render()
{
    driver->draw2DImage(car->getHUDImage(),core::rect<s32>(screenWidth-getHudSize()-20,screenHeight-getHudSize()-20,screenWidth-20,screenHeight-20),core::rect<s32>(0,0,512,512),0,0,true);

    draw2DImage2(driver, gauge1, core::rect<s32>(0,0,512,512), core::position2d<s32>(screenWidth-getHudSize()-20,screenHeight-getHudSize()-20),
                 core::position2d<s32>(screenWidth-(getHudSize()/2)-20,screenHeight-(getHudSize()/2)-20), irr::f32(car->getSpeedoSpeed()), irr::core::vector2df(0.42f,0.42f), true, irr::video::SColor(128,255,255,255));

    draw2DImage2(driver, gauge2, core::rect<s32>(0,0,512,512), core::position2d<s32>(screenWidth-(getHudSize()/1.5)-20,screenHeight-(getHudSize()/3)-20),
                 core::position2d<s32>(screenWidth-(getHudSize()/2)-20,screenHeight-(getHudSize()/5)-20),
                 irr::f32(car->getSpeedo()), irr::core::vector2df(0.12f,0.12f), true, irr::video::SColor(128,255,255,255));
}

Speedo::~Speedo()
{

}
