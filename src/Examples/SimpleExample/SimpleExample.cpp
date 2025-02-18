#include "SimpleExample.h"

bool registerSimpleExample()
{
    using namespace SimpleExample;
    ExampleRegistry::registerExample("SimpleExample", Init, Update, Draw, Quit);
    return true;  // Static initializer
}
