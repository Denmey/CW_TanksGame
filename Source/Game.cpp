#include "Game.h"
#include "ResMngr.h"
#include "SpriteRenderer.h"
#include "GameObjects.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <vector>

// Game-related State data
SpriteRenderer  *Renderer;
// Data for game objects
GLfloat speed, baseSpeed = 80.0f; //turnTime считает время на каждом ходу, после обработки хода обнуляется
GLfloat turnTime, TimeForTurn = 0.2f, TimeForAction = 0.1f;//TimeForTurn - время на выбор действия, TimeForAction - время на совершение действия
std::vector<Tank> Tanks;
std::vector<Bullet> Bullets;
GLint TanksNumb = 4;
int map[15][20];

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ 

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // Take a seed for rand
    srand(time(NULL));
    // Load shaders
    ResourceManager::LoadShader("Data/Shaders/Sprite.vrtx", "Data/Shaders/sprite.frag", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("Data/Textures/TankBody.png", GL_TRUE, "TankBody");
    ResourceManager::LoadTexture("Data/Textures/TankTower.png", GL_TRUE, "TankTower");
    ResourceManager::LoadTexture("Data/Textures/Wall.png", GL_TRUE, "Wall");
    ResourceManager::LoadTexture("Data/Textures/Bullet.png", GL_TRUE, "Bullet");
    
    // Set render-specific controls
    // Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    ResourceManager::loadLevelFromFile(&map[0][0], "Data/Level.txt");

    //Load Tanks in memory
    for(int i = 0; i < TanksNumb; ++i){
        Tanks.push_back(Tank());
        GLint p=0,t=0;
        while (map[t][p]!=0){
            p=rand()%18;
            t=rand()%14;
            // std::cout<< "t = " << t << ", p = " << p << ", map[t,p]=" << map[t][p] << std::endl;
        }
        Tanks[i].setCell(p, t);
        Tanks[i].setBodyAngle(rand()%4*90);
        Tanks[i].setTowerAngle(Tanks[i].getBodyAngle());
        map[t][p] = 2+i;
        Tanks[i].setID(2+i);
    }
    for (int i = 0; i < 15; ++i){
        for (int k = 0; k < 20; ++k)
            std::cout << /*"map[" << i << "][" << k << "]=" <<*/ map[i][k]; //<< std::endl;
        std::cout << std::endl;
    }
}

void Game::Update(GLfloat dt)
{
    speed = baseSpeed * dt;
    if (turnTime < TimeForTurn) {
        for (int i = 1; i < Tanks.size(); ++i){
            Tanks[i].setAction(rand()%8);
        }
    }
    else if (turnTime < (TimeForTurn+TimeForAction)){
        for (int i = 0; i < Tanks.size(); ++i){
            if (Tanks[i].getAction()==Fire){
                GLint target = Tanks[i].fire(Bullets, &map[0][0]);
                Tanks[i].setAction(None);
                if (target==2){
                Render();
                while (1){
                    
                    std::cout<<"You lose!"<<std::endl;
                    getchar();
                    getchar();
                }
                }
                for (int k = 0; k < Tanks.size(); ++k)
                    if (Tanks[k].getID()==target){
                        map[Tanks[k].getYCell()][Tanks[k].getXCell()]=0;
                        Tanks.erase(Tanks.begin()+k);
                        break;
                    }
                    
            }
            else Tanks[i].doAction(dt, TimeForAction, &map[0][0]);
        }
    }
    else {
        turnTime=0;
        for (int i = 0; i < Tanks.size(); ++i)
        {
            Tanks[i].setAction(None);
            Tanks[i].checkData(Tanks[i].getID(), &map[0][0]);
        }
        if (Tanks.size()==1 && Tanks[0].getID()==2)
        while (1){
            std::cout<<"You win!"<<std::endl;
            getchar();
            getchar();
        }
        Bullets.clear();

    }
}


void Game::ProcessInput(GLfloat dt)
{
    speed = baseSpeed * dt;
    turnTime+=dt;
    if (turnTime < TimeForAction)
    {
    // std::cout<<"Input Processing"<<std::endl;
    if (checkButtonHold(GLFW_KEY_W))
        Tanks[0].setAction(MoveForward);
    else if (checkButtonHold(GLFW_KEY_S))
        Tanks[0].setAction(MoveBack);
    else if (checkButtonHold(GLFW_KEY_D))
        Tanks[0].setAction(TurnBodyRight);
    else if (checkButtonHold(GLFW_KEY_A))
        Tanks[0].setAction(TurnBodyLeft);
    else if (checkButtonHold(GLFW_KEY_RIGHT))
        Tanks[0].setAction(TurnTowerRight);
    else if (checkButtonHold(GLFW_KEY_LEFT))
        Tanks[0].setAction(TurnTowerLeft);
    else if (checkButtonHold(GLFW_KEY_UP))
        Tanks[0].setAction(Fire);
    }
};

void Game::Render()
{
    Texture2D TankBodyTexture  = ResourceManager::GetTexture("TankBody");
    Texture2D TankTowerTexture = ResourceManager::GetTexture("TankTower");
    Texture2D WallTexture = ResourceManager::GetTexture("Wall");
    Texture2D BulletTexture = ResourceManager::GetTexture("Bullet");

    //Draw Player's tank
	Renderer->DrawSprite(TankBodyTexture,
    glm::vec2(Tanks[0].getX(),Tanks[0].getY()), glm::vec2(CellSize,CellSize), glm::radians(Tanks[0].getBodyAngle()), glm::vec3(0.0f, 0.8f, 0.0f));
    Renderer->DrawSprite(TankTowerTexture,
    glm::vec2(Tanks[0].getX(),Tanks[0].getY()), glm::vec2(CellSize,CellSize), glm::radians(Tanks[0].getTowerAngle()), glm::vec3(0.0f, 0.8f, 0.0f));


	// Draw other Tanks
    for (int i = 1; i < Tanks.size(); ++i){
        Renderer->DrawSprite(TankBodyTexture,
        glm::vec2(Tanks[i].getX(),Tanks[i].getY()), glm::vec2(CellSize,CellSize), glm::radians(Tanks[i].getBodyAngle()), glm::vec3(0.9f, 0.0f, 0.0f));
        Renderer->DrawSprite(TankTowerTexture,
        glm::vec2(Tanks[i].getX(),Tanks[i].getY()), glm::vec2(CellSize,CellSize), glm::radians(Tanks[i].getTowerAngle()), glm::vec3(0.9f, 0.0f, 0.0f));
        
    }
    // Draw Bullets
    for (int i = 0; i < Bullets.size(); ++i){
        Renderer->DrawSprite(BulletTexture,
        glm::vec2(Bullets[i].X,Bullets[i].Y), glm::vec2(Bullets[i].Width,Bullets[i].Height), 0, glm::vec3(1.0f, 1.0f, 0.0f));
    }
    
    // Draw Walls
    for (int i = 0; i < 15; ++i)
    for (int k = 0; k < 20; ++k)
    if (map[i][k]==1) 
    Renderer->DrawSprite(WallTexture,
    glm::vec2(k*CellSize,i*CellSize), glm::vec2(CellSize,CellSize), 0, glm::vec3(0.0f, 0.6f, 0.6f));


    };




GLboolean Game::checkButtonPress(GLint button){
    if (this->Keys[button]==GL_TRUE)
		if (this->Keys2[button]==GL_FALSE){
            this->Keys2[button]=GL_TRUE;
            return GL_TRUE;
        }
    checkButtonRelease(button);
    return GL_FALSE;
}
GLboolean Game::checkButtonHold(GLint button){
    if (this->Keys[button]==GL_TRUE)
        return GL_TRUE;
    return GL_FALSE;
}
GLboolean Game::checkButtonRelease(GLint button){
    if (this->Keys[button]==GL_FALSE)
		if (this->Keys2[button]==GL_TRUE){
            this->Keys2[button]=GL_FALSE;
            return GL_TRUE;
	    }
    return GL_FALSE;    
}