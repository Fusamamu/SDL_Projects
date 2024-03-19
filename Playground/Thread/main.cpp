#include <iostream>
#include <thread>

void Hello()
{
    std::cout << "Hello world" << 'n';
}
int main()
{
    std::thread _t (Hello);
    _t.join();
    return 0;
}