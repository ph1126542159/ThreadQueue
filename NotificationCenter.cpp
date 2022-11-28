#include "NotificationCenter.h"
#include "AbstractObserver.h"
#include <assert.h>
namespace PH {
NotificationCenter::NotificationCenter()
{

}

NotificationCenter::~NotificationCenter()
{

}

void NotificationCenter::addObserver(const AbstractObserver &observer)
{
    std::unique_lock<std::mutex> lock(_mutex);
    std::shared_ptr<AbstractObserver> ptr;
    ptr.reset(observer.clone());
    _observers.push_back(ptr);
}

void NotificationCenter::removeObserver(const AbstractObserver &observer)
{
    std::unique_lock<std::mutex> lock(_mutex);
    for (ObserverList::iterator it = _observers.begin(); it != _observers.end(); ++it)
    {
        if (observer.equals(**it))
        {
            (*it)->disable();
            _observers.erase(it);
            return;
        }
    }
}

bool NotificationCenter::hasObserver(const AbstractObserver &observer) const
{
    std::unique_lock<std::mutex> lock(_mutex);
    for (const auto& p: _observers)
        if (observer.equals(*p)) return true;

    return false;
}

bool NotificationCenter::hasObservers() const
{
    std::unique_lock<std::mutex> lock(_mutex);

    return !_observers.empty();
}

void NotificationCenter::postNotification(Notification::Ptr pNotification)
{
    assert(pNotification.get()!=nullptr);
    std::unique_lock<std::mutex> lock(_mutex);
    ObserverList observersToNotify(_observers);
    lock.unlock();
    for (auto& p: observersToNotify)
    {
        p->notify(pNotification.get());
    }
}

size_t NotificationCenter::countObservers() const
{
    std::unique_lock<std::mutex> lock(_mutex);

    return _observers.size();
}

}
