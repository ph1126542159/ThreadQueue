#include "NotificationQueue.h"
#include "NotificationCenter.h"
#include <assert.h>
namespace PH {
NotificationQueue::NotificationQueue()
{

}

NotificationQueue::~NotificationQueue()
{
    try {
        clear();
    } catch (...) {
    }
}

void NotificationQueue::clear()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _nfQueue.clear();
}

bool NotificationQueue::hasIdleThreads() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return (!_waitQueue.empty());
}

void NotificationQueue::remove(Notification::Ptr pNf)
{
    std::unique_lock<std::mutex> lock(_mutex);
    NfQueue::iterator it = std::find(_nfQueue.begin(), _nfQueue.end(), pNf);
    if (it == _nfQueue.end()) return;
    _nfQueue.erase(it);
}

bool NotificationQueue::empty() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return _nfQueue.empty();
}

int NotificationQueue::size() const
{
    std::unique_lock<std::mutex> lock(_mutex);
    return static_cast<int>(_nfQueue.size());
}

void NotificationQueue::wakeupAll()
{
    std::unique_lock<std::mutex> lock(_mutex);
    for (auto p: _waitQueue) p->set();
    _waitQueue.clear();
}

void NotificationQueue::dispatch(NotificationCenter &notificationCenter)
{
    std::unique_lock<std::mutex> lock(_mutex);
    Notification::Ptr pNf = popOne();
    while (pNf)
    {
        notificationCenter.postNotification(pNf);
        pNf = popOne();
    }
}

void NotificationQueue::enqueueNotification(Notification::Ptr pNotification)
{
    assert(nullptr!=pNotification.get());
    std::unique_lock<std::mutex> lock(_mutex);
    if (_waitQueue.empty())
    {
        _nfQueue.push_back(pNotification);
    }
    else
    {
        WaitInfo* pWI = _waitQueue.front();
        _waitQueue.pop_front();
        pWI->pNf = pNotification;
        pWI->set();
    }
}

void NotificationQueue::enqueueUrgentNotification(Notification::Ptr pNotification)
{
    assert(nullptr!=pNotification.get());
    std::unique_lock<std::mutex> lock(_mutex);
    if (_waitQueue.empty())
    {
        _nfQueue.push_front(pNotification);
    }
    else
    {
        WaitInfo* pWI = _waitQueue.front();
        _waitQueue.pop_front();
        pWI->pNf = pNotification;
        pWI->set();
    }
}

Notification::Ptr NotificationQueue::dequeueNotification()
{
    std::unique_lock<std::mutex> lock(_mutex);
    return popOne();
}

Notification::Ptr NotificationQueue::waitDequeueNotification()
{
    Notification::Ptr pNf;
    WaitInfo* pWI = 0;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        pNf = popOne();
        if (nullptr!=pNf.get()) return pNf;
        pWI = new WaitInfo;
        _waitQueue.push_back(pWI);
    }
    pWI->wait();
    pNf = pWI->pNf;
    delete pWI;
    return pNf;
}

Notification::Ptr NotificationQueue::waitDequeueNotification(long milliseconds)
{
    Notification::Ptr pNf;
    WaitInfo* pWI = 0;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        pNf = popOne();
        if (nullptr!=pNf.get()) return pNf;
        pWI = new WaitInfo;
        _waitQueue.push_back(pWI);
    }
    if (pWI->tryWait(milliseconds))
    {
        pNf = pWI->pNf;
    }
    else
    {
        std::unique_lock<std::mutex> lock(_mutex);
        pNf = pWI->pNf;
        for (WaitQueue::iterator it = _waitQueue.begin(); it != _waitQueue.end(); ++it)
        {
            if (*it == pWI)
            {
                _waitQueue.erase(it);
                break;
            }
        }
    }
    delete pWI;
    return pNf;
}

Notification::Ptr NotificationQueue::popOne()
{
    Notification::Ptr pNf;
    if(!_nfQueue.empty()){
        pNf=_nfQueue.front();
        _nfQueue.pop_front();
    }
    return pNf;
}

}
