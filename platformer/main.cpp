
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <iostream>
#include <deque>
#include <array>
#include <vector>
#include <string>
#include <stdio.h>
#include <map>
// SFML
// It has been a long time waiting for a library with the capabilities
// The way it moves and behaves is valiant
// It controls all, though only you are the true controller
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
//#include "script/stages.hpp"


// use this one some time so it dont lag
// sprite.setTextureRect(sf::IntRect(10, 10, 32, 32));


sf::Texture spritesheet;
sf::RenderTexture surface;
sf::RenderTexture movingSurface;
sf::RenderTexture foregroundSurface;
sf::Texture platformTexture;
sf::Music niceMusic;
const int screenWidth = 1920;
const int screenHeight = 1080;
int gridSize = 120;
int timey;

bool titleScreen = true;
bool left;
bool right;
bool up;
bool down;

bool space = false;

bool win = false;
bool blink = false;
int curLevel = 1;
int curWorld = 1;
int prevWorld = 0;

std::string songs[] = {"rick.wav"};

sf::Font font;


sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "platform");



// okei doke so here we have all of the land form thingies, their location and size :thumbsup:
std::map<int,std::array<int,4>> textures = {
    {1,{128,192,32,32}},
    {2,{128,160,32,32}},
    {3,{96,192,32,32}},
    {4,{160,192,32,32}},
    {5,{96,160,32,32}},
    {6,{160,160,32,32}},
    {7,{128,32,32,24}},
    {8,{128,64,32,24}},
    {9,{96,64,32,24}},
    {10,{160,64,32,24}},
    {11,{416,256,32,32}},
    {12,{192,224,32,32}},
    {13,{224,224,32,32}},
    {14,{32,96,32,32}},
    {15,{128,96,32,32}},
    {16,{128,128,32,32}},
    {17,{160,256,32,8}},
    {21,{128,0,32,32}},
    {23,{320,192,32,32}},
    {24,{320,224,32,32}},
    {25,{288,160,32,32}},
    {26,{480,160,32,32}}
};

// This down here is so i dont forger 💀 what the textures are
/*
 0 = empty space
 1 = regular dirt (160,224)
 2 = regular grass (160, 192)
 3 = left edge dirt (128,224)
 4 = right edge dirt (192,224)
 5 = left edge grass (128,192)
 6 = right edge grass (192,192)
 7 = remote island (160,64)
 8 = middle island (160,96)
 9 = left island (128,96)
 10 = right island (192,96
 11 = sign arrow (448,256)
 12 = dirt spike left (192,224)
 13 = dirt spike right (224,224)
 14 = spikes (32,96)
 15 = singular grass (128,96)
 16 = singular dirt (128, 128)
 17 = moving platform (up)
 18 = moving platform (left)
 19 = moving platform (Down)
 20 = moving platform (right)
 21 = chain
 23 = medium water
 24 = deep water
 25 = left of water
 26 = right of water
 */

// I forgor what i did before but this here is the good kusch
// Has all of the level data in the struct
struct level
{
    int state;
    int startPos[3];
    int gridSize;
    std::vector<int> platformRange;
    std::vector<int> mobRange;
    std::vector<std::vector<int>> map;
};

struct level level1 = {1,{200,940},120,{100},{},{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {0,0,0,5,2,2,6,0,0,0,0,5,2,2,2,2},
    {2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,1}
}
};

struct level level2 = {0,{200,940},80,{120,300},{},{
    {0,0,21,0,0,0,0,0,21,0,0,0,0,0,0,0,0,0,21,0,0,0,0,0},
    {0,0,21,0,0,0,0,0,21,0,0,0,0,0,0,0,0,0,21,0,0,0,0,11},
    {0,0,9,8,8,8,8,8,10,0,0,0,0,18,0,0,0,0,9,8,8,8,8,8},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,9,8,10,0,0,18,0,0,0,0,20,0,0,0,0,18,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,13,14,14,14,14},
    {0,0,0,0,0,5,2,2,2,2,6,0,0,5,2,2,2,2,1,1,1,1,1,1},
    {2,2,2,2,2,1,1,1,1,1,13,14,14,12,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}
};

struct level level3 = {0,{20,1000},80,{120,100,50,80},{},{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11},
    {0,0,0,0,0,0,0,0,0,0,0,0,5,6,0,0,0,20,0,0,0,0,5,2},
    {0,0,0,0,0,0,0,0,0,0,0,0,3,4,22,22,22,22,22,22,22,22,26,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,3,1,23,23,23,23,23,23,23,23,26,1},
    {0,0,15,0,0,19,0,0,15,0,19,0,3,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,16,0,0,0,0,0,16,0,0,0,3,1,1,1,4,0,0,0,0,0,3,1},
    {0,0,16,0,0,0,0,0,16,0,0,0,3,1,1,4,0,0,0,0,0,0,0,3},
    {0,17,16,0,0,0,0,0,16,0,0,0,3,1,1,25,22,22,22,22,22,22,22,26},
    {0,0,3,14,14,14,14,14,12,14,14,14,12,1,1,25,23,23,23,23,23,23,23,26},
    {0,0,3,1,1,1,1,1,1,1,1,1,1,1,1,1,25,24,24,24,24,24,26,1},
    {2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}
};


// This down here is so i dont forger 💀 what the textures are
/*
 0 = empty space
 1 = regular dirt (160,224)
 2 = regular grass (160, 192)
 3 = left edge dirt (128,224)
 4 = right edge dirt (192,224)
 5 = left edge grass (128,192)
 6 = right edge grass (192,192)
 7 = remote island (160,64)
 8 = middle island (160,96)
 9 = left island (128,96)
 10 = right island (192,96
 11 = sign arrow (448,256)
 12 = dirt spike left (192,224)
 13 = dirt spike right (224,224)
 14 = spikes (32,96)
 15 = singular grass (128,96)
 16 = singular dirt (128, 128)
 17 = moving platform (up)
 18 = moving platform (left)
 19 = moving platform (Down)
 20 = moving platform (right)
 21 = chain
 23 = medium water
 24 = deep water
 25 = left of water
 26 = right of water
 */

std::map<int,struct level> levels =
{
    {1,level1},
    {2,level2},
    {3,level3}
};


class graphics
{
public:
    sf::Texture spritesheet;
    sf::Texture worldTexture;
    
    graphics()
    {
        if(!spritesheet.loadFromFile(resourcePath()+"Resources/images/stage.png")){
            std::cout << "I brokey ): \n";
            return;
        }
        
        if(!worldTexture.loadFromFile(resourcePath()+"Resources/images/worldSelection.png")){
            std::cout << "I brokey ): \n";
            return;
        }
        
        worldTexture.setSmooth(false);
        
    }
};

graphics graphics;

class buttons
{
public:
    int x;
    int y;
    bool pressed = false;
    int type;
    int whichOne;
    
    sf::Sprite butt;
    
    buttons(int x1,int y1,float scaleFac,int whichOne1)
    {
        x = x1*scaleFac;
        y = y1*scaleFac;
        whichOne = whichOne1;
        type = levels.at(whichOne1).state;
        butt.setTexture(graphics.spritesheet);
        butt.setTextureRect(sf::IntRect(64+(type*16),0,16,11));
        butt.setScale(scaleFac, scaleFac);
        butt.setPosition(x, y);
    }
    
    void buttonPress(bool down)
    {
        if(down)
            butt.setTextureRect(sf::IntRect(64+(type*16),11,16,11));
        else
            butt.setTextureRect(sf::IntRect(64+(type*16),0,16,11));
    }
    
    void buttonUpdate()
    {
        type = levels.at(whichOne).state;
        butt.setTextureRect(sf::IntRect(64+(type*16),0,16,11));
        butt.setPosition(x, y);
    }
    
};

struct world {
    int pos[2];
    float scale;
    sf::IntRect texture;
    sf::IntRect foreground;
    std::vector<std::array<int, 2>> buttons;
    std::vector<std::array<int, 4>> obstacles;
};

struct world world1 = {{0,420},2,sf::IntRect(0,0,128,72),sf::IntRect(0,72,128,72),{{17,41},{52,56},{87,41}},{
    {40,26,41,23},
    {8,27,4,2},
    {24,24,8,2},
    {4,66,8,2},
    {98,24,8,2},
    {108,27,4,2},
    {37,58,4,2},
    {78,58,4,2},
    {111,60,8,2},
    {0,0,128,18}
}
};

std::map<int,struct world> worlds =
{
    {1,world1}

};

class World
{
public:
    std::vector<std::array<int, 2>> buttonLocation;
    std::vector<buttons> button;
    std::vector<std::array<int,4>> obstacles;
    sf::Sprite menu;
    sf::Sprite foreground;
    
    World(world amongus)
    {
        obstacles = amongus.obstacles;
        
        buttonLocation = amongus.buttons;
        
        menu.setTexture(graphics.worldTexture);
        menu.setTextureRect(amongus.texture);
        menu.setScale(screenHeight/72, screenHeight/72);
        menu.setPosition(0, 0);
        
        foreground.setTexture(graphics.worldTexture);
        foreground.setTextureRect(amongus.foreground);
        foreground.setScale(screenHeight/72, screenHeight/72);
        foreground.setPosition(0, 0);
        
        int i = 1;
        for(auto& butt:buttonLocation)
        {
            buttons buttList(butt[0],butt[1],menu.getScale().x,i);
            button.push_back(buttList);
            ++i;
        }
    }
    
    void worldReset(world amongus)
    {
        obstacles = amongus.obstacles;
        buttonLocation = amongus.buttons;
        
        menu.setTextureRect(amongus.texture);
        menu.setScale(screenHeight/72, screenHeight/72);
        menu.setPosition(0, 0);
        
        foreground.setTextureRect(amongus.foreground);
        foreground.setScale(screenHeight/72, screenHeight/72);
        foreground.setPosition(0, 0);
        
        int i = 1;
        for(auto& butt:buttonLocation)
        {
            buttons buttList(butt[0],butt[1],menu.getScale().x,i);
            button.push_back(buttList);
            ++i;
        }
    }
    
};

World world(world1);

class movingPlatform
{
public:
    int origin[2];
    int pos[2];
    int dy = 1;
    int dx = 2;
    int range;
    
    sf::Sprite platform;
    
    movingPlatform(int x,int y,int moveX,int moveY,int range1)
    {
        std::cout << range1 << std::endl;
        if (range1 >= levels.at(curLevel).platformRange.size())
            range = levels.at(curLevel).platformRange.at(0);
        else
            range = levels.at(curLevel).platformRange.at(range1);
        
        pos[0] = x;
        pos[1] = y;
        origin[0] = x;
        origin[1] = y;
        dx *= moveX;
        dy *= moveY;
        platform.setTexture(graphics.spritesheet);
        platform.setTextureRect(sf::IntRect(textures.at(17)[0],textures.at(17)[1],textures.at(17)[2],textures.at(17)[3]));
        platform.setScale((float)gridSize/textures.at(17)[2],
                       (float)gridSize/textures.at(17)[2]);
        platform.setPosition(x, y);
    }
    
    void update()
    {
        if(dy)
        {
            if (pos[1] + dy > origin[1] + range || pos[1] + dy < origin[1] - range)
                dy = -dy;
            pos[1] += dy;
        } else
        {
            if (pos[0] + dx > origin[0] + range || pos[0] + dy < origin[0] - range)
                dx = -dx;
            pos[0] += dx;
        }
        
        platform.setPosition(pos[0], pos[1]);
    }
    
};

class water
{
public:
    int state,x,y;
    sf::Sprite leau;
    
    water(float x1,float y1)
    {
        state = std::rand() % 3 + 0;
        leau.setTexture(graphics.spritesheet);
        leau.setTextureRect(sf::IntRect(320+state*32,160,32,32));
        leau.setColor(sf::Color(255,255,255,200));
        leau.setScale((float)gridSize/32, (float)gridSize/32);
        x = x1;
        y = y1;
        leau.setPosition(x, y);
    }
    
    void update()
    {
        state += 1;
        if (state > 3)
            state = 0;
        
        leau.setTextureRect(sf::IntRect(320+state*32,160,32,32));
        leau.setScale((float)gridSize/32, (float)gridSize/32);
        leau.setPosition(x, y);
    }
};

class Level
{
public:
    std::vector<std::array<int,3>> tile;
    std::vector<movingPlatform> platforms;
    std::vector<water> waters;
    sf::Sprite exit;
    Level()
    {
        exit.setTexture(graphics.spritesheet);
        exit.setTextureRect(sf::IntRect(0,16,11,9));
        exit.setOrigin(exit.getLocalBounds().width, 0);
        exit.setScale(5,5);
        exit.setPosition(screenWidth-5, 5);
        levelReset(levels.at(curLevel).map);
        
    }
    
    void levelReset(std::vector<std::vector<int>> map)
    {
        platforms.clear();
        waters.clear();
        gridSize = levels.at(curLevel).gridSize;
        tile.clear();
        int row = 0;
        int column;
        int platNum = 1;
        for (auto& i:map)
        {
            column = 0;
            for (auto& o:i)
            {
                switch (o){
                    case 0:{
                        break;
                    }
                    case 17:{
                        auto platform = movingPlatform(column*gridSize,row*gridSize,0,1,platNum);
                        platforms.push_back(platform);
                        ++platNum;
                        break;
                    }
                    case 18:{
                        auto platform = movingPlatform(column*gridSize,row*gridSize,1,0,platNum);
                        platforms.push_back(platform);
                        ++platNum;
                        break;
                    }
                    case 19:{
                        auto platform = movingPlatform(column*gridSize,row*gridSize,0,-1,platNum);
                        platforms.push_back(platform);
                        ++platNum;
                        break;
                    }
                    case 20:{
                        auto platform = movingPlatform(column*gridSize,row*gridSize,-1,0,platNum);
                        platforms.push_back(platform);
                        ++platNum;
                        break;
                    }
                        
                    case 22:{
                        auto sipsip = water(column*gridSize,row*gridSize);
                        waters.push_back(sipsip);
                        break;
                    }
                    
                    default:{
                        tile.push_back({column*gridSize,row*gridSize,o});
                        break;
                    }
                        /*
                if (1 <= o && o <=16)
                    tile.push_front({column*gridSize,row*gridSize,o});
                else if (o == 17){
                    auto platform = movingPlatform(column*gridSize,row*gridSize,0,1);
                    platforms.push_front(platform);
                } else if (o == 18){
                    auto platform = movingPlatform(column*gridSize,row*gridSize,1,0);
                    platforms.push_front(platform);
                }
                        */
                }
                ++column;
            }
            ++row;
        }
    }
    
    void draw()
    {
        surface.clear(sf::Color(135, 206, 235));
        foregroundSurface.clear(sf::Color(255,255,255,0));
        sf::Sprite block;
        
       
        for (auto& i:tile)
        {
            
            block.setTexture(graphics.spritesheet,true);
            block.setTextureRect(sf::IntRect(textures.at(i[2])[0],textures.at(i[2])[1],textures.at(i[2])[2],textures.at(i[2])[3]));
            block.setScale((float)gridSize/textures.at(i[2])[2],
                           (float)gridSize/textures.at(i[2])[2]);
            block.setPosition(i[0], i[1]);
            if (i[2] == 23 || i[2] == 24)
                block.setColor(sf::Color(255,255,255,180));
            else
                block.setColor(sf::Color(255,255,255,255));
            surface.draw(block);
            
            
        }
        
        surface.draw(exit);
    }
};
Level theLevel;

void screenReset()
{
    theLevel.draw();
}

class player
{
    public:
    float pos[2];
    float velY = 10;
    float velX = 0;
    sf::Texture blobTex;
    sf::Sprite blob;
    int animationPhase;
    int animationY;
    int animationX;
    float dy = 0;
    int dx = 0;
    bool canMove = true;
    bool jumped = false;
    bool die = false;
    bool touchWater = false;
    bool touchWater1 = false;
    
    player(int x, int y)
    {
        pos[0] = x;
        pos[1] = y;
        
        blob.setTexture(graphics.spritesheet);
        blob.setTextureRect(sf::IntRect(0,0,12,8));
        blob.setScale(gridSize/12*0.7,gridSize/12*0.7);
        blob.setPosition(x, y);
    }
    
    void update()
    {
        dy = 0;
        dx = 0;
        
        // get keys pressed
        if (left && canMove)
            dx -= 7*((float)gridSize/120);
        if (right && canMove)
            dx += 7*((float)gridSize/120);
        
        // collsision
        dy += velY;
       
        velY += (float)gridSize/120;
        if (velY > gridSize/8)
            velY = gridSize/8;
        
        if (pos[0] + dx + blob.getGlobalBounds().width >= screenWidth && canMove){
            dx = 0;
        }else if (pos[0] >= screenWidth && canMove == false){
                
                win = true;
        }else if (pos[0] + dx < 0){
                dx = 0;
        }
        
        jumped = true;
        
        touchWater1 = false;
        for (auto& tile:theLevel.tile)
        {
            // take your time with this one its a doozy
            if (((blob.getPosition().y + blob.getGlobalBounds().height + dy) > tile[1]
                && (blob.getPosition().y + blob.getGlobalBounds().height + dy) < tile[1] + textures.at(tile[2])[3]*gridSize/textures.at(tile[2])[2])
                && blob.getPosition().x+blob.getGlobalBounds().width > tile[0]
                && blob.getPosition().x < tile[0] + gridSize )
            {
                switch (tile[2]) {
                    case 11:
                        canMove = false;
                        break;
                    case 14:
                        die = true;
                        if(niceMusic.getStatus() != sf::SoundSource::Playing){
                            niceMusic.play();
                            niceMusic.setVolume(1000);
                        }
                        break;
                    case 21:
                        break;
                    case 23:
                    case 24:
                        if (!touchWater)
                            velY += (float)gridSize/60;
                        touchWater = true;
                        touchWater1 = true;
                        if (velY > -7)
                            velY -= (float)gridSize/60;
                        break;
                    
                    default:
                        jumped = false;
                        collission(tile[0],tile[1],0);
                        velY = 1;
                        break;
                    
                }
            }
            
            if (blob.getPosition().y + dy < tile[1] + textures.at(tile[2])[3]*gridSize/textures.at(tile[2])[2]
                && blob.getPosition().y + dy > tile[1]
                && blob.getPosition().x+blob.getGlobalBounds().width > tile[0]
                && blob.getPosition().x < tile[0] + gridSize )
            {
                
                switch (tile[2]) {
                    case 11:
                        canMove = false;
                        levels.at(curLevel).state = 2;
                        if (curLevel+1 <= levels.size()){
                            if (levels.at(curLevel+1).state != 2)
                                levels.at(curLevel+1).state = 1;
                        } else
                            std::cout << "No more levels \n";
                        dx = 5*((float)gridSize/120);;
                        break;
                    case 14:
                    case 21:
                    case 23:
                    case 24:
                        break;
                        
                    default:
                        velY = 3;
                        dy = 3;
                        break;
                }
            }
            
            if (blob.getPosition().x + dx < tile[0] + gridSize
                && blob.getPosition().x + dx > tile[0]
                && blob.getPosition().y < tile[1] + gridSize
                && blob.getPosition().y + blob.getGlobalBounds().height > tile[1])
            {
                switch (tile[2]) {
                    case 11:
                        canMove = false;
                        break;
                    case 14:
                    case 21:
                    case 23:
                    case 24:
                        break;
                    default:
                        dx = 0;
                        break;
                }
            }
            
            if (blob.getPosition().x + blob.getGlobalBounds().width + dx > tile[0]
                && blob.getPosition().x + blob.getGlobalBounds().width + dx < tile[0] + gridSize
                && blob.getPosition().y < tile[1] + gridSize
                && blob.getPosition().y + blob.getGlobalBounds().height > tile[1])
            {
                switch (tile[2]) {
                    case 11:
                        canMove = false;
                        break;
                    case 14:
                    case 21:
                    case 23:
                    case 24:
                        break;
                    default:
                        dx = 0;
                        break;
                }
            }
            
        }
        
        for (auto& platform:theLevel.platforms)
        {
            if ((blob.getPosition().y + blob.getGlobalBounds().height + dy > platform.pos[1]
                 && (blob.getPosition().y + blob.getGlobalBounds().height + dy) < platform.pos[1] + platform.dy + textures.at(17)[3]*gridSize/textures.at(17)[2])
                 && blob.getPosition().x+blob.getGlobalBounds().width > platform.pos[0]
                 && blob.getPosition().x < platform.pos[0] + gridSize )
            {
                this->dy += platform.dy;
                jumped = false;
                collission(platform.pos[0],platform.pos[1],platform.dy);
                velY = 1;
                velX = platform.dx;
                
            }
            
            if (blob.getPosition().y + dy < platform.pos[1] + textures.at(17)[3]*gridSize/textures.at(17)[2]
                && blob.getPosition().y + dy > platform.pos[1]
                && blob.getPosition().x+blob.getGlobalBounds().width > platform.pos[0]
                && blob.getPosition().x < platform.pos[0] + gridSize )
            {
                velY = 3;
                dy = 3;
                velX = platform.dx;
            }
            
            if (blob.getPosition().x + dx < platform.pos[0] + gridSize
                && blob.getPosition().x + dx > platform.pos[0]
                && blob.getPosition().y < platform.pos[1] + textures.at(17)[3]*gridSize/textures.at(17)[2]
                && blob.getPosition().y + blob.getGlobalBounds().height > platform.pos[1])
            {
                this->dx += platform.dx;
                this->dy += platform.dy;
            }
            
            if (blob.getPosition().x + blob.getGlobalBounds().width + dx > platform.pos[0]
                && blob.getPosition().x + blob.getGlobalBounds().width + dx < platform.pos[0] + gridSize
                && blob.getPosition().y < platform.pos[1] + textures.at(17)[3]*gridSize/textures.at(17)[2]
                && blob.getPosition().y + blob.getGlobalBounds().height > platform.pos[1])
            {
                this->dx += platform.dx;
                this->dy += platform.dy;
        
            }
        }
        
        for (auto& water:theLevel.waters)
        {
            if (((blob.getPosition().y + blob.getGlobalBounds().height + dy) > water.y
                 && (blob.getPosition().y + blob.getGlobalBounds().height + dy) < water.y + gridSize)
                 && blob.getPosition().x+blob.getGlobalBounds().width > water.x
                && blob.getPosition().x < water.x + gridSize)
            {
                if (!touchWater)
                    velY += (float)gridSize/60;
                touchWater = true;
                touchWater1 = true;
                if (velY > -7)
                    velY -= (float)gridSize/60;
                break;
            }
        }
        
        if (!touchWater1)
            touchWater = false;
        
        if (touchWater && space)
            velY -= (float)gridSize/40;
        
        
        dx += velX;
        if(velX>0)
           velX -= 1;
        else
            velX = 0;
        
        
        
        pos[0] += dx;
        pos[1] += dy;
    
        if (pos[1] + blob.getGlobalBounds().height > screenHeight)
            pos[1] = screenHeight - blob.getGlobalBounds().height;
        blob.setPosition(pos[0], pos[1]);
        
        
    }

    void collission(float x, float y,int pdy)
    {
        dy -= 1;
        if (((blob.getPosition().y + blob.getGlobalBounds().height + dy - pdy) > y
            && (blob.getPosition().y + blob.getGlobalBounds().height + dy - pdy) < y + gridSize)
            && blob.getPosition().x+blob.getGlobalBounds().width > x
            && blob.getPosition().x < x + gridSize )
            collission(x,y,pdy);
    }
    
    void animation()
    {
        if (timey%6 == 0){
            if ((left && !right) || (right && !left))
            animationPhase += 1;
        }
        
        if (left && !right)
            animationY = 0;
        else if ((right && !left) || !canMove)
            animationY = 8;
            
        if (!jumped){
            switch (animationPhase)
            {
                case 1:
                    animationX = 0;
                    break;
                case 2:
                    animationX = 12;
                    break;
                case 3:
                    animationX = 24;
                    animationPhase = 0;
                    break;
                default:
                    animationX = 24;
                    animationPhase = 0;
                    break;
            }
        } else {
            animationX = 36;
        }
        
        blob.setTextureRect(sf::IntRect(animationX, animationY, 12, 8));
        blob.setScale(gridSize/12*0.7,gridSize/12*0.7);
        blob.setPosition(pos[0], pos[1]);
        
    }
    
    void menuUpdate()
    {
        dx = 0;
        dy = 0;
        
        if(left)
            dx -= 5;
        if(right)
            dx += 5;
        if(up)
            dy -= 5;
        if(down)
            dy += 5;
        
        if (pos[0] + dx < 0 || pos[0] + dx + blob.getGlobalBounds().width > screenWidth)
            dx = 0;
        if (pos[1] + dy < 0 || pos[1] + dy + blob.getGlobalBounds().height > screenHeight)
            dy = 0;
        
        for (auto& ob:worlds.at(curWorld).obstacles)
        {
            if(blob.getPosition().x + dx + blob.getGlobalBounds().width > ob[0]*world.menu.getScale().y &&
               blob.getPosition().x + dx < ob[0]*world.menu.getScale().y + ob[2]*world.menu.getScale().y &&
               blob.getPosition().y < ob[1]*world.menu.getScale().y + ob[3]*world.menu.getScale().y &&
               blob.getPosition().y + blob.getGlobalBounds().height > ob[1]*world.menu.getScale().y)
            {
                if(dx>0)
                    menuCollision(1, ob);
                else if(dx<0)
                    menuCollision(3, ob);
            }
            if(blob.getPosition().x + blob.getGlobalBounds().width > ob[0]*world.menu.getScale().y &&
               blob.getPosition().x < ob[0]*world.menu.getScale().y + ob[2]*world.menu.getScale().y &&
               blob.getPosition().y + dy < ob[1]*world.menu.getScale().y + ob[3]*world.menu.getScale().y &&
               blob.getPosition().y + blob.getGlobalBounds().height + dy > ob[1]*world.menu.getScale().y)
            {
                if(dy>0)
                    menuCollision(2, ob);
                else if(dy<0)
                    menuCollision(0, ob);
            }
        }
        
        pos[0] += dx;
        pos[1] += dy;
        
        blob.setPosition(pos[0], pos[1]);
    }
    
    void menuCollision(int direction,std::array<int,4> ob)
    {
        if(direction == 0){
            dy += 1;
            if(blob.getPosition().x + blob.getGlobalBounds().width > ob[0]*world.menu.getScale().y &&
               blob.getPosition().x < ob[0]*world.menu.getScale().y + ob[2]*world.menu.getScale().y &&
               blob.getPosition().y + dy < ob[1]*world.menu.getScale().y + ob[3]*world.menu.getScale().y &&
               blob.getPosition().y + blob.getGlobalBounds().height + dy > ob[1]*world.menu.getScale().y)
                menuCollision(direction, ob);
        }
        else if(direction == 1){
            dx -= 1;
            if(blob.getPosition().x + dx + blob.getGlobalBounds().width > ob[0]*world.menu.getScale().y &&
               blob.getPosition().x + dx < ob[0]*world.menu.getScale().y + ob[2]*world.menu.getScale().y &&
               blob.getPosition().y < ob[1]*world.menu.getScale().y + ob[3]*world.menu.getScale().y &&
               blob.getPosition().y + blob.getGlobalBounds().height > ob[1]*world.menu.getScale().y)
                menuCollision(direction, ob);
        }
        else if(direction == 2){
            dy -= 1;
            if(blob.getPosition().x + blob.getGlobalBounds().width > ob[0]*world.menu.getScale().y &&
               blob.getPosition().x < ob[0]*world.menu.getScale().y + ob[2]*world.menu.getScale().y &&
               blob.getPosition().y + dy < ob[1]*world.menu.getScale().y + ob[3]*world.menu.getScale().y &&
               blob.getPosition().y + blob.getGlobalBounds().height + dy > ob[1]*world.menu.getScale().y)
                menuCollision(direction, ob);
        }
        else if(direction == 3){
            dx += 1;
            if(blob.getPosition().x + dx + blob.getGlobalBounds().width > ob[0]*world.menu.getScale().y &&
               blob.getPosition().x + dx < ob[0]*world.menu.getScale().y + ob[2]*world.menu.getScale().y &&
               blob.getPosition().y < ob[1]*world.menu.getScale().y + ob[3]*world.menu.getScale().y &&
               blob.getPosition().y + blob.getGlobalBounds().height > ob[1]*world.menu.getScale().y)
                menuCollision(direction, ob);
        }
            
    }
    
    void menuAnimation()
    {
        if (timey%6 == 0){
            if ((left && !right) || (right && !left) || (up && !down) || (down && !up))
            animationPhase += 1;
        }
        
        if (left && !right)
            animationY = 0;
        else if ((right && !left))
            animationY = 8;
            
        switch (animationPhase)
        {
            case 1:
                animationX = 0;
                break;
            case 2:
                animationX = 12;
                break;
            case 3:
                animationX = 24;
                animationPhase = 0;
                break;
            default:
                animationX = 24;
                animationPhase = 0;
                break;
            
        }
        
        blob.setTextureRect(sf::IntRect(animationX, animationY, 12, 8));
        blob.setScale(gridSize/12*0.7,gridSize/12*0.7);
        blob.setPosition(pos[0], pos[1]);
    }
    
    void onLevel(bool play)
    {
        for (int i = 0;i<world.button.size();i++){
        if ((pos[0] + blob.getGlobalBounds().width > world.button.at(i).butt.getPosition().x
             && pos[0] < world.button.at(i).butt.getPosition().x + world.button.at(i).butt.getGlobalBounds().width) &&
            (pos[1] + blob.getGlobalBounds().height > world.button.at(i).butt.getPosition().y
                 && pos[1] < world.button.at(i).butt.getPosition().y + world.button.at(i).butt.getGlobalBounds().height))
        {
            if (play && levels.at(i+1).state != 0){
                titleScreen = false;
                curLevel = i+1;
                theLevel.levelReset(levels.at(curLevel).map);
                theLevel.draw();
                pos[0] = levels.at(curLevel).startPos[0];
                pos[1] = levels.at(curLevel).startPos[1];
            }else if(play && levels.at(i+1).state == 0){
                world.button.at(i).buttonPress(false);
            } else {
                world.button.at(i).buttonPress(true);
            }
            return;
        }
        }
        for(auto& i:world.button){
            i.buttonPress(false);
        }
    }
};

player player(0, 420);

int main(int, char const**)
{
    // Create the surface which we will draw all non-moving objects
    if (!surface.create(screenWidth, screenHeight))
        return EXIT_FAILURE;

    if (!movingSurface.create(screenWidth, screenHeight))
        return EXIT_FAILURE;
    
    if (!foregroundSurface.create(screenWidth, screenHeight))
        return EXIT_FAILURE;
    // Set the Icon

    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "Resources/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    if(!font.loadFromFile(resourcePath()+"Resources/bubble.ttf"))
        return EXIT_FAILURE;
    
    if(!niceMusic.openFromFile(resourcePath()+"Resources/music/"+songs[0]))
        return EXIT_FAILURE;
    
    theLevel.draw();
    
    sf::Sprite surfaceSprite(surface.getTexture());
    sf::Sprite surfaceMove(movingSurface.getTexture());
    sf::Sprite foregroundSprite(foregroundSurface.getTexture());
    
    // Start the game loop
    while (window.isOpen())
    {
        sf::sleep(sf::milliseconds(10));
        timey += 1;
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            
            if (event.type == sf::Event::KeyPressed){
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::W:
                        up = true;
                        break;
                    case sf::Keyboard::A:
                        left = true;
                        break;
                    case sf::Keyboard::S:
                        down = true;
                        break;
                    case sf::Keyboard::D:
                        right = true;
                        break;
                        
                    case sf::Keyboard::Space:
                        space = true;
                        if (!titleScreen)
                        {
                            if (!player.jumped && player.canMove){
                            player.velY = -5*gridSize/24;
                            player.jumped = true;
                            }
                            if (win){
                                titleScreen = true;
                                for(auto& i:world.button){
                                    i.buttonUpdate();
                                }
                                win = false;
                                player.canMove = true;
                                gridSize = 120;
                                player.blob.setScale(gridSize/12*0.7,gridSize/12*0.7);
                                player.pos[0] = worlds.at(curWorld).pos[0];
                                player.pos[1] = worlds.at(curWorld).pos[1];
                            }
                            if (player.die){
                                niceMusic.stop();
                                player.canMove = true;
                                player.die = false;
                                theLevel.levelReset(levels.at(curLevel).map);
                                theLevel.draw();
                                player.pos[0] = levels.at(curLevel).startPos[0];
                                player.pos[1] = levels.at(curLevel).startPos[1];
                                player.blob.setPosition(levels.at(curLevel).startPos[0], levels.at(curLevel).startPos[1]);
                            }
                        } else {
                            player.onLevel(false);
                        }
                        break;
                        
                    default:
                        break;

                }
            }
            else if (event.type == sf::Event::KeyReleased){
                switch (event.key.code)
                {
                    case sf::Keyboard::W:
                        up = false;
                        break;
                    case sf::Keyboard::A:
                        left = false;
                        break;
                    case sf::Keyboard::S:
                        down = false;
                        break;
                    case sf::Keyboard::D:
                        right = false;
                        break;
                    case sf::Keyboard::Space:
                        space = false;
                        if(titleScreen){
                            for(auto& i:world.button){
                                i.buttonPress(false);
                            }
                            player.onLevel(true);
                        }
                        break;
                    default:
                        break;
            }
                
            }
            else if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left){
                    if(event.mouseButton.x > theLevel.exit.getPosition().x - theLevel.exit.getGlobalBounds().width
                       && event.mouseButton.x < theLevel.exit.getPosition().x
                       && event.mouseButton.y < theLevel.exit.getPosition().y + theLevel.exit.getGlobalBounds().height
                       && event.mouseButton.y > theLevel.exit.getPosition().y){
                        titleScreen = true;
                        win = false;
                        player.canMove = true;
                        player.blob.setScale(70,70);
                        gridSize = 120;
                        std::cout << player.blob.getScale().x << std::endl;
                        player.pos[0] = worlds.at(curWorld).pos[0];
                        player.pos[1] = worlds.at(curWorld).pos[1];
                    }
                }
            }
        }

        
        
        // Clear screen
        window.clear();
        
        if (!titleScreen){
            if (player.die == false){
                player.update();
                player.animation();
            }
            // Draw the texture
                
            movingSurface.draw(player.blob);
            
            for (auto& i:theLevel.platforms){
                i.update();
                movingSurface.draw(i.platform);
            }
            
            for (auto& i:theLevel.waters){
                if(timey%24 == 0)
                    i.update();
                movingSurface.draw(i.leau);
            }
            
            if (win)
            {
                sf::Text youWin("Level "+std::to_string(curLevel)+" complete!",font,96);
                youWin.setFillColor(sf::Color::Black);
                youWin.setOrigin(youWin.getLocalBounds().width/2, youWin.getLocalBounds().height/2);
                youWin.setPosition((screenWidth/2)+4, (screenHeight/2.5)+4);
                movingSurface.draw(youWin);
                youWin.setFillColor(sf::Color::White);
                youWin.setPosition((screenWidth/2), (screenHeight/2.5));
                movingSurface.draw(youWin);
                if (timey%20 == 0)
                    blink = !blink;
                if (!blink){
                    sf::Text continuu("Press SPACE to continue...",font,16);
                    continuu.setOrigin(continuu.getLocalBounds().width/2, continuu.getLocalBounds().height/2);
                    continuu.setPosition(screenWidth/2, screenHeight*0.75);
                    movingSurface.draw(continuu);
                
                }
            }
            
            // This code only runs if noob (:
            if (player.die == true){
                sf::Text killMessage("You Died ):",font,96);
                killMessage.setOrigin(killMessage.getLocalBounds().width/2, killMessage.getLocalBounds().height/2);
                killMessage.setPosition(screenWidth/2, screenHeight/2);
                movingSurface.draw(killMessage);
            }
        } else
        {
            if (space)
                player.onLevel(false);
            player.menuUpdate();
            player.menuAnimation();
            surface.draw(world.menu);
            foregroundSurface.draw(world.foreground);
            for(auto& i:world.button)
            {
                movingSurface.draw(i.butt);
            }
            
            movingSurface.draw(player.blob);
        }
        surface.display();
        movingSurface.display();
        foregroundSurface.display();
        
        window.draw(surfaceSprite);
        window.draw(surfaceMove);
        window.draw(foregroundSprite);
        window.display();
        
        movingSurface.clear(sf::Color(255,255,255,0));
    }

    return EXIT_SUCCESS;
}
