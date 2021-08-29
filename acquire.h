#ifndef ACQUIRE_H
#define ACQUIRE_H

#include <QThread>
extern "C"
{
#include "stdafx.h"
#include "conio.h"
#include "USB3102A.h"
}

class acquire : public QThread
{
    Q_OBJECT

public:
    explicit acquire(QObject *parent = nullptr);

    double fAnlgArray[4*100];

    QString filepath;

    void acquiresys();

    bool flag=0,saveflag=0;

    void run();

signals:
    void acquireDone();

};

#endif // ACQUIRE_H
