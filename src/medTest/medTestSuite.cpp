#include "medTestSuite.h"

#include <cstdlib>
#include <QDebug>
#include <QTest>

QList<const QMetaObject*> medTestSuite::registry;

int medTestSuite::runAll(int argc, char** argv)
{
    int result = 0;

    qDebug() << "Running tests";

    foreach (const QMetaObject* metaObject, registry)
    {
        medTestSuite* testSuite = dynamic_cast<medTestSuite*>(metaObject->newInstance());
        result += testSuite->run(argc, argv);
        delete testSuite;
    }

    return (result == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int medTestSuite::run(int argc, char** argv)
{
    int result = 0;

    foreach (const QMetaObject* metaObject, tests)
    {
        QObject* test = metaObject->newInstance();
        result += QTest::qExec(test, argc, argv);
        delete test;
    }

    return (result == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
