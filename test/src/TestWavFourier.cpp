#include <QString>
#include <QTest>

class TestWavFourier : public QObject {
	Q_OBJECT

private slots:
	void hello();
};

void TestWavFourier::hello()
{
	QString str = "Hello";
	QVERIFY(str == "Hello");
	QCOMPARE(str, "Hello");
}



QTEST_MAIN(TestWavFourier)
#include "TestWavFourier.moc"
