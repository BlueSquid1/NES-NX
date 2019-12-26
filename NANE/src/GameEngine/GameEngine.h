#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <iostream> //std::string
#include <memory>
#include <SDL.h>

#ifdef __SWITCH__
#include <switch.h>
#endif

#include "Network/Logger.h"
#include "Graphics/GraphicsEngine.h"

class GameEngine
{
    private:
    std::unique_ptr<GraphicsEngine> graphics;

    bool shouldExit = false;

    public:
    GameEngine();
    bool Init(const std::string & serverIp);
    
    //returns false if fatal error has occured
    bool UserInput();

    //returns false if fatal error has occured
    bool Processing();

    //returns false if fatal error has occured
    bool Display();

    void Close();

    //getter and setters
    bool GetShouldExit();
};

#endif