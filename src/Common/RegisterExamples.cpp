#include "Context.h"
#include "Examples/SimpleExample/SimpleExample.h"
#include "Examples/Triangle/Triangle.h"

bool ExampleRegistry::registerExamples()
{
    registerSimpleExample();
    registerTriangle();
    return true;
}
