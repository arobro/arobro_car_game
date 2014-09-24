#ifndef CAR_H
#define CAR_H
#include <ctime>
#include <ik_ISound.h>

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
#include <irrklang.h>
#pragma comment(lib, "irrklang.lib")
using namespace irrklang;
class Car
{
public:
    Car(irr::video::IVideoDriver *driver,IrrlichtDevice *device,ISceneNode* node,ITerrainSceneNode* terrain,ITerrainSceneNode* terrain2,ISound* music,int waterHeight);
    virtual ~Car();
    void accerate();
    void stopAccerating();
    double getSpeedo();
    ITexture* getHUDImage();
    double getSpeedoSpeed();
    unsigned int getCarSpeed();
    void keyPressUp();
    void turn (int amount);
    void moveFoward(int speedOveride);
    bool isCarUnderwater();
    void updateSlope();


protected:
private:
    IrrlichtDevice *device;
    ITerrainSceneNode* terrain;
    ITerrainSceneNode* terrain2;
    ISound* music;
    int waterHeight;

    bool carUnderwater = false;


    ITexture* hudImage[10];
    bool backwards = false;
    bool keyUp = true;
    clock_t startTime;
    clock_t gearChangeTime;
    clock_t accerateKeyPressTime;
    int gearNo;

    int currentSpeedoSpeed = 0;
    int maxSpeedoSpeed = 160;

    int currentNumberEngine = -100;
    int minNumberEngine = -140;
    int maxNumberEngine = 25;
    ISceneNode* carNode;

    int extraTime = 0;
    double extraTime2 = 0;
    unsigned int currentCarSpeed = 0;

};

#endif // CAR_H
