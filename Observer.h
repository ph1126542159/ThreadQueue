#ifndef OBSERVER_H
#define OBSERVER_H
#include <mutex>
#include "AbstractObserver.h"
namespace PH{
template <class C,class N>
class Observer : public AbstractObserver{
public:
    typedef void (C::*Callback)(N*);

    Observer(C& object,Callback method):
        _pObject(&object),_method(method){

    }


    Observer(const Observer& observer):
        AbstractObserver(observer),_pObject(observer.object),_method(observer.method){

    }

    ~Observer(){

    }

    Observer& operator=(const Observer& observer){
        if(&observer!=this){
            _pObject=observer._pObject;
            _method=observer._method;
        }
        return *this;
    }

    void notify(Notification* pNf)const{
        std::unique_lock<std::mutex> lock(_mutex);
        if(!_pObject) return;
        N* pCastNf=dynamic_cast<N*>(pNf);
        if(!pCastNf) return;
        (_pObject->*_method)(pCastNf);
    }
    bool equals(const AbstractObserver& abstractObserver) const
    {
        const Observer* pObs = dynamic_cast<const Observer*>(&abstractObserver);
        return pObs && pObs->_pObject == _pObject && pObs->_method == _method;
    }

    bool accepts(Notification* pNf, const char* pName = 0) const
    {
        return dynamic_cast<N*>(pNf) && (!pName || pNf->name() == pName);
    }

    AbstractObserver* clone() const
    {
        return new Observer(*this);
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);

        _pObject = 0;
    }
private:
    Observer()=delete;

    C* _pObject;

    Callback _method;

    mutable std::mutex _mutex;
};
}
#endif // OBSERVER_H
