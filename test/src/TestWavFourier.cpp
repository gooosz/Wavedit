#include <QTest>

#include <QString>
#include <QVector>

#include "../../Wavedit/src/WavFourier.h"

class TestWavFourier : public QObject {
	Q_OBJECT

	QVector<double> *data;	// gets filled before every test
	WavFourier wavfourier;

private slots:
	void init();	// fill data vector
	void helloWorld();

	void testGetStuetzstellen_data();
	void testGetStuetzstellen();

	void testFreq_data();
	void testFreq();

	void testDFT();

	void testIDFT();
};

void TestWavFourier::helloWorld()
{
	QString str = "Hello";
	QVERIFY(QString::compare(str, "Test", Qt::CaseSensitive));
	QCOMPARE(str, "Hello");
}

void TestWavFourier::init()
{
	data = new QVector<double>();
	for (int i=0; i<4; i++) {
		data->push_back(i);
	}
}

void TestWavFourier::testGetStuetzstellen_data()
{
	QTest::addColumn<QVector<double>>("data");
	QTest::addColumn<QVector<double>>("stuetzstellen");

	// data.size, correct result are stored in table
	QTest::newRow("empty data") << QVector<double>() << QVector<double>({});
	QTest::newRow("data.size() = 4") << QVector<double>(4) << QVector<double>({0, M_PI/2.0, M_PI, 3.0*M_PI/2.0});
	QTest::newRow("data.size() = 7") << QVector<double>(7) << QVector<double>(
		{0, 2.0*M_PI/7.0, 4.0*M_PI/7.0, 6.0*M_PI/7.0, 8.0*M_PI/7.0, 10.0*M_PI/7.0, 12.0*M_PI/7.0}
	);
}

void TestWavFourier::testGetStuetzstellen()
{
	QFETCH(QVector<double>, data);
	QFETCH(QVector<double>, stuetzstellen);
	QCOMPARE(wavfourier.getStuetzstellen(data.size()), stuetzstellen);
}


void TestWavFourier::testFreq_data()
{
	QTest::addColumn<QVector<double>>("data");
	QTest::addColumn<QVector<double>>("sample_spacing");
	QTest::addColumn<QVector<double>>("frequency bins");


}

void TestWavFourier::testFreq()
{

}

void TestWavFourier::testDFT()
{

}

void TestWavFourier::testIDFT()
{

}


QTEST_MAIN(TestWavFourier)
#include "TestWavFourier.moc"
