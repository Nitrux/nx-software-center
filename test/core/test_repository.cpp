#include <QtTest/QtTest>

#include <QList>

#include "entities/Application.h"
#include "entities/Repository.h"

class Test_Repository : public QObject {
Q_OBJECT

private slots:

    void containsApplication() {
        Repository r;
        Application a("testApp", "1.0");
        r.add(a);

        Q_ASSERT(r.contains(a.getId()));
        Q_ASSERT(!r.contains("none-v1"));
    }

    void notContainsApplication() {
        Repository r;
        Q_ASSERT(!r.contains("none-v1"));
    }

    void getExistentApplication() {
        Repository r;
        Application a("testApp", "1.0");
        r.add(a);

        try {
            Application a1 = r.get(a.getId());
            QCOMPARE(a, a1);
        } catch (ApplicationNotFoundException) {
            QFAIL("ApplicationNotFoundException throw but not expected");
        }
    }

    void getNonExistentApplication() {
        Repository r;

        try {
            Application a = r.get("unknown-v0.1");
            QFAIL("ApplicationNotFoundException not throw!");
        } catch (ApplicationNotFoundException) {
        }
    }

    void countAll() {
        Repository r;

        r.add(Application("app", "1"));
        r.add(Application("app", "2"));
        r.add(Application("app", "3"));

        QCOMPARE(r.countAll(), 3);
    }

    void countByName() {
        Repository r;
        r.add(Application("app1", "1"));
        r.add(Application("app1", "2"));
        r.add(Application("app1", "3"));
        r.add(Application("app2", "1"));
        r.add(Application("app2", "2"));

        QCOMPARE(r.countByName(), 2);
    }

    void getAll() {
        Repository r;
        QList<Application> apps;
        apps << Application("app1", "1");
        apps << Application("app1", "2");
        apps << Application("app1", "3");
        apps << Application("app2", "1");

        for (const Application &a : apps)
            r.add(a);

        QCOMPARE(apps, r.getAll());
    }

    void getAllLatestVersions() {
        Repository r;
        QList<Application> apps;
        apps << Application("app1", "1");
        apps << Application("app1", "2");
        apps << Application("app1", "3");
        apps << Application("app2", "1");

        for (const Application &a : apps)
            r.add(a);

        QList<Application> expected;
        expected << Application("app1", "3");
        expected << Application("app2", "1");
        QCOMPARE(expected, r.getAllLatestVersions());
    }

    void getAllVersion() {
        Repository r;
        QList<Application> apps;
        apps << Application("app1", "2");
        apps << Application("app1", "1");
        apps << Application("app1", "3");
        apps << Application("app2", "1");

        for (const Application &a : apps)
            r.add(a);

        QList<Application> expected;
        expected << Application("app1", "1");
        expected << Application("app1", "2");
        expected << Application("app1", "3");
        QCOMPARE(expected, r.getAllVersions("app1"));
    }

    void getLatestVersion() {
        Repository r;
        QList<Application> apps;
        apps << Application("app1", "2");
        apps << Application("app1", "1");
        apps << Application("app1", "3");

        for (const Application &a : apps)
            r.add(a);

        Application expected("app1", "3");
        QCOMPARE(expected, r.getLatestVersion("app1"));
    }

    void removeAll() {
        Repository r;
        QList<Application> apps;
        apps << Application("app1", "2");
        apps << Application("app1", "1");
        apps << Application("app1", "3");
        apps << Application("app2", "1");

        for (const Application &a : apps)
            r.add(a);

        r.removeAll();
        QList<Application> expected;
        QCOMPARE(expected, r.getAll());
    }

    void removeAllVersions() {
        Repository r;
        QList<Application> apps;
        apps << Application("app1", "2");
        apps << Application("app1", "1");
        apps << Application("app1", "3");
        Application app2("app2", "1");
        apps << app2;

        for (const Application &a : apps)
            r.add(a);

        r.removeAllVersions("app1");
        QList<Application> expected;
        expected << app2;
        QCOMPARE(expected, r.getAll());
    }

    void remove() {
        Repository r;
        QList<Application> apps;
        apps << Application("app1", "1");
        apps << Application("app1", "2");
        apps << Application("app1", "3");
        Application app2("app2", "1");
        apps << app2;

        for (const Application &a : apps)
            r.add(a);

        r.remove(app2.getId());

        QList<Application> expected(apps);
        expected.removeAll(app2);
        QCOMPARE(expected, r.getAll());
    }
};

QTEST_MAIN(Test_Repository)

#include "test_repository.moc"
