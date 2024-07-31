#include <iostream>
#include "Application.h"

int main()
{
    Application::GetInstance().Start();
    Application::GetInstance().Update();
    Application::GetInstance().Quit();
    return 0;
}
