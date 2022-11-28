#include "Notification.h"
#include <typeinfo>
namespace PH {
Notification::Notification()
{

}

std::string Notification::name() const
{
    return typeid(*this).name();
}

Notification::~Notification()
{

}

}
