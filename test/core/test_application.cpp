#include <QtTest/QtTest>

#include "entities/Application.h"

class Test_Application : public QObject {
Q_OBJECT

private slots:

    void identity() {
        Application a("test", "v0.1");

        QCOMPARE(a.getCodeName(), QString("test"));
        QCOMPARE(a.getVersion(), QString("v0.1"));
        QCOMPARE(QString("test-v0.1"), a.getId());
    };

    void compare_equal() {
        Application a("test", "v0.1");
        Application b("test", "v0.1");

        QCOMPARE(a, b);
    }

    void compare_different() {
        Application a("test", "v0.1");
        Application b("testl", "v0.1");

        Q_ASSERT(a != b);
    }

    void compare_lower() {
        Application a("test", "v0.1");
        Application b("test", "v0.2");

        Q_ASSERT(a < b);
    }

    void compare_same_name() {
        Application a("test", "v0.1");
        Application b("test", "v0.2");

        Q_ASSERT(a.compare_by_name(b) == 0);
    }

    void compare_different_name() {
        Application a("test", "v0.1");
        Application b("notest", "v0.1");

        Q_ASSERT(a.compare_by_name(b) != 0);
    }

    void compare_lower_version() {
        Application a("test", "v0.1");
        Application b("test", "v0.2");

        Q_ASSERT(a.compare_by_version(b) < 0);
    }

    void compare_grether_version() {
        Application a("test", "v0.3");
        Application b("test", "v0.2");

        Q_ASSERT(a.compare_by_version(b) > 0);
    }

    void test_set_get_description() {
        Application a("test", "v0.2");
        QString description = "Test Application description";
        a.setDescription(description);

        QCOMPARE(description, a.getDescription());
    }
};

QTEST_MAIN(Test_Application)

#include "test_application.moc"
