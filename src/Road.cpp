#include "Road.h"

void loadRoad2( int roadFileMin,int roadFileMax,ISceneManager* smgr,ISceneNode* road[],ITexture* texture ,std::unordered_map<int,Point> roadFileMap)
{

    for(int i1=roadFileMin; i1<roadFileMax; i1++)
    {
        if(road[i1]==0)
        {
            Point roadPoint = roadFileMap[i1];
            road[i1] = smgr->addCubeSceneNode();
            road[i1]->setScale(vector3df(4.0f, 0, 1.0f));
            road[i1]->setMaterialTexture(0, texture);
            road[i1]->setPosition(core::vector3df(roadPoint.x,roadPoint.y+1,roadPoint.z));
            road[i1]->setRotation(vector3df(0,roadPoint.r+90,0));
            // cout << x3 << "_" << y3 << "_" << z3 << "_" << r3 << "_" << endl;
        }
    }
}
void removeRoad2(int smallestNumber,int largestNumber,ISceneNode* road[])
{
    //cout << "drop " << largestNumber << endl;

    for(int i3=0; i3<largestNumber; i3++)
    {
        if(road[i3]!=NULL)
        {
            road[i3]->remove();
            road[i3]=NULL;
            //cout << "remove  " << i3 << endl;
        }
    }
}
void loadNextRoadSegments2(int roadFileMax,ISceneNode* node,ISceneManager* smgr,ISceneNode* road[],ITexture* texture ,std::unordered_map<int,Point> roadFileMap)
{

    static int loopCounter = 0;
    loopCounter++;
    if(loopCounter>9)
    {
        loopCounter=0;
        int smallestNumber = 0;

        for(int i2=0; i2<roadFileMax; i2++)
        {
            Point roadPoint = roadFileMap[i2];

            core::vector3df carVec = node->getPosition();
            core::vector3df roadVecTxtFile = vector3df(roadPoint.x,roadPoint.y,roadPoint.z);

            if(roadVecTxtFile.X < carVec.X+100 && roadVecTxtFile.Z < carVec.Z +100)
            {
                smallestNumber = i2;
                //cout << carVec.X << "_" << carVec.Y << "_" << carVec.Z  << endl;
            }
        }
        //cout << "smallest " << smallestNumber << endl;

        int nextSegmentNumberEnd = smallestNumber + 500;
        if(nextSegmentNumberEnd>roadFileMax)
        {
            nextSegmentNumberEnd = roadFileMax;
        }



        loadRoad2(smallestNumber-100,nextSegmentNumberEnd,smgr,road,texture,roadFileMap);
        removeRoad2(0,smallestNumber-1000,road);
    }
}

Road::Road(IVideoDriver *driver,ISceneManager* smgr,ISceneNode* carNode)
{
    this->carNode = carNode;
    this->smgr = smgr;

    texture = driver->getTexture("GameData/road/asphalt_09.jpg");

    std::ifstream infile("GameData/road_read.txt");
    std::string line;

    int ii = 0;
    while (std::getline(infile, line))
    {
        std::stringstream split(line);
        std::string s;
        int j = 0;
        struct Point roadPoint;
        while (getline(split, s, ','))
        {
            j++;
            if(j==1)
            {
                roadPoint.x = atof(s.c_str());
            }
            else if(j==2)
            {
                roadPoint.y = atof(s.c_str());
            }
            else if(j==3)
            {
                roadPoint.z = atof(s.c_str());
            }
            else if(j==4)
            {
                roadPoint.r = atof(s.c_str());
            }

        }
        roadFileMap[ii] = roadPoint;
        road[ii]=NULL;
        ii++;
    }
    roadFileMax = ii-1;
    loadRoad2(0,525,smgr,road,texture,roadFileMap);

    roadfile.open("GameData/road.txt");
}

ISceneNode** Road::getRoad()
{
    return road;
}
std::unordered_map<int,Point> Road::getRoadFileMap()
{
    return roadFileMap;
}
int Road::getRoadFileMax()
{
    return roadFileMax;
}

void Road::update()
{
    loadNextRoadSegments2(roadFileMax,carNode,smgr,getRoad(),texture,getRoadFileMap());
}

void Road::createNewRoad()
{
    vector3df tvec = carNode->getPosition();
    static int i;
    i++;

    road[i] = smgr->addCubeSceneNode();
    road[i]->setScale(vector3df(2.0f, 0, 3.f));
    road[i]->setMaterialTexture(0, texture);
    road[i]->setPosition(vector3df(tvec.X,tvec.Y-1,tvec.Z));
    road[i]->setRotation(vector3df(carNode->getRotation().X,carNode->getRotation().Y,carNode->getRotation().Z));


    roadfile << tvec.X << "," << tvec.Y-1 << "," << tvec.Z << "," << carNode->getRotation().Y+90 << "\n";
}


Road::~Road()
{
    roadfile.close();
}
