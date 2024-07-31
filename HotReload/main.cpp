#include <iostream>
#include <dlfcn.h>
#include "SomeLib.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    void* _handle = dlopen("SomeLibrary/libSomeLib.dylib", RTLD_LAZY);

    typedef void (*Hello)();

    auto _hello = (Hello)dlsym(_handle, "Hello");

    _hello();

    dlclose(_handle);

    return 0;
}
