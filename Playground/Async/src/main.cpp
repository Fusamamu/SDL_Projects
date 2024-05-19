#include<iostream>
#include<future>

int square(int _value)
{
    return _value*_value;

}

int main()
{
    std::future<int> _future = std::async(&square, 100);

    int _result = _future.get();
    
    std::cout << _result << std::endl;
    return -1;
};
