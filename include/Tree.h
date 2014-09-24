#ifndef TREE_H
#define TREE_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "global.h"

#include <irrlicht.h>

#include "../CTreeGenerator.h"
#include "../CBillboardGroupSceneNode.h"
#include "../CTreeSceneNode.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

class Tree
{
public:
    Tree(int noTrees, int area,IrrlichtDevice *device,IVideoDriver *driver,ITerrainSceneNode* terrain,ITerrainSceneNode* terrain2,ISceneManager* smgr);
    virtual ~Tree();
protected:
private:
};

#endif // TREE_H
