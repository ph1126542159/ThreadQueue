#ifndef ABSTRACTOBSERVER_H
#define ABSTRACTOBSERVER_H
#include "Notification.h"

namespace PH {
class AbstractObserver
{
public:
    AbstractObserver();

    AbstractObserver(const AbstractObserver& observer);

    virtual ~AbstractObserver();

    AbstractObserver& operator=(const AbstractObserver& observer);

    virtual void notify(Notification* pNf)const=0;

    virtual bool equals(const AbstractObserver& observer)const=0;

    virtual bool accepts(Notification* pNf,const char* pName=0)const=0;

    virtual AbstractObserver* clone()const=0;

    virtual void disable()=0;
};

}


#endif // ABSTRACTOBSERVER_H
