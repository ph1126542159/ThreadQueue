#include "RefCountedObject.h"
namespace PH {
RefCountedObject::RefCountedObject(): _counter(1)
{
}


RefCountedObject::~RefCountedObject()
{
}
}
