#include "Context.h"
#include "../Examples/SimpleExample/SimpleExample.h"
#include "../Examples/SimpleExample2/SimpleExample2.h"

bool ExampleRegistry::registerExamples()
{
    registerSimpleExample();
    registerSimpleExample2();
    return true;
}
