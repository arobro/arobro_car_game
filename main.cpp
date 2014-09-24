#include "global.h"
#include "speedo.h"
#include "Road.h"
#include "TreeBillboard.h"
#include "Tree.h"

#include "RealisticWater.h"
//#include <irrlicht.h>

#include "CloudSceneNode.h"
#include "Car.h"
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include <irrklang.h>
using namespace irrklang;

scene::ICameraSceneNode* camera;
scene::IMeshSceneNode* myCube;
core::vector3df myPoint1;
core::vector3df myPoint2;

scene::CCloudSceneNode* cloudLayer1;
scene::CCloudSceneNode* cloudLayer2;
scene::CCloudSceneNode* cloudLayer3;
//User input
bool keys[KEY_KEY_CODES_COUNT];

class MyEventReceiver : public IEventReceiver
{
public :
    virtual bool OnEvent(const SEvent& event)
    {
        if(event.EventType == EET_KEY_INPUT_EVENT)
        {
            keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
            return false;
        }
        return false;
    }
};
//Globals
MyEventReceiver rv;

video::IVideoDriver* driver = 0;
scene::ITerrainSceneNode* terrain = 0;

int main()
{
    //sounds
    ISoundEngine* engine = createIrrKlangDevice();
    ISound* music = engine->play3D("GameData/sounds/test.ogg",vec3df(0,0,0), true, false, true);
    music->setMinDistance(2000.0f);
    engine->setListenerPosition(vec3df(2350.0f,0,2350.0f), vec3df(4350.0f,0,4350.0f));

    int screenWidth = 1920;
    int screenHeight = 1080;

    IrrlichtDevice *device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(screenWidth,  screenHeight), 32, false, false, false, 0);

    device->setEventReceiver(&rv);

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    smgr->setAmbientLight(video::SColorf(255,255,255,1.0f));

    // create skybox and skydome
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
                                  driver->getTexture("GameData/skybox/irrlicht2_up.jpg"),
                                  driver->getTexture("GameData/skybox/irrlicht2_dn.jpg"),
                                  driver->getTexture("GameData/skybox/irrlicht2_lf.jpg"),
                                  driver->getTexture("GameData/skybox/irrlicht2_rt.jpg"),
                                  driver->getTexture("GameData/skybox/irrlicht2_ft.jpg"),
                                  driver->getTexture("GameData/skybox/irrlicht2_bk.jpg"));
    scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("GameData/skybox/skydome.jpg"),16,8,0.95f,2.0f);
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    RealisticWaterSceneNode * water = new RealisticWaterSceneNode(smgr,72000,72000,"GameData/water",dimension2du(512,512),smgr->getRootSceneNode(),1000);
    water->setPosition(core::vector3df(-0.0f,getWaterHeight(),-0.0f));

    //clouds
    // add 1st cloud layer
    cloudLayer1 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
    cloudLayer1->setTranslation(core::vector2d<f32>(0.008f, 0.0f));
    cloudLayer1->getMaterial(0).setTexture(0, driver->getTexture("GameData/clouds/cloud01.png"));
    cloudLayer1->setCloudHeight(0.5f, 0.1f, -0.05f);
    // add 2nd cloud layer
    cloudLayer2 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
    cloudLayer2->setTranslation(core::vector2d<f32>(0.006f, 0.003f));
    cloudLayer2->getMaterial(0).setTexture(0, driver->getTexture("GameData/clouds/cloud02.png"));
    cloudLayer2->setCloudHeight(0.4f, 0.05f, -0.1f);
    cloudLayer2->setTextureScale(0.5f);
    // add 3rd cloud layer
    cloudLayer3 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
    cloudLayer3->setTranslation(core::vector2d<f32>(0.006f, 0.003f));
    cloudLayer3->getMaterial(0).setTexture(0, driver->getTexture("GameData/clouds/cloud03.png"));
    cloudLayer3->setCloudHeight(0.35f, 0.0f, -0.15f);
    cloudLayer3->setTextureScale(0.4f);

    //Load Map
    ITexture*  terrianTexture = driver->getTexture("GameData/Grass_tiled.jpg");
    ITexture*  terrianTextureSea = driver->getTexture("GameData/road/asphalt_09.jpg");

    terrain = smgr->addTerrainSceneNode(
                  "GameData/HeightMap.jpg",
                  0,					// parent node
                  -1,					// node id
                  core::vector3df(0.f, 0.f, 0.f),		// position
                  core::vector3df(0.f, 0.f, 0.f),		// rotation
                  core::vector3df(10.f, 1.2f, 10.f),	// scale
                  video::SColor ( 255, 255, 255, 255 ),	// vertexColor
                  5,					// maxLOD
                  scene::ETPS_17,				// patchSize
                  4					// smoothFactor
              );
    terrain->setMaterialFlag(video::EMF_LIGHTING, false);
    terrain->setMaterialTexture(0,terrianTexture);
    terrain->setMaterialTexture(1, driver->getTexture("GameData/terrain/detailmap3.jpg"));
    terrain->setMaterialType(video::EMT_DETAIL_MAP);
    terrain->scaleTexture(1.0f, 20.0f);

    // add terrain scene node
    scene::ITerrainSceneNode* terrain2 = smgr->addTerrainSceneNode(
            "GameData/RoadMap.jpg",
            0,					// parent node
            -1,					// node id
            core::vector3df(0.f, 0.f, 0.f),		// position
            core::vector3df(0.f, 0.f, 0.f),		// rotation
            core::vector3df(10.f, 1.2f, 10.f),	// scale
            video::SColor ( 255, 255, 255, 255 ),	// vertexColor
            5,					// maxLOD
            scene::ETPS_17,				// patchSize
            20				// smoothFactor
                                         );

    terrain2->setMaterialFlag(video::EMF_LIGHTING, false);
    terrain2->setMaterialTexture(0,	driver->getTexture("GameData/terrain/terrain-texture.jpg"));
    terrain2->setMaterialTexture(1, driver->getTexture("GameData/road/asphaltTexture.bmp"));
    terrain2->setMaterialType(video::EMT_DETAIL_MAP);
    terrain2->scaleTexture(1.0f, 20.0f);

//car
    IAnimatedMesh* mesh = smgr->getMesh("GameData/car3/newcar.obj");
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setMD2Animation(scene::EMAT_STAND);
        // node->setMaterialTexture( 0, driver->getTexture("GameData/sydney.bmp") );
        node->setScale(core::vector3df(10.0f,10.0f,10.0f));
        node->setPosition(core::vector3df(3350.0f,440.0f,3350.0f));
    }
    Car car1(driver,device,node,terrain,terrain2,music,getWaterHeight());
    Car *ptrCar1 = &car1;//new car

    //2d speedo
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    guienv->addStaticText(L"welcome! ",rect<s32>(10,10,260,22), true);

    ITexture* mapPreview = driver->getTexture("GameData/mapPreview.png");
    Speedo speedo1(driver,node,screenWidth,screenHeight,ptrCar1);
    TreeBillboard treeBillboard1(4600,25000,driver,terrain,terrain2,smgr);
    Tree tree1(1500,25000,device,driver,terrain,terrain2,smgr);
    Road road1(driver,smgr,node);
    int const roadFileMax = road1.getRoadFileMax();
    ITexture* texture = driver->getTexture("GameData/road/asphalt_09.jpg");

    camera = smgr->addCameraSceneNode();
    camera->setPosition(vector3df(20,480,20));
    camera->bindTargetAndRotation(true); // you can enable this if you need to
    camera->setFarValue(2000.0f);//  camera->setFarValue(7000.0f);

    bool CameraMoveSlow = false;

    irr::core::array<irr::scene::ISceneNode*> nodes;
    smgr->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes,0);

    for(int j=0; j<nodes.size(); j++)
        for(int i=0; i<nodes[j]->getMaterialCount(); i++)
        {
            nodes[j]->getMaterial(i).setFlag(irr::video::EMF_FOG_ENABLE,true);
            nodes[j]->getMaterial(i).setFlag(irr::video::EMF_LIGHTING,true);
        }

    enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};

    ISceneNode *cube1 = smgr->addCubeSceneNode(.7f, 0, -1);
    cube1->setMaterialTexture(0, driver->getTexture("GameData/roadTexture.jpg"));

    ISceneNode *cube2 = smgr->addCubeSceneNode(.7f, 0, -1);
    cube2->setMaterialTexture(0, driver->getTexture("GameData/roadTexture.jpg"));

    while(device->run())
    {
        //camera angle
        car1.updateSlope();
        camera->setTarget(node->getAbsolutePosition()+vector3df(0,7,0));

        int distance = camera->getTarget().getDistanceFrom(camera->getAbsolutePosition());
        if (distance > 120 || CameraMoveSlow)
        {
            CameraMoveSlow = true;

            f32 speed = .2f;
            vector3df mPos = camera->getAbsolutePosition();
            vector3df mView = camera->getTarget();
            vector3df vVector = mView - mPos; // Get the view vector

            mPos.X += vVector.X * speed;
            mPos.Y += (vVector.Y * speed)+ 1;
            mPos.Z += vVector.Z * speed;

            camera->setPosition(mPos);
        }
        if( distance < 10)
        {
            CameraMoveSlow = false;
        }
        //Move
        if(keys[KEY_KEY_D])
        {
            car1.turn(5);
        }
        if(keys[KEY_KEY_A])
        {
            car1.turn(-5);
        }
        if(keys[KEY_KEY_N])
        {
            car1.stopAccerating();
        }
        if(keys[KEY_KEY_W]  == UP)
        {
            car1.keyPressUp();
        }
        if(keys[KEY_KEY_W]  == DOWN)
        {
            car1.accerate();
        }
        if(keys[KEY_KEY_W])
        {
            car1.moveFoward(0);
        }
        if(keys[KEY_KEY_R])
        {
            car1.moveFoward(1);
            road1.createNewRoad();
        }
        if(keys[KEY_KEY_Q])
        {
            return 0;
        }

        road1.update();
        //if udnerwater
        if (car1.isCarUnderwater())
        {
            //fog underwater /
            driver->setFog(video::SColor(0,72,209,204), EFT_FOG_LINEAR, 0, 500, 0.5, true, false);
            smgr->setAmbientLight(video::SColorf(1.8,1.8,1.8,1));
            terrain->setMaterialTexture(0,terrianTextureSea);
        }
        else  //should only run this code when change terrian/water state, not every time to save cpu
        {
            //disable fog
            smgr->setAmbientLight(video::SColorf(255,255,255,1.0f));
            driver->setFog(video::SColor(28,128,128,128), EFT_FOG_LINEAR, 350000, 650000, .01f, false, false);
            terrain->setMaterialTexture(0,terrianTexture);
        }
        //night
        //smgr->setAmbientLight(video::SColorf(2.8,2.8,2.8,.5f));
        //driver->setFog(video::SColor(0,0,0,.5f), EFT_FOG_LINEAR, 0, 500, 0.5, true, false);

        camera->updateAbsolutePosition();

        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

        draw2DImage2(driver, mapPreview, core::rect<s32>(0,0,512,512), core::position2d<s32>(screenWidth-getHudSize()+10,20),
                     core::position2d<s32>(screenWidth-(getHudSize()/1.5)+20,getHudSize()/3+20),
                     irr::f32(node->getRotation().Y+180), irr::core::vector2df(0.5f,0.5f), true, irr::video::SColor(128,255,255,255));
        speedo1.render();
        driver->endScene();

        core::stringw str = core::stringw( driver->getFPS() );
        device->setWindowCaption(str.c_str());
    }

    if (cloudLayer1)
        cloudLayer1->drop();
    if (cloudLayer2)
        cloudLayer2->drop();
    if (cloudLayer3)
        cloudLayer3->drop();

    device->drop();

    if (music)
        music->drop(); // release music stream.

    engine->drop(); // delete engine
    return 0;
}
