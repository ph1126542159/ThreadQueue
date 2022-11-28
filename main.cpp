#include <QCoreApplication>
#include "NotificationQueue.h"
#include <QThread>
#include <QDebug>
class WorkData : public PH::Notification{
public:
    using Ptr=PH::AutoPtr<WorkData>;
    WorkData(int num):
        _num(num){

    }
    int getData(){return _num;}
private:
    int _num;
};
class Work : public QThread{
public:
    Work(PH::NotificationQueue& queue):_queue(queue){

    }
    void run()override{
        while (true) {
            PH::Notification::Ptr pNf=_queue.waitDequeueNotification();
            WorkData::Ptr workData=pNf.cast<WorkData>();
            if(nullptr==workData) return;
            int num=workData->getData();
            qDebug()<<"resuklt:"<<num;
        }
    }

private:
    PH::NotificationQueue& _queue;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PH::NotificationQueue queue;
    Work w(queue);
    w.start();
    for(int i=0;i<100000;i++){
        PH::Notification::Ptr ptr=new WorkData(i);
        queue.enqueueNotification(ptr);
    }
    return a.exec();
}
