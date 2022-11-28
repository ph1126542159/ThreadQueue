#ifndef NOTIFICATIONCENTER_H
#define NOTIFICATIONCENTER_H
#include <vector>
#include <cstddef>
#include "Notification.h"
#include <mutex>
namespace PH {
class AbstractObserver;
class NotificationCenter
{
public:
    NotificationCenter();
    ~NotificationCenter();
    void addObserver(const AbstractObserver& observer);
    void removeObserver(const AbstractObserver& observer);
    bool hasObserver(const AbstractObserver& observer) const;
    bool hasObservers() const;
    void postNotification(Notification::Ptr pNotification);
    std::size_t countObservers() const;
private:
    typedef std::shared_ptr<AbstractObserver> AbstractObserverPtr;
    typedef std::vector<AbstractObserverPtr> ObserverList;

    ObserverList  _observers;
    mutable std::mutex _mutex;
};
}



#endif // NOTIFICATIONCENTER_H
