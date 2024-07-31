#include <iostream>
#include <thread>
#include <condition_variable>
#include <chrono>

std::mutex gMut;
std::condition_variable gCon;

bool IsComplete = false;

int main()
{
    int _result = 0;

    std::thread _reporter([&] {
        std::unique_lock<std::mutex> _lock(gMut);

        if(!IsComplete)
            gCon.wait(_lock);

        std::cout << "Reporter called" << '\n';
        std::cout << _result << '\n';
    });

    std::thread _worker([&]{
        std::unique_lock<std::mutex> _lock(gMut);

        _result = 20;

        std::cout << "Start wait" << '\n';
        std::this_thread::sleep_for(std::chrono::seconds (5));
        std::cout << "Stop wait" << '\n';

        gCon.notify_one();

    });

    _reporter.join();
    _worker.join();

    return 0;
}