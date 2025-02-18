#include "SimpleExample2.h"

bool registerSimpleExample2()
{
    using namespace SimpleExample2;
    ExampleRegistry::registerExample("SimpleExample2", Init, Update, Draw, Quit);
    return true;  // Static initializer
}
