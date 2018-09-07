#include <gtest/gtest.h>

#include <QCoreApplication>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
