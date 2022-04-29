
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

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
#include <vector>
#include <iostream>
#include <map>
#include <array>

using namespace std;

sf::RenderTexture surface;
int gridSize;
int gridWidth;
int gridHeight;
sf::Texture spritesheet;

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
    {22,{320,164,32,32}},
    {23,{320,192,32,32}},
    {24,{320,224,32,32}},
    {25,{288,160,32,32}},
    {26,{480,160,32,32}}
};

int elements = 26;

void drawGrid()
{
    for (int i=0; i<=gridWidth; i++)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(i*gridSize, 0)),
            sf::Vertex(sf::Vector2f(i*gridSize, 1080))
        };
            
        line[0].color = sf::Color(60,113,106,255);
        line[1].color = sf::Color(60,113,106,255);
        
        surface.draw(line, 2, sf::Lines);
        //std::cout << "amogus \n";
    }
    
    for (int i=0; i<=gridHeight; i++)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(0, i*gridSize)),
            sf::Vertex(sf::Vector2f(1920, i*gridSize))
        };
            
        line[0].color = sf::Color(60,113,106,255);
        line[1].color = sf::Color(60,113,106,255);
        
        surface.draw(line, 2, sf::Lines);
        //std::cout << "amogus \n";
    }
}

void printData(vector<vector<int>> data)
{
    cout << "{";
    for(int i = 0;i<gridHeight;i++){
        cout << "{";
        for(int o = 0;o<gridWidth;o++){
            cout << data.at(i).at(o) << ",";
        }
        cout << "},";
        cout << endl;
    }
    cout << "} \n";
    
}

bool test(sf::Vector2f mouse,sf::FloatRect rect)
{
    if (rect.contains(mouse))
        return true;
    else
        return false;
}

void drawAll(vector<vector<int>> data)
{
    surface.clear();
    drawGrid();
    
    sf::Sprite block;
    block.setTexture(spritesheet);
    
    for (int i=0;i<gridHeight;i++)
    {
        for (int o=0;o<gridWidth;o++)
        {
            if(data[i][o] != 0 && !(data[i][o]<=20 && data[i][o] >= 17)){
            
                block.setTextureRect(sf::IntRect(textures.at(data[i][o])[0],textures.at(data[i][o])[1],textures.at(data[i][o])[2],textures.at(data[i][o])[3]));
                block.setScale((float)gridSize/32, (float)gridSize/32);
                block.setPosition(o*gridSize, i*gridSize);
                surface.draw(block);
            }
            else if (data[i][o]<=20 && data[i][o] >= 17){
                block.setTextureRect(sf::IntRect(textures.at(17)[0],textures.at(17)[1],textures.at(17)[2],textures.at(17)[3]));
                block.setScale((float)gridSize/32, (float)gridSize/32);
                block.setPosition(o*gridSize, i*gridSize);
                surface.draw(block);
            }
        }
    }
}

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1920,1080), "SFML window");

    if (!spritesheet.loadFromFile(resourcePath()+ "resources/stage.png"))
        return EXIT_FAILURE;
    surface.create(1920, 1080);
    
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "resources/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    
    //std::cin >> gridSize;
    gridSize = 80;
    gridHeight = 1080/gridSize;
    gridWidth = 1920/gridSize;
    
    if (1080%gridSize != 0)
        gridHeight += 1;
    if (1920%gridSize != 0)
        gridHeight += 1;
    drawGrid();
    vector<vector<int>> data = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,3,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,15,0,0,0,0,0,15,0,20,0,3,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,16,0,0,19,0,0,16,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,16,0,0,0,0,0,16,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0},
        {0,19,16,0,0,0,0,0,16,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,3,14,14,14,14,14,12,14,14,14,12,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,3,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {2,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0}
        };
    //vector<vector<int>> data(gridHeight, vector<int>(gridWidth,0));
    
    
    
    // Start the game loop
    while (window.isOpen())
    {
        sf::sleep(sf::milliseconds(sf::Int32(20)));
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                printData(data);
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                printData(data);
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;
                    
                    for(int i = 0;i<gridHeight;i++){
                        for(int o = 0;o<gridWidth;o++){
                            if(test(sf::Vector2f(x,y),sf::FloatRect(o*gridSize,i*gridSize,gridSize,gridSize))){
                                if(data[i][o] + 1 > elements)
                                    data[i][o] = 0;
                                else
                                    data[i][o] += 1;
                                printData(data);
                                drawAll(data);
                                break;
                            }
                        }
                    }
                    
                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;
                    
                    for(int i = 0;i<gridHeight;i++){
                        for(int o = 0;o<gridWidth;o++){
                            if(test(sf::Vector2f(x,y),sf::FloatRect(o*gridSize,i*gridSize,gridSize,gridSize))){
                                if(data[i][o] -1 < 0)
                                    data[i][o] = elements;
                                else
                                    data[i][o] -= 1;
                                printData(data);
                                drawAll(data);
                                break;
                            }
                        }
                    }
                    
                }
            }
        }

        // Clear screen
        window.clear();
        
        surface.display();
        
        sf::Sprite sprite(surface.getTexture());
        window.draw(sprite);
        
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
