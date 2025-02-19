#include "Examples/Triangle/Triangle.h"

bool registerTriangle()
{
    using namespace Triangle;
    ExampleRegistry::registerExample("Triangle", Init, Update, Draw, Quit);
    return true;  // Static initializer
}
