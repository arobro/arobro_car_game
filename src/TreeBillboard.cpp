#include "TreeBillboard.h"

TreeBillboard::TreeBillboard(int noTrees, int area,IVideoDriver *driver,ITerrainSceneNode* terrain,ITerrainSceneNode* terrain2,ISceneManager* smgr)
{
    //trees
    srand( (u32)time(0) );
    ISceneNode* nodeTree[20000];
    ITexture* treeTexture[10];
    treeTexture[0] = driver->getTexture("GameData/trees/tree1HD.png");
    treeTexture[1] = driver->getTexture("GameData/trees/Tree02.png");
    treeTexture[2] = driver->getTexture("GameData/trees/cactus.png");
    treeTexture[3] = driver->getTexture("GameData/trees/grass.png");
    treeTexture[4] = driver->getTexture("GameData/trees/Tree03.png");
    treeTexture[5] = driver->getTexture("GameData/trees/Tree04.png");
    treeTexture[6] = driver->getTexture("GameData/trees/Tree05.png");
    treeTexture[7] = driver->getTexture("GameData/trees/Tree06.png");
    treeTexture[8] = driver->getTexture("GameData/trees/Tree07.png");
    treeTexture[9] = driver->getTexture("GameData/trees/Tree08.png");

    for (int i=0; i<noTrees; i++)  //2000
    {
        int treeSize = rand() % 100 + 30;
        int treeX = rand() % area;
        int treeZ = rand() % area;

        int treeY1 = terrain->getHeight(treeX,treeZ);
        int treeY = treeY1+(treeSize/2);
        int treeY2 = terrain2->getHeight(treeX,treeZ);

        if(treeY1 > getWaterHeight() && treeY > 0 && treeY2 < getWaterHeight())
        {
            nodeTree[i] = smgr->addBillboardSceneNode(nodeTree[i], dimension2d<f32>(treeSize, treeSize));
            nodeTree[i]->setMaterialFlag(video::EMF_LIGHTING,false);
            nodeTree[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            nodeTree[i]->setMaterialTexture(0, treeTexture[rand() %  (sizeof(treeTexture)/sizeof(*treeTexture))] );
            nodeTree[i]->setPosition(vector3df(treeX,treeY,treeZ));
        }
    }
}

TreeBillboard::~TreeBillboard()
{
    //dtor
}
