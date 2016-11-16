#pragma once

#include <medTestExport.h>
#include <QList>
#include <QMetaObject>
#include <QMetaType>
#include <QObject>

class MEDTEST_EXPORT medTestSuite : public QObject
{
    Q_OBJECT

public:
    static int runAll(int argc, char** argv);

    template<typename TYPE>
    static void registerSuite()
    {
        registry.append(&TYPE::staticMetaObject);
    }

    Q_INVOKABLE medTestSuite() {}
    medTestSuite(const medTestSuite& other);

protected:
    template<typename TYPE>
    void addTest()
    {
        tests.append(TYPE::staticMetaObject);
    }

private:
    static QList<const QMetaObject*> registry;

    QList<const QMetaObject*> tests;

    virtual int run(int argc, char** argv);
};
