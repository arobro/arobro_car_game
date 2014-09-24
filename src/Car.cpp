#include "Car.h"
#include <iostream>
#include <cstdio>
#include <ctime>




Car::Car(irr::video::IVideoDriver *driver,IrrlichtDevice *device,ISceneNode* node,ITerrainSceneNode* terrain,ITerrainSceneNode* terrain2,ISound* music,int waterHeight)
{
    this->device = device;
    this->terrain = terrain;;
    this->terrain2 = terrain2;
    this->music = music;
    this->waterHeight = waterHeight;


    carNode = node;
    startTime = clock();
    gearNo = 0;
    gearChangeTime = clock();
    hudImage[0] = driver->getTexture("GameData/hud/hud0.png");
    hudImage[1] = driver->getTexture("GameData/hud/hud1.png");
    hudImage[2] = driver->getTexture("GameData/hud/hud2.png");
    hudImage[3] = driver->getTexture("GameData/hud/hud3.png");
    hudImage[4] = driver->getTexture("GameData/hud/hud4.png");
    hudImage[5] = driver->getTexture("GameData/hud/hud5.png");
    hudImage[6] = driver->getTexture("GameData/hud/hud6.png");
}

ITexture* Car::getHUDImage()
{
    return hudImage[gearNo];
}
bool Car::isCarUnderwater()
{
    return carUnderwater;
}

void Car::moveFoward(int speedOveride)
{
    //move foward
    int rotation = carNode->getRotation().Y;

    int speed = speedOveride;
    if(speedOveride == 0)
        speed = getCarSpeed();


    float X2 = (float)sin(3.14159265 * rotation / 180.f) * speed;
    float Z2 = (float)cos(3.14159265 * rotation / 180.f) * speed;
    carNode->setPosition( carNode->getPosition()+vector3df(X2,0,Z2));

    //put the car on the terrian height
    core::vector3df tvec =  carNode->getPosition();
    int terrian1Y = terrain->getHeight(tvec.X,tvec.Z)+2;
    int terrian2Y = terrain2->getHeight(tvec.X,tvec.Z)+2;

    if (terrian1Y < terrian2Y) //on the road
    {
        device->setWindowCaption(L"on the road");
        carNode->setPosition(vector3df(tvec.X,terrian2Y,tvec.Z));
    }
    else
    {
        device->setWindowCaption(L"off the road");
        carNode->setPosition(vector3df(tvec.X,terrian1Y,tvec.Z));
    }

    music->setPosition(vec3df(tvec.X,0,tvec.Z));//music

    if (terrian1Y < waterHeight)
    {
        carUnderwater = true;
    }
    else
    {
        carUnderwater = false;
    }
}


void Car::turn(int amount)
{
    carNode->setRotation(carNode->getRotation()+vector3df(0,amount,0));

    if(carNode->getRotation().Y>=360)
    {
        carNode->setRotation(carNode->getRotation()+vector3df(0,-360,0));
    }
    if(carNode->getRotation().Y<=-360)
    {
        carNode->setRotation(carNode->getRotation()+vector3df(0,+360,0));
    }
    // cout << node->getRotation().Y << endl;
}

void Car::accerate()
{
    backwards = false;
    keyUp = false;
    accerateKeyPressTime = clock();
}

void Car::keyPressUp()
{
    keyUp = true;
}

void Car::stopAccerating()
{
    backwards = true;//backwards
    gearNo=0;
}

unsigned int Car::getCarSpeed()
{
    int maxSpeed = 6;
    currentCarSpeed = (maxSpeed/6)*gearNo;

    if(std::difftime(clock(), gearChangeTime)>=530 && std::difftime(clock(), accerateKeyPressTime)<=130 && gearNo<6)
    {
        extraTime2 = extraTime2 + 1;
    }

    if(std::difftime(clock(), gearChangeTime)<530 && std::difftime(clock(), accerateKeyPressTime)>130 && gearNo>1)
    {
        extraTime2 = extraTime2 - 1;
    }

    currentCarSpeed = currentCarSpeed+2 + extraTime2;


    return currentCarSpeed;
}

double Car::getSpeedoSpeed()
{
    int maxSpeed = -280;
    currentSpeedoSpeed = (maxSpeed/6)*gearNo;

    if(std::difftime(clock(), gearChangeTime)>=130 && std::difftime(clock(), accerateKeyPressTime)<=130 && gearNo<6)
    {
        extraTime = extraTime + 1;
    }

    if(std::difftime(clock(), gearChangeTime)<130 && std::difftime(clock(), accerateKeyPressTime)>130 && gearNo>0)
    {
        extraTime = extraTime - 5;
    }

    return currentSpeedoSpeed+90-extraTime;
}

double Car::getSpeedo()
{

    if(std::difftime(clock(), accerateKeyPressTime)>130 && keyUp)
    {
        backwards = true;//backwards

    }

    //change gear after 5 seconds
    if(std::difftime(clock(), gearChangeTime)>1000)
    {
        if(backwards == false && gearNo < 6)
        {
            gearNo++;//change gear up
            gearChangeTime = clock();//reset the clock
            extraTime = 0;
            extraTime2 = 0;
            currentNumberEngine = -100;
        }

    }

    //change gear after 5 seconds
    if(std::difftime(clock(), gearChangeTime)>200)
    {

        if(backwards && gearNo > 0)
        {
            gearNo--;//change gear down
            gearChangeTime = clock();//reset the clock
            extraTime = 0;
            extraTime2 = 0;
        }
    }

    if(std::difftime(clock(), startTime)>10)
    {
        startTime = clock();
        if(backwards && currentNumberEngine<=maxNumberEngine)
            currentNumberEngine=currentNumberEngine+10;
        else if (backwards==false && currentNumberEngine>=minNumberEngine)
            currentNumberEngine=currentNumberEngine-10;
    }

    return currentNumberEngine;
}


void Car::updateSlope()
{
    //front
    core::vector3df tvecRotation = carNode->getRotation();
    float carLenght = 4;
    float X12 = (float)sin(3.14159265 * tvecRotation.Y / 180.f) * carLenght;
    float Z12 = (float)cos(3.14159265 * tvecRotation.Y / 180.f) * carLenght;

    //car slope
    core::vector3df tvec = carNode->getPosition();
    float terrianHeightCarFront = terrain->getHeight(tvec.X,tvec.Z);
    float terrianHeightCarBack = terrain->getHeight(tvec.X + X12,tvec.Z + Z12);
    float carSlopeHeight = terrianHeightCarBack - terrianHeightCarFront;

    //front // cout << carSlopeHeight << endl;
    if (carSlopeHeight!=0)
    {
        float Opposite_Adjacent = carSlopeHeight / carLenght;
        // cout << "Opposite_Adjacent_" << Opposite_Adjacent << endl;
        float carSlope = (180/3.14159) * atan(Opposite_Adjacent) ;//tan-1

        carNode->setRotation(vector3df(carSlope*-1,tvecRotation.Y,tvecRotation.Z));
    }

    //SMaterial m;
    //driver->setMaterial(m);
    //cube1->setPosition(vector3df(tvec.X,terrianHeightCarFront+20,tvec.Z));
    //cube2->setPosition(vector3df(tvec.X + X12,terrianHeightCarFront+20,tvec.Z + Z12));
}

Car::~Car()
{
    //dtor
}
