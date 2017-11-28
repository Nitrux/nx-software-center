#include <QtTest/QtTest>

#include "entities/application.h"

class Test_Application : public QObject {
  Q_OBJECT

 private slots:
  void identity() {
    Application a("test", "v0.1");

    QCOMPARE(a.getName(), QString("test"));
    QCOMPARE(a.getVersion(), QString("v0.1"));
    QCOMPARE(QString("test-v0.1"), a.getId());
  };

  void compare() {
    Application a("test", "v0.1");
    Application b("test", "v0.1");

    QCOMPARE(a, b);
  }
};

QTEST_MAIN(Test_Application)
#include "test_application.moc"
