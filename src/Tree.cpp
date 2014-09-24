#include "Tree.h"

bool lightsEnabled = true;

CTreeGenerator* generator = 0;
CTreeSceneNode* tree[1500];

video::ITexture* billTexture = 0;
video::ITexture* leafTexture = 0;

struct STreeDesignFiles
{
    const c8* DesignFile;
    const c8* TreeTextureFile;
    const c8* LeafTextureFile;
    const c8* BillTextureFile;
};

struct STreeDesign
{
    CTreeGenerator* Generator;
    video::ITexture* TreeTexture;
    video::ITexture* LeafTexture;
    video::ITexture* BillTexture;
};
const s32 NUM_TREE_DESIGNS = 4;
const STreeDesignFiles treeDesignFiles[NUM_TREE_DESIGNS] =
{
    { "GameData/trees/Oak.xml", "GameData/textures/OakBark.png", "GameData/textures/OakLeaf.png", "GameData/textures/OakBillboard.png" },
    { "GameData/trees/Aspen.xml", "GameData/textures/AspenBark.png", "GameData/textures/AspenLeaf.png", "GameData/textures/AspenBillboard.png" },
    { "GameData/trees/Pine.xml", "GameData/textures/PineBark.png", "GameData/textures/PineLeaf.png", "GameData/textures/PineBillboard.png" },
    { "GameData/trees/Willow.xml", "GameData/textures/WillowBark.png", "GameData/textures/WillowLeaf.png", "GameData/textures/WillowBillboard.png" }
};

STreeDesign treeDesigns [NUM_TREE_DESIGNS];
s32 currentTreeDesign = 0;
core::vector3df lightDir;
s32 seed = 0;
video::E_MATERIAL_TYPE leafMaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

void generateNewTree(int t)
{
    tree[t]->setup( treeDesigns[currentTreeDesign].Generator, seed, treeDesigns[currentTreeDesign].BillTexture );

    tree[t]->getLeafNode()->getMaterial(0).TextureLayer[0].AnisotropicFilter = true;
    tree[t]->getLeafNode()->getMaterial(0).TextureLayer[0].BilinearFilter = false;

    tree[t]->getLeafNode()->setMaterialTexture( 0, treeDesigns[currentTreeDesign].LeafTexture );
    tree[t]->getLeafNode()->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );

    tree[t]->setMaterialTexture( 0, treeDesigns[currentTreeDesign].TreeTexture );

    if ( lightsEnabled )
        tree[t]->getLeafNode()->applyVertexShadows( lightDir, 1.0f, 0.25f );

    tree[t]->getLeafNode()->setMaterialType( leafMaterialType );
//tree->setPosition(core::vector3df(50.0f, 430.0f, 50.0f));
}

Tree::Tree(int noTrees, int area,IrrlichtDevice *device,IVideoDriver *driver,ITerrainSceneNode* terrain,ITerrainSceneNode* terrain2,ISceneManager* smgr)
{
    srand( (u32)time(0) ); // This is to generate random seeds.

    // Load tree designs
    for ( s32 i=0; i<NUM_TREE_DESIGNS; i++ )
    {
        treeDesigns[i].Generator = new CTreeGenerator( smgr );

        io::IXMLReader* xml = device->getFileSystem()->createXMLReader( treeDesignFiles[i].DesignFile );
        if (xml)
        {
            treeDesigns[i].Generator->loadFromXML( xml );
            xml->drop();
        }

        treeDesigns[i].TreeTexture = driver->getTexture( treeDesignFiles[i].TreeTextureFile );
        treeDesigns[i].LeafTexture = driver->getTexture( treeDesignFiles[i].LeafTextureFile );
        treeDesigns[i].BillTexture = driver->getTexture( treeDesignFiles[i].BillTextureFile );
    }

    // Load leaf shader
    leafMaterialType = (video::E_MATERIAL_TYPE) driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
                           "GameData/shaders/leaves.vert",
                           "main",
                           EVST_VS_2_0,
                           "GameData/shaders/leaves.frag",
                           "main",
                           EPST_PS_2_0,
                           0,
                           EMT_TRANSPARENT_ALPHA_CHANNEL,
                           0);
    // Tree scene node
    for (int i=0; i<noTrees; i++)   //300
    {

        int x = rand() % area;
        int z = rand() % area;

        int terrianTreeY = terrain->getHeight(x,z);

        if(terrianTreeY > getWaterHeight() && terrianTreeY > 0)
        {
            currentTreeDesign =  rand() % 4;
            tree[i] = new CTreeSceneNode( smgr->getRootSceneNode(), smgr );
            tree[i]->setMaterialFlag( video::EMF_LIGHTING, lightsEnabled );
            generateNewTree(i);
            tree[i]->setPosition(vector3df(x,terrianTreeY,z));
            tree[i]->setScale(vector3df(.8f,.8f,.8f));
            tree[i]->drop();
        }
    }


    // Light
    scene::ILightSceneNode* light = smgr->addLightSceneNode( 0, core::vector3df(100,100,100), video::SColorf(1,1,1,1), 10000.0f );
    light->getLightData().AmbientColor.set( 0.25f, 0.25f, 0.25f, 0.25f );
    lightDir = core::vector3df(-1,-1,-1);
    lightDir.normalize();



}

Tree::~Tree()
{
    //dtor
}
