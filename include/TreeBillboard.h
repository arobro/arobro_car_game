#ifndef TREEBILLBOARD_H
#define TREEBILLBOARD_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "global.h"

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

class TreeBillboard
{
public:
    TreeBillboard(int noTrees, int area, IVideoDriver *driver,ITerrainSceneNode* terrain,ITerrainSceneNode* terrain2,ISceneManager* smgr);
    virtual ~TreeBillboard();
protected:
private:
};

#endif // TREEBILLBOARD_H
