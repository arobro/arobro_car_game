#ifndef ROAD_H
#define ROAD_H

#include <irrlicht.h>
#include <unordered_map>

#ifndef GLOBAL_H
#include "global.h"
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>


using namespace std;
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Road
{
public:
    Road(IVideoDriver *driver,ISceneManager* smgr,ISceneNode* carNode);
    ISceneNode** getRoad();
    std::unordered_map<int,Point> getRoadFileMap();
    int getRoadFileMax();
    void update();
    void createNewRoad();
    virtual ~Road();
protected:
private:
    ISceneNode* road [50000];
    std::unordered_map<int,Point> roadFileMap;
    int roadFileMax;
    ISceneNode* carNode;
    ISceneManager* smgr;
    ITexture* texture;
    ofstream roadfile;
};

#endif // ROAD_H
