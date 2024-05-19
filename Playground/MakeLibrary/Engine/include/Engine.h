#pragma once

class Engine
{
public:
    //Render
    //Window

    Engine();
    ~Engine();

    void Init();
    void Update();
    void Quit();

    void ProcessInput();
    void Render();


};