#include <gtest/gtest.h>

#include <QList>
#include <QMetaType>
#include <QCoreApplication>

#include "entities/Application.h"
#include "entities/Upgrader.h"

Q_DECLARE_METATYPE(Application);

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qRegisterMetaType<Application>("Application");
    qRegisterMetaType<QList<Application>>("ApplicationList");
    qRegisterMetaType<UpgradeList>("UpgradeList");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
