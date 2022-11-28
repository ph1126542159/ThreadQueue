#ifndef REFCOUNTEDOBJECT_H
#define REFCOUNTEDOBJECT_H
#include "AtomicCounter.h"
namespace PH {
class RefCountedObject
{
public:
    RefCountedObject();

    void duplicate() const;

    void release() const noexcept;

    int referenceCount() const;

protected:
    virtual ~RefCountedObject();
private:
    RefCountedObject(const RefCountedObject&);
    RefCountedObject& operator = (const RefCountedObject&);

    mutable AtomicCounter _counter;
};

inline int RefCountedObject::referenceCount() const
{
    return _counter.value();
}


inline void RefCountedObject::duplicate() const
{
    ++_counter;
}


inline void RefCountedObject::release() const noexcept
{
    try
    {
        if (--_counter == 0) delete this;
    }
    catch (...)
    {
    }
}


}
#endif // REFCOUNTEDOBJECT_H
