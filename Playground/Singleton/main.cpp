#include <iostream>

/*Singleton version 1*/
class Singleton_V1
{
public:
    static Singleton_V1& Get()
    {
        return s_Instance;
    }

    void Function()
    {

    }

    Singleton_V1(const Singleton_V1&) = delete;

private:
    Singleton_V1() { }

    static Singleton_V1 s_Instance;
};

Singleton_V1 Singleton_V1::s_Instance;

/*Singleton version 2*/
class Singleton_V2
{
public:
    static Singleton_V2& Get()
    {
        return s_Instance;
    }

    static float Float()
    {
        return Get().IFloat();
    }

    Singleton_V2(const Singleton_V2&) = delete;

private:
    Singleton_V2() { }

    float IFloat(){return 5.f;}

    static Singleton_V2 s_Instance;
};

Singleton_V2 Singleton_V2::s_Instance;

/*Singleton version 3*/
class Singleton_V3
{
public:
    static Singleton_V3& Get()
    {
        static Singleton_V3 s_Instance;
        return s_Instance;
    }

    static float Float()
    {
        return Get().IFloat();
    }

    Singleton_V3(const Singleton_V2&) = delete;

private:
    Singleton_V3() { }

    float IFloat(){return 5.f;}
};

int main()
{
    Singleton_V1::Get().Function();
    auto _valueA = Singleton_V2::Float();
    auto _valueB = Singleton_V3::Float();
    return 0;
}
