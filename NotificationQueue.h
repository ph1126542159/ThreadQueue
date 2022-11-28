#ifndef NOTIFICATIONQUEUE_H
#define NOTIFICATIONQUEUE_H
#include "Notification.h"
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
namespace PH {
class NotificationCenter;
class NotificationQueue
{
public:
    NotificationQueue();

    ~NotificationQueue();

    void clear();

    bool hasIdleThreads()const;

    void remove(Notification::Ptr pNf);

    bool empty()const;

    int size()const;

    void wakeupAll();

    void dispatch(NotificationCenter& notificationCenter);

    void enqueueNotification(Notification::Ptr pNotification);

    void enqueueUrgentNotification(Notification::Ptr pNotification);

    Notification::Ptr dequeueNotification();

    Notification::Ptr waitDequeueNotification();

    Notification::Ptr waitDequeueNotification(long milliseconds);
protected:
    Notification::Ptr popOne();

private:
    typedef std::deque<Notification::Ptr> NfQueue;
    class WaitInfo
    {
    public:
        Notification::Ptr pNf;
        void set(){
            std::unique_lock<std::mutex> lock(_mutex);
            nfAvailable.notify_one();
        }
        void wait(){
            std::unique_lock<std::mutex> lock(_mutex);
            nfAvailable.wait(lock);
        }
        bool tryWait(unsigned long mesc){
            std::unique_lock<std::mutex> lock(_mutex);
            return nfAvailable.wait_for(lock,std::chrono::milliseconds(mesc))==std::cv_status::timeout;
        }
    private:
        mutable std::mutex _mutex;
        std::condition_variable_any nfAvailable;
    };

    typedef std::deque<WaitInfo*> WaitQueue;

    NfQueue _nfQueue;

    WaitQueue _waitQueue;

    mutable std::mutex _mutex;
};
}



#endif // NOTIFICATIONQUEUE_H
